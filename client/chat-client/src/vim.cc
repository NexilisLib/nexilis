#include "vim.hh"
#include "debug.hh"
#include "nexilis/packet.hh"

#include <cstdint>
#include <ncurses.h>

#include <vector>

enum class VimFeature
{
    escape,
    no_feature
};

struct VimCommand
{
    VimFeature feature;
    std::vector<int> data;
};

VimCommand createCommand(Window& window)
{
    std::vector<int> command;

    while (true)
    {
        int ch = wgetch(window.getWindow());
        if (ch != -1 && ch != 27)
        {
            // If user entered ":".
            if (ch == 58)
            {
                mvprintw(window.getWinSize().second - 1, 0, ":");
                refresh();

                // User input after ":".
                while (true)
                {
                    int commandCh = wgetch(window.getWindow());

                    if (commandCh != -1 && commandCh != 27)
                    {
                        command.emplace_back(commandCh);

                        mvprintw(window.getWinSize().second - 1, static_cast<int>(command.size()), "%c", static_cast<char>(commandCh));
                        refresh();

                        // User entered space, so command is ready.
                        if (commandCh == 10)
                        {
                            return VimCommand{ VimFeature::escape, command};
                        }
                    }
                    else if (commandCh == 27)
                    {
                        return VimCommand{VimFeature::no_feature, command};
                    }
                }
            }
        }
        else if (ch == 27)
        {
            return VimCommand{VimFeature::no_feature, command};
        }
    }
    return VimCommand{ VimFeature::no_feature, command};
}

void useVimMode(State& programState, Window& window, const std::function<void(const std::vector<uint8_t>&)>& sendTCPMessage)
{
    useVimMode(27, programState, window, sendTCPMessage);
}

void useVimMode(int trigger, State& programState, Window& window, const std::function<void(const std::vector<uint8_t>&)>& sendTCPMessage)
{
    // Esc-key press
    if (trigger == 27)
    {
        auto vimCommand = createCommand(window);

        if (vimCommand.feature == VimFeature::escape)
        {
            // Check for "q" or ":x".
            if ((vimCommand.data[0] == 113 && vimCommand.data[1] == 10) || (vimCommand.data[0] == 120 && vimCommand.data[1] == 10))
            {
                switch (programState)
                {
                    case State::chat:
                    {
                        programState = State::rooms;
                        sendTCPMessage(nexilis::Packet::Room::leave());
                        break;
                    }

                    // Force close application in menu.
                    case State::menu:
                    {
                        endwin();
                        exit(0);
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
}

