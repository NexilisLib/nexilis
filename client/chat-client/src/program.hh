#ifndef CHAT_CLIENT_PROGRAM_HH
#define CHAT_CLIENT_PROGRAM_HH

#include "menu.hh"
#include "nexilis_client.hh"
#include "window.hh"
#include "chat.hh"
#include "program_state.hh"

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

    void inputHandler(Window& window);
    void updateScreenSize(Window& window);

    void debugObject(boost::json::object object);
    void readMessage(boost::json::object object);
    void parseMessage(boost::json::object object);

    // Message sending functions.
    void sendTCPMessage(const std::vector<uint8_t>& message);
    void sendUDPMessage(const std::vector<uint8_t>& message);

    /// Vim mode simply built into this function.
    /// Blocking operation if the current input is ":".
    void useVim(int input);

    /// Applying the ":q", or ":x" command, so we are switching the program state.
    void applyVim(std::vector<char> command, State state);

    /// Changing the value of the m_state member.
    /// \param state The new state of the m_state member variable.
    void updateState(State state);
private:

    /// Command line arguments argc.
    int m_argc;

    /// Command line argument argv.
    char** m_argv;

    /// Program state.
    State m_state = State::menu;

    /// Window object.
    Window m_window;

    /// Menu object.
    Menu m_menu;

    /// NexilisClient object.
    NexilisClient m_nexilisClient;

    /// Chat object.
    Chat m_chat;

    /// Current input variable.
    int m_input;

    /// Current menu choice.
    int m_menuChoice = 0;

    /// Chat room choice.
    int m_roomChoice = 0;

    /// The newest message from the server.
    boost::json::object m_currentMessage;

    /// Should the room data to be updated?
    bool updateRooms = true;
};

#endif
