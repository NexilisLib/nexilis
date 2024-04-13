#ifndef NEXILIS_COMMAND_HH
#define NEXILIS_COMMAND_HH

#include <nexilis/authentication.hh>
#include <nexilis/client.hh>
#include <nexilis/command_type.hh>
#include <nexilis/log.hh>
#include <nexilis/protocol.hh>
#include <nexilis/json.hh>

#include <cstddef>
#include <cstdint>
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
    /// \return True if the reading of the command is succesfull.
    static bool read(const std::vector<uint8_t>& command, Client& client, Protocol& protocol);

    /// Read the command from client.
    /// \param command_data The data for the command
    /// \param lenght The command lenght in bytes.
    /// \param client The client that sent the message.
    /// \param protocol The protocol that was used in receiving the message.
    /// \return True if the reading of the command is succesfull.
    static bool read(const char* command_data, size_t lenght, Client& client, Protocol& protocol);

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

private:
    /// Send message to every protocol that is avainable for a client;
    static void sendMessageToClient(std::vector<uint8_t> data, Client& client, Protocol& protocol);

private:
    static Authentication* m_authentication;

    /// Nexilis_status is 1, indicating internal nexilis command.
    static ::boost::json::object m_nexilisStatus;
};

} // namespace nexilis

#endif
