#ifndef NEXILIS_COMMAND_HH
#define NEXILIS_COMMAND_HH

#include <nexilis/boost/boost_io_context.hh>
#include <nexilis/command_type.hh>
#include <nexilis/connection.hh>
#include <nexilis/protocol.hh>
#include <nexilis/log.hh>

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
    /// \param connection The connection that sent the message.
    /// \param protocol The protocol that was used in the sending of the message.
    /// \return True if the reading of the command is succesfull.
    static bool read(const std::vector<uint8_t>& command, Connection& connection, Protocol& protocol);

    /// Read the command from client.
    /// \param command_data The data for the command
    /// \param lenght The command lenght in bytes.
    /// \param connection The connection that sent the message.
    static bool read(const char* command_data, size_t lenght, Connection& connection, Protocol& protocol);

private:
    /// Create IPv4 address from IPV4 data.
    /// \param address The address data.
    /// \return string Created IPV4 string.
    std::string createIPv4Address(const std::vector<uint8_t>& address);

    // This is most likely temporary function until I'll know how to make the interface smarter.
    static unsigned short convertToUnsignedShort(const std::vector<uint8_t>& bytes)
    {
        if (bytes.size() < sizeof(unsigned short))
        {
            Log::error("Port conversion failed");
        }

        unsigned short value = 0;

        for (size_t i = 0; i < sizeof(unsigned short); ++i)
        {
            value |= static_cast<unsigned short>(bytes[i] << (8 * i));
        }

        return value;
    }

    // This feels so wrong.
    static std::vector<uint8_t> createVectorWithoutHeaderBytes(const std::vector<uint8_t>& original)
    {
        // Return empty vector if the original vector has less than two elements.
        if (original.size() < 2)
        {
            return {};
        }

        std::vector<uint8_t> modified(original.begin() + 2, original.end());

        return modified;
    }
};

} // namespace nexilis

#endif
