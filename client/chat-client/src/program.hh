#ifndef CHAT_CLIENT_PROGRAM_HH
#define CHAT_CLIENT_PROGRAM_HH

#include "window.hh"
#include "menu.hh"
#include "nexilis_client.hh"
#include <boost/json/object.hpp>

class Program
{
public:
    /// Constructor.
    Program(int argc, char** argv);

    /// Destructor.
    ~Program();

    /// The update loop for the program.
    void update();

private:
    void inputHandler();
    void updateScreenSize();
    void end();
    void sendTCPMessage(const std::vector<uint8_t>& message);
    void debugObject(boost::json::object object);

private:
    /// Command line arguments argc.
    int m_argc;

    /// Command line argument argv.
    char** m_argv;

    /// Window object.
    Window m_window;

    /// Menu object.
    Menu m_menu;

    /// NexilisClient object.
    NexilisClient m_nexilisClient;

    /// Current input variable.
    int m_input;

    /// Current menu choice.
    int m_choice = 0;
};

#endif
