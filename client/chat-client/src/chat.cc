#include "chat.hh"

#include <ncurses.h>
#include <sstream>

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

    for (auto r = m_rooms.begin(); r != m_rooms.end(); r++)
    {
        auto clients = r->getClients();

        if (clients.empty())
        {
            mvwprintw(window.getWindow(), 0, 0, "NO CLIENTS IN ROOMS");
        }
        else
        {
            mvwprintw(window.getWindow(), 0, 0, "SOME CLIENTS IN ROOMS");
        }
    }


    if (m_rooms.empty() && newRooms.empty())
    {
        mvwprintw(window.getWindow(), 10, 10, "Nothing to show");
    }
    else if (m_clientApi->clientInRoom())
    {
        mvwprintw(window.getWindow(), 10, 10, "Client already in a room");
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
    std::string data;

    std::stringstream ss;
    ss << room.getName() << " ";

    auto clients = room.getClients();
    for (auto c = clients.begin(); c != clients.end(); c++)
    {
        ss << c->getId() << " ";
    }
    return ss.str();
}
