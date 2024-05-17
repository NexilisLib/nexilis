#include "vim.hh"
#include "debug.hh"

#include <cctype>
#include <curses.h>
#include <iterator>
#include <ncurses.h>

#include <vector>

void useVimMode(State& programState, Window& window)
{
    useVimMode(27, programState, window);
}

void useVimMode(int trigger, State& programState, Window& window)
{
    if (trigger == 27)
    {
        char nextKey = -1;
        int ch;
        std::vector<char> command;
        bool commandReady = false;

        while (!commandReady)
        {
            ch = wgetch(window.getWindow());
            nextKey = static_cast<char>(tolower(ch));

            if (nextKey != -1)
            {
                // If user entered ":".
                if (nextKey == 58)
                {
                    mvprintw(window.getWinSize().second - 1, 0, ":");
                    refresh();

                    while (true)
                    {
                        ch = wgetch(window.getWindow());
                        int thirdKey = static_cast<char>(tolower(ch));
                        command.emplace_back(thirdKey);
                        mvprintw(window.getWinSize().second - 1, static_cast<int>(command.size()) + 1, "%c", thirdKey);
                        refresh();

                        if (thirdKey == 10)
                        {
                            commandReady = true;
                            debug("Escaped vim command");
                            break;
                        }
                    }
                }
            }

        }

        // Check for ":q" or ":x".
        if ((command[0] == 'q' && command[1] == 10) || (command[0] == 'x' && command[1] == 10))
        {
            switch (programState)
            {
                case State::chat:
                {
                    programState = State::rooms;
                    break;
                }
                case State::menu:
                {
                    // TODO
                    //end();
                    break;
                }
                case State::infopage:
                {
                    programState = State::menu;
                    break;
                }
                case State::rooms:
                {
                    programState = State::menu;
                    break;
                }
            }
        }
    }
}


/*
void Program::useVim(int input)
{
    // Currently only reading after ":".
    if (input == 58)
    {
        mvprintw(m_window.getWinSize().second - 1, 0, ":");
        refresh();

        // Loop to read input until a valid key is pressed
        char nextKey;
        std::vector<char> keys;
        while (nextKey != 10)
        {
            nextKey = static_cast<char>(tolower(wgetch(m_window.getWindow())));
            if (nextKey != -1)
            {
                mvprintw(m_window.getWinSize().second - 1, static_cast<int>(keys.size()) + 1, "%c", nextKey);
                refresh();
                keys.emplace_back(nextKey);
            }
        }

        applyVim(keys, m_state);
    }
}

void Program::applyVim(std::vector<char> command, State currentState)
{
    if ((command[0] == 'q' && command[1] == 10) || (command[0] == 'x' && command[1] == 10))
    {
        switch (currentState)
        {
            case State::chat:
            {
                updateState(State::rooms);
                break;
            }
            case State::menu:
            {
                end();
                break;
            }
            case State::infopage:
            {
                updateState(State::menu);
                break;
            }
            case State::rooms:
            {
                updateState(State::menu);
                break;
            }
        }
    }
}
*/
