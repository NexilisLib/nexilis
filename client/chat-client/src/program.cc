#include "program.hh"
#include "menu.hh"
#include "debug.hh"
#include <string>

Program::Program(int argc, char** argv) :
    m_argc(argc),
    m_argv(argv),
    m_window(),
    m_menu()
{
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

        // Launch actions from menu.
    }
}

void Program::updateScreenSize()
{
    // TODO
}

void Program::update()
{
    inputHandler();
    m_menu.update(m_window.getWindow(), m_choice);
}
