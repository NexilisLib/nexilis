#ifndef NEXILIS_LOGGABLE_HH
#define NEXILIS_LOGGABLE_HH

#include <string>

namespace nexilis
{

/// Class that gives better logging by inheriting it.
class Loggable
{
public:
    /// Constructor, the class name we are logging from.
    Loggable(const std::string& name);

    /// Move constructor.
    Loggable(Loggable&& other);

    /// Move assignment operator.
    Loggable& operator=(Loggable&& other);

    /// Deleted copy constructor.
    Loggable(const Loggable&) = delete;

    /// Deleted copy assignment operator.
    Loggable& operator=(const Loggable&) = delete;

public:
    /// Logging functionality.

    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);

public:
    /// Getters.

    /// Get the associated name.
    std::string getName()
    {
        return m_name;
    }

    /// Get the option for adding line number.
    bool getAddLineNumber()
    {
        return m_addLineNumber;
    }

    /// Get the option for adding colon after the log.
    bool getAddColon()
    {
        return m_addColon;
    }

public:
    /// Setters.

    /// Set the associated name.
    void setName(const std::string& name)
    {
        m_name = name;
    }

    void setLineNumber(bool addLineNumber)
    {
        m_addLineNumber = addLineNumber;
    }

    void setAddColon(bool addColon)
    {
        m_addColon = addColon;
    }
private:

    std::string createMessage(const std::string& text);

    std::string m_name;
    bool m_addLineNumber = false;
    bool m_addColon = true;
};

}

#endif
