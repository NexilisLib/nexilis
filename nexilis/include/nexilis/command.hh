#ifndef NEXILIS_COMMAND_HH
#define NEXILIS_COMMAND_HH

#include <nexilis/boost/boost_io_context.hh>
#include <nexilis/command_type.hh>
#include <nexilis/connection.hh>

#include <cstddef>
#include <string>
#include <vector>

namespace nexilis
{

class Command
{
public:
    /// Create a new command.
    /// \param mainCommand The main command given to the server.
    /// \param subCommand The subCommand aka the value, or parameter given for the mainCommand.
    /// \return std::vector<unsigned char> The vector of bytes in the created command.
    static std::vector<unsigned char> create(MainCommand mainCommand, unsigned char subCommand = 0x10);

    /// Same function as before but we use the underlying type.
    /// \param mainCommand The main command given to the server.
    /// \param subCommand The subCommand or "value" the value for the main command.
    /// \return std::vector<unsigned char> The vector of bytes in the created command.
    static std::vector<unsigned char> create(unsigned char mainCommand, unsigned char subCommand = 0x10);

    /// Read the command from client.
    /// \param The vector of bytes that is the command.
    /// \return True if the reading of the command is succesfull.
    static bool read(const std::vector<unsigned char>& command, Connection& connection);

    /// Read the command from client.
    /// \param command_data The data for the command
    /// \param lenght The command lenght in bytes.
    static bool read(const char* command_data, size_t lenght, Connection& connection);

private:
    /// Create IPv4 address from IPV4 data.
    /// \param address The address data.
    /// \return string Created IPV4 string.
    std::string createIPv4Address(const std::vector<unsigned char>& address);
};

} // namespace nexilis

#endif
