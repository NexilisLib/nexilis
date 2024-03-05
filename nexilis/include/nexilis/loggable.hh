#ifndef NEXILIS_LOGGABLE_HH
#define NEXILIS_LOGGABLE_HH

#include <nexilis/log.hh>

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

private:
    /// Internal macro API

    #define _LOGGABLE_LINE_NUMBERS(result) \
    { \
        std::stringstream ss; \
        ss << "\n@: " << __FILE__ << ":" << std::dec << __LINE__ << std::endl; \
        result = ss.str(); \
    }

    #define _LOGGABLE_LN_PREFERENCE(logger, ...) \
    { \
        std::string result; \
        result += getName(); \
        if (getAddColon()) \
        { \
            result += ": "; \
        } \
        std::string lineNumbers; \
        if (getAddLineNumber()) \
        { \
            _LOGGABLE_LINE_NUMBERS(lineNumbers); \
        } \
        logger(result, ##__VA_ARGS__, lineNumbers); \
    }

    #define _LOGGABLE_LN_ALWAYS(logger, ...) \
    { \
        std::string result; \
        result += getName(); \
        if (getAddColon()) \
        { \
            result += ": "; \
        } \
        std::string lineNumbers; \
        _LOGGABLE_LINE_NUMBERS(lineNumbers); \
        logger(result, ##__VA_ARGS__, lineNumbers); \
    }

protected:
    /// Logging API.

    #define DEBUG(...) \
        _LOGGABLE_LN_PREFERENCE(Log::debug, ##__VA_ARGS__);

    #define DEBUG_LN(...) \
        _LOGGABLE_LN_ALWAYS(Log::debug, ##__VA_ARGS__);

    #define INFO(...) \
        _LOGGABLE_LN_PREFERENCE(Log::info, ##__VA_ARGS__);

    #define INFO_LN(...) \
        _LOGGABLE_LN_ALWAYS(Log::info, ##__VA_ARGS__);

    #define WARNING(...) \
        _LOGGABLE_LN_PREFERENCE(Log::warning, ##__VA_ARGS__);

    #define WARNING_LN(...) \
        _LOGGABLE_LN_ALWAYS(Log::warning, ##__VA_ARGS__);

    #define ERROR(...) \
        _LOGGABLE_LN_PREFERENCE(Log::error, ##__VA_ARGS__);

    #define ERROR_LN(...) \
        _LOGGABLE_LN_ALWAYS(Log::error, ##__VA_ARGS__);

    #define CRITICAL(...) \
        _LOGGABLE_LN_PREFERENCE(Log::critical, ##__VA_ARGS__);

    #define CRITICAL_LN(...) \
        _LOGGABLE_LN_ALWAYS(Log::critical, ##__VA_ARGS__);

protected:
    /// Getters.

    /// Get the associated name.
    std::string getName() const
    {
        return m_name;
    }

    /// Get the option for adding line number.
    bool getAddLineNumber() const
    {
        return m_addLineNumber;
    }

    /// Get the option for adding colon after the log.
    bool getAddColon() const
    {
        return m_addColon;
    }

protected:
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

    void addLineNumbers()
    {
        m_addLineNumber = true;
    }

    void removeLineNumbers()
    {
        m_addLineNumber = false;
    }

    void setAddColon(bool addColon)
    {
        m_addColon = addColon;
    }

    void addColons()
    {
        m_addColon = true;
    }

    void removeColons()
    {
        m_addColon = false;
    }

private:
    std::string m_name;
    bool m_addLineNumber = true;
    bool m_addColon = true;
};

}

#endif
