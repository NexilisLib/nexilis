#include "program.hh"

Program::Program(int argc, char** argv) :
    m_argc(argc),
    m_argv(argv),
    m_window()
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
}

void Program::updateScreenSize()
{
    // TODO
}

void Program::update()
{
    inputHandler();
}
