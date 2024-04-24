#include "chat.hh"

#include <ncurses.h>

Chat::Chat(Menu::State* state, nexilis::ClientAPI* clientApi) :
    m_state(state),
    m_clientApi(clientApi)
{
}

void Chat::update(WINDOW* window, int& hightlight)
{
    switch (*m_state)
    {
        case Menu::State::menu:
        case Menu::State::infopage:
            break;

        case Menu::State::chat:
        {
            showRooms(window, hightlight);
            break;
        }
    }
    wrefresh(window);
}

void Chat::showRooms(WINDOW* window, int& highlight)
{
    auto newRooms = m_clientApi->getActiveRooms();

    if (newRooms != m_rooms)
    {
        m_rooms = newRooms;
    }
    int halfY = 10;
    int halfX = 40;

    for (size_t i = 0; i < m_rooms.size(); i++)
    {
        if (highlight == static_cast<int>(i))
        {
            wattron(window, A_REVERSE);
            mvwprintw(window, halfY, halfX, "%s", m_rooms[i].getName().c_str());
            wattroff(window, A_REVERSE);
        }
        else
        {
            mvwprintw(window, halfY, halfX, "%s", m_rooms[i].getName().c_str());
        }
        halfY++;
    }
}
