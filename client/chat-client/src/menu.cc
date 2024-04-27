#include "menu.hh"

#include <ncurses.h>

void Menu::update(WINDOW* window, int& highlight, State state)
{
    switch (state)
    {
        case State::menu:
        {
            showMenu(window, highlight);
            break;
        }
        // The infopage shall be inside the menu.
        case State::infopage:
        {
            showInfo(window);
            break;
        }
        // This does not concern menu, do nothing.
        case State::chat:
        {
            break;
        }
    }
    wrefresh(window);
}

void Menu::showMenu(WINDOW* window, int& highlight)
{
    /*
     * TODO
    int halfY = grid.y / 3;
    int halfX = grid.x / 2 - 2;
    */
    int halfY = 10;
    int halfX = 40;

    for (auto& f : m_menuFields)
    {
        if (highlight == f.first)
        {
            wattron(window, A_REVERSE);
            mvwprintw(window, halfY, halfX, "%s", f.second.c_str());
            wattroff(window, A_REVERSE);
        }
        else
        {
            mvwprintw(window, halfY, halfX, "%s", f.second.c_str());
        }
        halfY++;
    }
}

void Menu::showInfo(WINDOW* window)
{
    /*
     * TODO
     */
    int halfX = 10;
    int y = 30;

    for (size_t i = 0; i < m_infoTexts.size(); i++)
    {
        mvwprintw(window, y, halfX, "%s", m_infoTexts[i].c_str());
        y += 2;
    }
}
