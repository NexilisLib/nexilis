#ifndef CHAT_CLIENT_PROGRAM_HH
#define CHAT_CLIENT_PROGRAM_HH

#include "menu.hh"
#include "nexilis_client.hh"
#include "window.hh"
#include "chat.hh"
#include "program_state.hh"

#include <nexilis/cmd_line_options.hh>

class Program
{
public:
    /// Constructor.
    Program(const nexilis::CmdLineOptions& opts);

    /// Destructor.
    ~Program();

    // Start running the program.
    void start();

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

private:
    /// Command line options given to the program.
    nexilis::CmdLineOptions m_options;

    /// Command line option for "-name", expecting string value.
    std::string m_optionUserName;

    /// Program state.
    State m_state = State::menu;

    /// Window object.
    Window m_window;

    /// Menu object.
    Menu m_menu;

    /// NexilisClient object.
    NexilisClient m_nexilisClient;

    /// Function for sending TCP messages using nexilis.
    std::function<void(const std::vector<uint8_t>&)> m_sendTCPMessage;
    std::function<void(const std::vector<uint8_t>&, const std::function<void()>&)> m_sendTCPMessageWithCallback;

    /// Chat object.
    Chat m_chat;

    /// Current input variable.
    int m_input;

    /// Current menu choice.
    int m_menuChoice = 0;

    /// Chat room choice.
    int m_roomChoice = 0;

    /// Should the room data to be updated?
    bool updateRooms = true;
};

#endif
