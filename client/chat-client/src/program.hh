#ifndef CHAT_CLIENT_PROGRAM_HH
#define CHAT_CLIENT_PROGRAM_HH

#include "window.hh"
#include "menu.hh"

class Program
{
public:
    /// Constructor.
    Program(int argc, char** argv);

    /// The update loop for the program.
    void update();

private:
    void inputHandler();
    void updateScreenSize();

private:
    /// Command line arguments argc.
    int m_argc;

    /// Command line argument argv.
    char** m_argv;

    /// Window object.
    Window m_window;

    /// Menu object.
    Menu m_menu;

    /// Current input variable.
    int m_input;

    /// Current menu choice.
    int m_choice = 0;
};

#endif
