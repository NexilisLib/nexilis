#include "chat.hh"

#include <curses.h>
#include <ncurses.h>

Chat::Chat(Menu::State* state, nexilis::ClientAPI* clientApi) :
    m_state(state),
    m_clientApi(clientApi)
{
}

void Chat::update(Window& window, int& hightlight)
{
    switch (*m_state)
    {
        case Menu::State::menu:
        case Menu::State::infopage:
            break;

        case Menu::State::chat:
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

    if (newRooms != m_rooms)
    {
        m_rooms = newRooms;
    }
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
                mvwprintw(window.getWindow(), startY + static_cast<int>(i), startX, "%s", m_rooms[i].getName().c_str());
                wattroff(window.getWindow(), A_REVERSE);
            }
            else
            {
                mvwprintw(window.getWindow(), startY + static_cast<int>(i), startX, "%s", m_rooms[i].getName().c_str());
            }
        }
    }
    else
    {
        mvwprintw(window.getWindow(), startY + 10, startX + 10, "No rooms to show");
    }
}
