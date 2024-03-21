#include "program.hh"
#include "debug.hh"
#include "nexilis_client.hh"

#include <ncurses.h>

#include <nexilis/packet.hh>
#include <nexilis/logger/file_handler.hh>

#include <cstdint>

Program::Program(int argc, char** argv) :
    m_argc(argc),
    m_argv(argv),
    m_window(),
    m_menu(),
    m_nexilisClient()
{
    m_nexilisClient.start();
}

Program::~Program()
{
    end();
}

void Program::inputHandler()
{
    m_input = wgetch(m_window.getWindow());

    switch (m_input)
    {
        case KEY_RESIZE:
        {
            updateScreenSize();
            return;
        }
    }

    /// Menu update ritual.
    if (m_menu.getState() == Menu::State::menu)
    {
        switch (tolower(m_input))
        {
            case 'j':
            case KEY_DOWN:
            {
                if (m_choice < MENU_ITEM_COUNT)
                {
                    ++m_choice;
                }
                else if (m_choice == MENU_ITEM_COUNT)
                {
                    m_choice = 0;
                }
                break;
            }
            case 'k':
            case KEY_UP:
            {
                if (m_choice > 0)
                {
                    --m_choice;
                }
                else if (m_choice == 0)
                {
                    m_choice = MENU_ITEM_COUNT;
                }
                break;
            }
        }

        // Press enter in menu to launch action.
        if (m_input == 10)
        {
            switch (m_choice)
            {
                // Chat.
                case 0:
                {
                    m_menu.changeState(Menu::State::chat);
                    wclear(m_window.getWindow());
                    break;
                }

                // Info.
                case 1:
                {
                    m_menu.changeState(Menu::State::infopage);
                    wclear(m_window.getWindow());
                    break;
                }

                // Quit.
                case 2:
                {
                    end();
                    break;
                }
            }
        }
    }

    else if (m_menu.getState() == Menu::State::chat)
    {
        if (m_input == 10)
        {
            debug("Sent message to the server asking for server data");
            sendTCPMessage(nexilis::Packet::Info::generalInfo());
        }
    }
}

void Program::updateScreenSize()
{
    // TODO
}

void Program::sendTCPMessage(const std::vector<uint8_t>& message)
{
    m_nexilisClient.getTCPClient().sendMessage(message);
}

void Program::end()
{
    endwin();
    exit(0);
}

void Program::update()
{
    inputHandler();
    m_menu.update(m_window.getWindow(), m_choice);
}
