#include "chat.hh"

#include <ncurses.h>

Chat::Room::Room(const std::string& name, int maxSize, uint64_t id) :
    m_name(name),
    m_maxSize(maxSize),
    m_id(id)
{
}

void Chat::addRoom(Room&& room)
{
    m_rooms.emplace_back(room);
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
