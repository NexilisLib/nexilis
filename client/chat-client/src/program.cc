#include "program.hh"
#include "debug.hh"
#include "nexilis_client.hh"

#include <ncurses.h>

#include <nexilis/json.hh>
#include <nexilis/log.hh>
#include <nexilis/logger/file_handler.hh>
#include <nexilis/packet.hh>

Program::Program(int argc, char** argv)
    : m_argc(argc),
      m_argv(argv),
      m_window(),
      m_menu(),
      m_nexilisClient()
{
    m_nexilisClient.start();
}

Program::~Program()
{
    end();
}

void Program::inputHandler()
{
    m_input = wgetch(m_window.getWindow());

    switch (m_input)
    {
        case KEY_RESIZE:
        {
            updateScreenSize();
            return;
        }
    }

    /// Menu update ritual.
    if (m_menu.getState() == Menu::State::menu)
    {
        switch (tolower(m_input))
        {
            case 'j':
            case KEY_DOWN:
            {
                if (m_choice < MENU_ITEM_COUNT)
                {
                    ++m_choice;
                }
                else if (m_choice == MENU_ITEM_COUNT)
                {
                    m_choice = 0;
                }
                break;
            }
            case 'k':
            case KEY_UP:
            {
                if (m_choice > 0)
                {
                    --m_choice;
                }
                else if (m_choice == 0)
                {
                    m_choice = MENU_ITEM_COUNT;
                }
                break;
            }
        }

        // Press enter in menu to launch action.
        if (m_input == 10)
        {
            switch (m_choice)
            {
                // Chat.
                case 0:
                {
                    m_menu.changeState(Menu::State::chat);
                    wclear(m_window.getWindow());
                    break;
                }

                // Info.
                case 1:
                {
                    m_menu.changeState(Menu::State::infopage);
                    wclear(m_window.getWindow());
                    break;
                }

                // Quit.
                case 2:
                {
                    end();
                    break;
                }
            }
        }
    }

    else if (m_menu.getState() == Menu::State::chat)
    {
        // Get information about the chat.
        if (updateRooms)
        {
            debug("Sent message to the server asking for server data");
            sendTCPMessage(nexilis::Packet::Info::rooms());

            // This will work for now, but this needs to be retriggered from somewhere.
            updateRooms = false;
        }

        if (m_input == 10)
        {
            debug("Pressed enter in chat mode");
            std::stringstream ss;
            ss << "Room amount: " << m_chat.getRoomAmount();
            debug(ss.str());
        }

        switch (tolower(m_input))
        {
            case 'j':
            case KEY_DOWN:
            {
                debug("Pressed key down");
                uint64_t copiedClientId = m_nexilisClient.getClientAPI().getClientId();
                std::string date = nexilis::Util::getDateAndTime();
                std::stringstream ss;
                ss << "../../../logs/" << copiedClientId << ":" << date << "log.json";
                nexilis::Json::saveToFile(m_nexilisClient.getClientAPI().getCurrentMessage(), ss.str());
                break;
            }

            case 'k':
            case KEY_UP:
            {
                debug("Pressed key up");
                debugObject(m_nexilisClient.getClientAPI().getCurrentMessage());
                break;
            }
        }
    }
}

void Program::updateScreenSize()
{
    // TODO
}

void Program::sendTCPMessage(const std::vector<uint8_t>& message)
{
    m_nexilisClient.getTCPClient().sendMessage(message);
}

void Program::sendUDPMessage(const std::vector<uint8_t>& message)
{
    m_nexilisClient.getUDPClient().sendMessage(message);
}

void Program::debugObject(boost::json::object object)
{
    std::string stringObject = boost::json::serialize(object);
    debug(stringObject);
}

void Program::end()
{
    endwin();
    exit(0);
}

void Program::readMessage(boost::json::object object)
{
    if (object != m_currentMessage)
    {
        m_currentMessage = object;
        parseMessage(m_currentMessage);
    }
}

void Program::parseMessage(boost::json::object object)
{
    // Parsing message.
    if (object.contains("nexilis_status"))
    {
        if (object["nexilis_status"] != 2)
        {
            debug("Nexilis status other than 2 in the client code!");
        }
    }

    if (object.contains("type"))
    {
        if (object["type"] == "roomData")
        {
            debug("Found the room data message");
            auto rooms = object.at("rooms").as_array();
            for (const auto& room : rooms)
            {
                std::string name = room.at("name").as_string().c_str();
                int maxSize = static_cast<int>(room.at("maxSize").as_int64());
                uint64_t id;

                if (room.at("id").if_uint64())
                {
                    id = room.at("id").as_uint64();
                }
                else if (room.at("id").if_int64())
                {
                    id = static_cast<uint64_t>(room.at("id").as_int64());
                }
                else
                {
                    id = 0;
                }
                m_chat.addRoom(Chat::Room(name, maxSize, id));
            }
        }
    }
}

void Program::update()
{
    inputHandler();
    m_menu.update(m_window.getWindow(), m_choice);
    readMessage(m_nexilisClient.getClientAPI().getCurrentMessage());
}
