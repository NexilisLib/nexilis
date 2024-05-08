#include "chat.hh"

#include <ncurses.h>
#include <sstream>

Chat::Chat(nexilis::ClientAPI* clientApi) :
    m_clientApi(clientApi)
{
}

void Chat::update(Window& window, int& hightlight, State state)
{
    switch (state)
    {
        case State::menu:
        case State::infopage:
            break;

        case State::chat:
        {
            werase(window.getWindow());
            showRooms(window, hightlight);
            wrefresh(window.getWindow());
            break;
        }
    }
}

void Chat::showRooms(Window& window, int& highlight)
{
    // Kinda sus in a loop honestly.
    auto newRooms = m_clientApi->getActiveRooms();

    if (m_rooms != newRooms)
    {
        m_rooms = newRooms;
    }

    if (m_rooms.empty() && newRooms.empty())
    {
        mvwprintw(window.getWindow(), 0, 0, "Nothing to show");
    }
    else if (m_clientApi->clientInRoom())
    {
        std::stringstream ss;
        ss << "In room: " << m_clientApi->clientRoomId();
        mvwprintw(window.getWindow(), 0, 0, "%s", ss.str().c_str());

        int textHeight = 5;
        int textWidth = 30;
        int startY = window.getWinSize().second / 2;
        int startX = window.getWinSize().first / 2;

        // Prompt message
        mvwprintw(window.getWindow(), startY, startX, "Enter chat message:");

        // Create a buffer to the store the input.
        char buffer[100];
        memset(buffer, 0, sizeof(buffer));

        // Move cursor to the input box.
        mvwprintw(window.getWindow(), 1, 1, "> ");
        wrefresh(window.getWindow());

        // Get input from the user
        wgetstr(window.getWindow(), buffer);

        // Print the input
        mvprintw(startY + 2, startX, "You entered: %s", buffer);
        refresh();

        // Wait for user input to exit
        getch();
    }
    else
    {
        int startX = 0;
        int startY = 0;
        if (!m_rooms.empty())
        {
            // Calculate starting position for room display
            startY = (window.getWinSize().second - static_cast<int>(m_rooms.size())) / 2;
            startX = (window.getWinSize().first - static_cast<int>(m_rooms[0].getName().length())) / 2;

            for (size_t i = 0; i < m_rooms.size(); i++)
            {
                if (highlight == static_cast<int>(i))
                {
                    wattron(window.getWindow(), A_REVERSE);
                    mvwprintw(window.getWindow(), startY + static_cast<int>(i), startX, "%s", roomData(m_rooms[i]).c_str());
                    wattroff(window.getWindow(), A_REVERSE);
                }
                else
                {
                    mvwprintw(window.getWindow(), startY + static_cast<int>(i), startX, "%s", roomData(m_rooms[i]).c_str());
                }
            }
        }
        else
        {
            mvwprintw(window.getWindow(), startY + 10, startX + 10, "No rooms to show");
        }
    }
}

std::string Chat::roomData(const nexilis::ClientAPI::Room& room)
{
    std::stringstream ss;
    ss << room.getName() << " ";

    auto clients = room.getClients();
    for (auto c = clients.begin(); c != clients.end(); c++)
    {
        ss << c->getName() << "(" << c->getId() << ")";

        if (c != clients.end() - 1)
        {
            ss << ", ";
        }
    }
    return ss.str();
}
