#include "debug.hh"

#include <ncurses.h>

#include <iostream>

void debug(const std::string& message)
{
#ifndef NEXILIS_DEBUG
    // End ncurses temporarily to print to the console.
    endwin();

    // Print to the standard console.
    std::cout << message << std::endl;

    // Re-initialize ncurses.
    initscr();

    // Refresh the window to display changes.
    refresh();
#endif
}
