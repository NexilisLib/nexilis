#ifndef NEXILIS_LOGGER_FILEHANDLER_HH
#define NEXILIS_LOGGER_FILEHANDLER_HH

#include <nexilis/logger/base_handler.hh>

#include <fstream>

namespace nexilis::logger
{

class FileHandler : public BaseHandler
{
public:
    /// Constructor.
    /// \param filename The file where the messages will be written.
    FileHandler(const std::string& filename)
        : ofs(filename, std::ios::app)
    {
    }

    /// Write messages to the given file.
    /// \param data The data of the message.
    void emit(const LogLevel& /*logLevel*/, const std::string& data) override
    {
        ofs << data << std::endl;
    }

    // Overloading the equality operator.
    bool operator==(const BaseHandler& other) const override
    {
        return this == &other;
    }
private:
    std::ofstream ofs;
};

} // namespace nexilis

#endif
