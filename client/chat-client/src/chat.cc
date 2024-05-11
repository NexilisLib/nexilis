#include "chat.hh"

#include <ncurses.h>

Chat::Chat(nexilis::ClientAPI* clientApi) :
    m_clientApi(clientApi)
{
}

void Chat::update(Window& window, int& hightlight, State state)
{
    switch (state)
    {
        case State::menu:
        case State::infopage:
            break;

        case State::chat:
        {
            werase(window.getWindow());
            showRooms(window, hightlight);
            wrefresh(window.getWindow());
            break;
        }
    }
}

void Chat::showRooms(Window& window, int& highlight)
{
    // Kinda sus in a loop honestly.
    auto newRooms = m_clientApi->getActiveRooms();

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
        std::stringstream ss;
        ss << "In room: " << m_clientApi->clientRoomId();
        mvwprintw(window.getWindow(), 0, 0, "%s", ss.str().c_str());

        int startY = window.getWinSize().second / 2;
        int startX = window.getWinSize().first / 2;

        // Prompt message
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
                    buffer.push_back(nextKey);
                    index++;
                }
            }

        }
        std::string userString(buffer.begin(), buffer.end());

        if (!userString.empty())
        {
            mvprintw(30, 30, "You entered: %s", userString.c_str());
            wrefresh(window.getWindow());

            std::stringstream ss;
            ss << "NOT EMPTY: " << userString;
            debug(ss.str());
        }
        else
        {
            mvprintw(40, 40, "Empty input");
            wrefresh(window.getWindow());
        }
    }
    else
    {
        int startX = 0;
        int startY = 0;
        if (!m_rooms.empty())
        {
            // Calculate starting position for room display
            startY = (window.getWinSize().second - static_cast<int>(m_rooms.size())) / 2;
            startX = (window.getWinSize().first - static_cast<int>(m_rooms[0].getName().length())) / 2;

            for (size_t i = 0; i < m_rooms.size(); i++)
            {
                if (highlight == static_cast<int>(i))
                {
                    wattron(window.getWindow(), A_REVERSE);
                    mvwprintw(window.getWindow(), startY + static_cast<int>(i), startX, "%s", roomData(m_rooms[i]).c_str());
                    wattroff(window.getWindow(), A_REVERSE);
                }
                else
                {
                    mvwprintw(window.getWindow(), startY + static_cast<int>(i), startX, "%s", roomData(m_rooms[i]).c_str());
                }
            }
        }
        else
        {
            mvwprintw(window.getWindow(), startY + 10, startX + 10, "No rooms to show");
        }
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
