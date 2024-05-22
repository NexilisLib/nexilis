#include "chat.hh"
#include "nexilis/client_api.hh"

#include <nexilis/packet.hh>

#include <ncurses.h>

#include <sstream>

Chat::Chat(nexilis::ClientAPI* clientApi, const std::function<void(const std::vector<uint8_t>&)>& sendTCP) :
    m_clientApi(clientApi),
    m_sendTCP(sendTCP)
{
}

void Chat::update(Window& window, int& hightlight, State& state)
{
    switch (state)
    {
        case State::menu:
        case State::infopage:
            break;

        case State::chat:
        {
            werase(window.getWindow());
            updateChat(window);
            break;
        }
        case State::rooms:
        {
            werase(window.getWindow());
            showRooms(window, hightlight, state);
            break;
        }
    }
}

void Chat::showRooms(Window& window, int& highlight, State& state)
{
    // Kinda sus in a loop honestly.
    auto newRooms = m_clientApi->getActiveRooms();

    // Update new rooms.
    if (m_rooms != newRooms)
    {
        m_rooms = newRooms;
    }

    if (m_rooms.empty() && newRooms.empty())
    {
        mvwprintw(window.getWindow(), 0, 0, "Nothing to show");
    }
    else if (m_clientApi->clientInRoom())
    {
        // If a client is in a room, the program state is assumed to be chat.
        state = State::chat;
    }
    else
    {
        state = State::rooms;

        int startX = 0;
        int startY = 0;
        if (!m_rooms.empty())
        {
            // Calculate position for room display
            startY = ((window.getWinSize().second - static_cast<int>(m_rooms.size())) / 2);
            startX = (window.getWinSize().first - static_cast<int>(m_rooms[0].getName().length())) / 2;

            for (size_t i = 0; i < m_rooms.size(); i++)
            {
                if (highlight == static_cast<int>(i))
                {
                    wattron(window.getWindow(), A_REVERSE);
                    mvwprintw(window.getWindow(), startY + static_cast<int>(i), startX, "%s", roomData(m_rooms[i]).c_str());
                    wrefresh(window.getWindow());
                    wattroff(window.getWindow(), A_REVERSE);
                }
                else
                {
                    mvwprintw(window.getWindow(), startY + static_cast<int>(i), startX, "%s", roomData(m_rooms[i]).c_str());
                    wrefresh(window.getWindow());
                }
            }
        }
        else
        {
            mvwprintw(window.getWindow(), startY + 10, startX + 10, "No rooms to show");
        }
    }
}

void Chat::updateChat(Window& window)
{
    std::stringstream ss;
    ss << "In room: " << m_clientApi->clientRoomId();
    mvwprintw(window.getWindow(), 0, 0, "%s", ss.str().c_str());

    // Prompt message.
    int startY = window.getWinSize().second / 2 + window.getWinSize().second / 3;
    int startX = window.getWinSize().first / 2;
    mvwprintw(window.getWindow(), startY, startX, "Enter chat message:");

    // Create a buffer to the store the input.
    std::vector<char> buffer;

    int ch;
    char nextKey = -1;
    int index = 0;

    while (nextKey != 10)
    {
        ch = wgetch(window.getWindow());
        nextKey = static_cast<char>(tolower(ch));

        if (nextKey != -1)
        {
            if (ch == KEY_BACKSPACE)
            {
                if (index > 0)
                {
                    mvwprintw(window.getWindow(), startY + 1, startX + 2 + index - 1, " ");
                    wrefresh(window.getWindow());
                    index--;
                    buffer.pop_back();
                }
            }
            else
            {
                // Display the character
                mvwprintw(window.getWindow(), startY + 1, startX + index, "%c", nextKey);
                wrefresh(window.getWindow());
                buffer.emplace_back(nextKey);
                index++;
            }
        }
        // No input, update existing messages.
        else
        {
            // Kinda sus in a loop honestly.
            auto newRooms = m_clientApi->getActiveRooms();

            // Update new rooms.
            if (m_rooms != newRooms)
            {
                m_rooms = newRooms;
            }

            uint64_t clientId = m_clientApi->getClientId();

            assert(clientId);
            assert(!m_rooms.empty());

            /// Get the room that client is currently in.
            nexilis::ClientAPI::Room clientRoom;
            for (auto& room : m_rooms)
            {
                for (auto& client : room.getClients())
                {
                    if (client.getId() == clientId)
                    {
                        clientRoom = room;
                    }
                }
            }
            assert(clientRoom != m_clientApi->getDefaultRoom());

            std::vector<nexilis::ClientAPI::Room::Communication> messages;

            bool foundMessages = false;
            for (auto r = m_rooms.begin(); r != m_rooms.end(); r++)
            {
                /// List room messages.
                if (*r == clientRoom)
                {
                    messages = r->getMessages();
                    foundMessages = true;
                }
            }

            // This really means that we correctly find the room where client currently is.
            // The messages could still be empty.
            assert(foundMessages);

            if (!messages.empty())
            {
                // Calculate position for room display
                int startY = (window.getWinSize().second - static_cast<int>(messages.size())) / 2;
                int startX = (window.getWinSize().first - static_cast<int>(messages[0].getPayload().length())) / 2;
                int index = 0;

                for (auto b = messages.begin(); b != messages.end(); b++)
                {
                    std::stringstream ss;
                    ss << "(" << b->getClientId() << ") " << b->getPayload();
                    mvwprintw(window.getWindow(), startY + index, startX, "%s", ss.str().c_str());
                    wrefresh(window.getWindow());
                    index++;
                }
            }
            else
            {
                int startX = 0;
                int startY = 0;
                mvwprintw(window.getWindow(), startY, startX, "No messages in this room");
                wrefresh(window.getWindow());
            }
        }
    }

    std::string userString(buffer.begin(), buffer.end());
    if (!userString.empty())
    {
        m_sendTCP(nexilis::Packet::Communicate::roomMessage(userString));
    }
    else
    {
        mvprintw(40, 40, "Empty input");
        wrefresh(window.getWindow());
    }
}

std::string Chat::roomData(const nexilis::ClientAPI::Room& room)
{
    std::stringstream ss;
    ss << room.getName() << " ";

    auto clients = room.getClients();
    for (auto c = clients.begin(); c != clients.end(); c++)
    {
        ss << c->getName() << "(" << c->getId() << ")";

        if (c != clients.end() - 1)
        {
            ss << ", ";
        }
    }
    return ss.str();
}
