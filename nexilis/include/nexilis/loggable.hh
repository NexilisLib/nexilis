#ifndef NEXILIS_LOGGABLE_HH
#define NEXILIS_LOGGABLE_HH

#include <nexilis/log.hh>

namespace nexilis
{

// Forward declarations.
template <typename Derived>
class Loggable;

template <typename Derived>
class LoggableClass : public Derived
{
public:
    void print()
    {
        typename Loggable<Derived>::FileLineInfo a = Loggable<Derived>::createFileLineInfo(__FILE__, __LINE__);
        a.printInfo();
    }
};

/// Class that gives logging macros by inheriting it.
/// Works both client on client and serverside code.
/// CRTP moments.
template <typename Derived>
class Loggable
{
public:
    class FileLineInfo
    {
    public:
        FileLineInfo(const std::string& file, int line) : file(file), line(line) {}

        void printInfo() const
        {
            std::cout << "@: " << file << ":" << line << std::endl;
        }

    private:
        std::string file;
        int line;
    };

    /// Helper function for creating FileLineInfo.
    template <typename T>
    FileLineInfo createFileLineInfo(const std::string& file, int line)
    {
        return FileLineInfo(file, line);
    }

    LoggableClass<Loggable> a;

    void print()
    {
        a.print();
    }


    /// Constructor, the class name we are logging from.
    //Loggable(const std::string& name) : m_name(name) {}

    /// Move constructor.
    /*
    Loggable(Loggable<Derived>&& other) :
        m_name(std::move(other.m_name)),
        m_addLineNumber(std::move(other.m_addLineNumber)),
        m_addColon(std::move(other.m_addColon))
    {
    }

    /// Move assignment operator.
    Loggable<Derived>& operator=(Loggable<Derived>&& other)
    {
        if (this != &other)
        {
            m_name = std::move(other.m_name);
            m_addLineNumber = std::move(other.m_addLineNumber);
            m_addColon = std::move(other.m_addColon);
        }
        return *this;
    }
    */

    /// Deleted copy constructor.
    Loggable(const Loggable&) = delete;

    /// Deleted copy assignment operator.
    Loggable& operator=(const Loggable&) = delete;

private:
    /// Underscore indicating detail.
    #define _LOGGABLE_LINE_NUMBERS(result) \
    { \
        std::stringstream ss; \
        ss << "\n@: " << __FILE__ << ":" << std::dec << __LINE__ << std::endl; \
        result = ss.str(); \
    }

    /// Underscore indicating detail.
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

    /// Underscore indicating detail.
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

    // Inheriting Loggable.
    // Unmaintained insta legacy, but it should work.
protected:
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
