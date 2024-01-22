#ifndef NEXILIS_COMMAND_HH
#define NEXILIS_COMMAND_HH

#include <nexilis/command_type.hh>
#include <nexilis/client.hh>
#include <nexilis/log.hh>
#include <nexilis/authentication.hh>
#include <nexilis/protocol.hh>

#include <cstdint>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>

namespace nexilis
{

/// This class is internal server side client command reader.
class Command
{
public:
    /// Read the command from client.
    /// \param command The vector of bytes that is the command.
    /// \param client The client that sent the message.
    /// \param protocol The protocol that was used in receiving the message.
    /// \param sendMessageToClient Callback function to send message back to the client.
    /// \return True if the reading of the command is succesfull.
    static bool read(const std::vector<uint8_t>& command, Client& client, Protocol& protocol, const std::function<void(const std::vector<uint8_t>&)> sendMessageToClient);

    /// Read the command from client.
    /// \param command_data The data for the command
    /// \param lenght The command lenght in bytes.
    /// \param client The client that sent the message.
    /// \param protocol The protocol that was used in receiving the message.
    /// \param sendMessageToClient Callback function to send message back to the client.
    /// \return True if the reading of the command is succesfull.
    static bool read(const char* command_data, size_t lenght, Client& client, Protocol& protocol, const std::function<void(const std::vector<uint8_t>&)> sendMessageToClient);

    /// Give server authentication details via Authentication object.
    /// /// \param authentication The object than contains authentication rules.
    static void setAuthentication(Authentication& authentication)
    {
        m_authentication = &authentication;
    }

    /// Get authentication details.
    static Authentication* getAuthentication()
    {
        return m_authentication;
    }

    /// Helper functions.

    /// Create IPv4 address from IPV4 data.
    /// \param address The address data.
    /// \return string Created IPV4 string.
    std::string createIPv4Address(const std::vector<uint8_t>& address);

    static std::vector<uint8_t> createVectorFromCommandPtr(const char* command_data, size_t lenght);

    static std::vector<uint8_t> removeAmountOfBytesFromVector(const std::vector<uint8_t>& original, uint8_t amount);

    static unsigned short convertToUnsignedShort(const std::vector<uint8_t>& bytes);
    static std::string convertToString(const std::vector<uint8_t>& bytes);

private:
    static Authentication* m_authentication;
};

} // namespace nexilis

#endif
