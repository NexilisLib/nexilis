#ifndef NEXILIS_LOGGABLE_HH
#define NEXILIS_LOGGABLE_HH

#include <nexilis/log.hh>

namespace nexilis
{

class Loggable
{
public:
    /// Constructor, the class name we are logging from.
    Loggable(const std::string& name, const char* file);

    /// Move constructor.
    Loggable(Loggable&& other);

    /// Move assignment operator.
    Loggable& operator=(Loggable&& other);

    /// Deleted copy constructor.
    Loggable(const Loggable&) = delete;

    /// Deleted copy assignment operator.
    Loggable& operator=(const Loggable&) = delete;

    /// Logging functionality.
    void debug(const std::string& message){}

    void info(const std::string& message);
    void infoExtra(const std::string& message, const int line = __LINE__);

    /// Getters.

    /// Get the associated name.
    std::string getName() const
    {
        return m_name;
    }

    /// Get the file name.
    std::string getFile() const
    {
        return m_file;
    }

private:
    std::string createShortMessage(const std::string& logtext);
    std::string createLongMessage(const std::string& logText, const int line);
    void printFromLogger(logger::LogLevel logLevel, const std::string& message);

private:
    std::string m_name;
    std::string m_file;
};

} // namespace nexilis

#endif
