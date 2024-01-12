#ifndef NEXILIS_COMMAND_HH
#define NEXILIS_COMMAND_HH

#include <nexilis/boost/boost_io_context.hh>
#include <nexilis/command_type.hh>
#include <nexilis/client.hh>
#include <nexilis/protocol.hh>
#include <nexilis/log.hh>
#include <nexilis/authentication.hh>

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>

namespace nexilis
{

/// This class offers an interface for:
/// - Creating command vector from two header bytes.
/// - Reading said command vector, returning boolean describing
/// if command is succesfull.
class Command
{
public:
    /// Create a new command.
    /// \param mainCommand The main command given to the server.
    /// \param subCommand The subCommand aka the value, or parameter given for the mainCommand.
    /// \return std::vector<uint8_t> The vector of bytes in the created command.
    static std::vector<uint8_t> create(MainCommand mainCommand, uint8_t subCommand = 0x10);

    /// Same function as before but we use the underlying type.
    /// \param mainCommand The main command given to the server.
    /// \param subCommand The subCommand or "value" the value for the main command.
    /// \return std::vector<uint8_t> The vector of bytes in the created command.
    static std::vector<uint8_t> create(uint8_t mainCommand, uint8_t subCommand = 0x10);

    /// Read the command from client.
    /// \param command The vector of bytes that is the command.
    /// \param client The client that sent the message.
    /// \param protocol The protocol that was used in the sending of the message.
    /// \param readByServer Tells if the command is read by server or not.
    /// \return True if the reading of the command is succesfull.
    static bool read(const std::vector<uint8_t>& command, Client& client, Protocol& protocol, bool readByServer);

    /// Read the command from client.
    /// \param command_data The data for the command
    /// \param lenght The command lenght in bytes.
    /// \param client The client that sent the message.
    /// \param readByServer Tells if the command is read by server or not.
    /// \return True if the reading of the command is succesfull.
    static bool read(const char* command_data, size_t lenght, Client& client, Protocol& protocol, bool readByServer);

    // Clientside access only.
    //static bool read(const std::vector<uint8_t>& command, Protocol& protocol);

    static void setAuthentication(Authentication& authentication)
    {
        m_authentication = &authentication;
    }

    static std::vector<uint8_t> createVectorFromCommandPtr(const char* command_data, size_t lenght);

    static std::vector<uint8_t> removeAmountOfBytesFromVector(const std::vector<uint8_t>& original, uint8_t amount);

private:
    static bool readServer(const std::vector<uint8_t>& command, Client& client, Protocol& protocol);
    static bool readServer(const char* command_data, size_t lenght, Client& client, Protocol& protocol);
    static bool readClient(const std::vector<uint8_t>& command, Client& client, Protocol& protocol);
    static bool readClient(const char* command_data, size_t lenght, Client& client, Protocol& protocol);

    /// Create IPv4 address from IPV4 data.
    /// \param address The address data.
    /// \return string Created IPV4 string.
    std::string createIPv4Address(const std::vector<uint8_t>& address);

    /// Helper functions
    static unsigned short convertToUnsignedShort(const std::vector<uint8_t>& bytes);
    static std::string convertToString(const std::vector<uint8_t>& bytes);

private:
    static Authentication* m_authentication;
};

} // namespace nexilis

#endif
