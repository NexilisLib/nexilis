#ifndef CHAT_CLIENT_WINDOW_HH
#define CHAT_CLIENT_WINDOW_HH

struct _win_st;

#include <utility>

class Window
{
public:
    /// Constructor.
    Window();

    /// Destructor.
    ~Window();

    /// Get the ncurses window.
    _win_st* getWindow() const;

    /// Get the window size.
    /// \return X, Y pair of the window size.
    std::pair<int, int> getWinSize() const;

private:
    /// Underlying ncurses window.
    _win_st* m_window = nullptr;

    /// Size of the window.
    std::pair<int, int> m_size;
};

#endif
