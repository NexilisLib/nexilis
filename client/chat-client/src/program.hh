#ifndef CHAT_CLIENT_PROGRAM_HH
#define CHAT_CLIENT_PROGRAM_HH

#include "menu.hh"
#include "nexilis_client.hh"
#include "window.hh"

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
    /// Stop running the program. Called in the destructor.
    void end();

    void inputHandler();
    void updateScreenSize();
    //void sendTCPMessage(const std::vector<uint8_t>& message);
    void debugObject(boost::json::object object);
    void readMessage(boost::json::object object);
    void parseMessage(boost::json::object object);

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

    /// The newest message from the server.
    boost::json::object m_currentMessage;
};

#endif
