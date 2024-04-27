#ifndef CHAT_CLIENT_MENU_HH
#define CHAT_CLIENT_MENU_HH

#include "program_state.hh"

/// Forward declare ncurses window.
struct _win_st;

#include <map>
#include <string>
#include <vector>

// Currently gives 2.
// Technically not correct, but we use the 0th index as well so it's fine.
#define MENU_ITEM_COUNT 2

class Menu
{
public:
    /// Constructor.
    Menu() = default;

    /// Update the menu view.
    void update(_win_st* window, int& highlight, State state);

private:
    void showMenu(_win_st* window, int& highlight);
    void showInfo(_win_st* window);

private:
    /// Choices in the menu.
    std::map<int, std::string> m_menuFields{
            {0, "Chat"},
            {1, "Info"},
            {2, "Quit"}};

    /// Stuff displayed in the infopage.
    std::vector<std::string> m_infoTexts{
            "Here we have information regarding this program",
            "Here is another line displaying information"};

};

#endif
