#ifndef CHAT_CLIENT_WINDOW_HH
#define CHAT_CLIENT_WINDOW_HH

#include <ncurses.h>

#include <utility>

class Window
{
public:
    /// Constructor.
    Window();

    /// Destructor.
    ~Window();

    /// Get the ncurses window.
    WINDOW* getWindow() const;
private:
    /// Underlying ncurses window.
    WINDOW* m_window;

    /// Size of the window.
    std::pair<int, int> m_size;
};

#endif
