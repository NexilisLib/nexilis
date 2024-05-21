#include "window.hh"

#include <ncurses.h>

#include <iostream>

Window::Window()
{
    /// Start curses mode.
    initscr();

    if (has_colors() == FALSE)
    {
        std::cout << "Your terminal does not support colors!" << std::endl;
        endwin();
        exit(1);
    }
    start_color();

    // Handle input correctly.
    cbreak();
    noecho();

    // Remove cursor.
    curs_set(0);

    /// Create a new window.
    m_window = newwin(0, 0, 0, 0);

    // Set the window to non-blocking mode.
    nodelay(m_window, TRUE);

    // Get the size of the window.
    int sx, sy;
    getmaxyx(m_window, sy, sx);
    m_size = std::pair(sx, sy);

    // Enable keypad mode, removes ESC-key.
    keypad(m_window, TRUE);
}

Window::~Window()
{
    // End curses mode.
    endwin();
}

WINDOW* Window::getWindow() const
{
    return m_window;
}

std::pair<int, int> Window::getWinSize() const
{
    return m_size;
}

void Window::updateScreenSize(const std::pair<int, int>& newSize)
{
    m_size = newSize;
}
