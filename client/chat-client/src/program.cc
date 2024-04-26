#include "program.hh"
#include "debug.hh"
#include "menu.hh"
#include "nexilis_client.hh"

#include <cctype>
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
      m_nexilisClient(),
      m_chat(&m_menu.getState(), &m_nexilisClient.getClientAPI())
{
    m_nexilisClient.start();
}

Program::~Program()
{
    end();
}

void Program::inputHandler(Window& window)
{
    m_input = wgetch(m_window.getWindow());

    switch (m_input)
    {
        case KEY_RESIZE:
        {
            updateScreenSize(window);
            break;
        }
    }

    /// Menu key input update ritual.
    switch (m_menu.getState())
    {
        case Menu::State::menu:
        {
            useVimInterface(window, tolower(m_input), State::menu);
            switch (tolower(m_input))
            {
                case 'j':
                case KEY_DOWN:
                {
                    if (m_menuChoice < MENU_ITEM_COUNT)
                    {
                        ++m_menuChoice;
                    }
                    else if (m_menuChoice == MENU_ITEM_COUNT)
                    {
                        m_menuChoice = 0;
                    }
                    break;
                }
                case 'k':
                case KEY_UP:
                {
                    if (m_menuChoice > 0)
                    {
                        --m_menuChoice;
                    }
                    else if (m_menuChoice == 0)
                    {
                        m_menuChoice = MENU_ITEM_COUNT;
                    }
                    break;
                }

                // Press enter in menu to launch action.
                case 10:
                {
                    switch (m_menuChoice)
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
            break;
        }

        case Menu::State::chat:
        {
            useVimInterface(window, tolower(m_input), State::chat);
            // Get information about the chat.
            // This is honestly pretty fucking stupid.
            if (updateRooms)
            {
                debug("Sent message to the server asking for server data");
                sendTCPMessage(nexilis::Packet::Info::rooms());

                updateRooms = false;
            }

            switch (tolower(m_input))
            {
                case 'j':
                case KEY_DOWN:
                {
                    if (m_roomChoice < m_chat.getRoomAmount())
                    {
                        ++m_roomChoice;
                    }
                    else if (m_roomChoice == m_chat.getRoomAmount())
                    {
                        m_roomChoice = 0;
                    }
                    break;
                }
                case 'k':
                case KEY_UP:
                {
                    if (m_roomChoice > 0)
                    {
                        --m_roomChoice;
                    }
                    else if (m_roomChoice == 0)
                    {
                        m_roomChoice = m_chat.getRoomAmount();
                    }
                    break;
                }
                case 10:
                {
                    debug("Pressed enter in chat mode");
                    size_t roomId = m_chat.getRoomIdByPosition(m_roomChoice);
                    std::stringstream ss;
                    ss << "Joining room: " << roomId;
                    debug(ss.str());
                    sendTCPMessage(nexilis::Packet::Room::join(roomId));
                    break;
                }

                case KEY_F(1):
                {
                    std::string newRoomName = nexilis::Util::getRandomString(5);
                    sendTCPMessage(nexilis::Packet::Room::create(newRoomName));
                    break;
                }

                case KEY_F(5):
                {
                    updateRooms = true;
                    break;
                }

                case KEY_F(6):
                {
                    debug("Pressed key down");
                    uint64_t copiedClientId = m_nexilisClient.getClientAPI().getClientId();
                    std::string date = nexilis::Util::getDateAndTime();
                    std::stringstream ss;
                    ss << "../../../logs/" << copiedClientId << ":" << date << "log.json";
                    nexilis::Json::saveToFile(m_nexilisClient.getClientAPI().getCurrentMessage(), ss.str());
                    break;
                }

                case KEY_F(7):
                {
                    debug("Pressed key up");
                    debugObject(m_nexilisClient.getClientAPI().getCurrentMessage());
                    break;
                }
            }
        }
        case Menu::State::infopage:
        {
            useVimInterface(window, tolower(m_input), State::infopage);
            break;
        }
    }
}

void Program::updateScreenSize(Window& window)
{
    int maxY, maxX;
    getmaxyx(window.getWindow(), maxY, maxX);
    auto newSize = std::make_pair(maxX, maxY);
    window.updateScreenSize(newSize);
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
        debug("There should not be any nexilis status messages in the client so something is wrong");
    }

    /// Here parse client specific messages.
}

void Program::useVimInterface(Window& window, int input, State state)
{
    // Pressed ":".
    if (input == 58)
    {
        debug("COLON PRESSED");

        mvprintw(window.getWinSize().second - 1, 0, ":");
    }
}

void Program::update()
{
    inputHandler(m_window);
    m_menu.update(m_window.getWindow(), m_menuChoice);
    m_chat.update(m_window, m_roomChoice);
    readMessage(m_nexilisClient.getClientAPI().getCurrentMessage());
}
