#ifndef NEXILIS_PACKET_HH
#define NEXILIS_PACKET_HH

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace nexilis
{

class Packet
{
public:
    // Internal initilization function.
    static void _initialize(size_t clientId);

    class Get
    {
    public:
        // The server sends the client identification to server.
        // This is mandatory packet to establish client connection.
        static std::vector<uint8_t> clientId();
    };

    class Info
    {
    public:
        static std::vector<uint8_t> general();
        static std::vector<uint8_t> clients();
        static std::vector<uint8_t> rooms();
    };

    class Communicate
    {
    public:
        static std::vector<uint8_t> broadcast(const std::string& message);
        static std::vector<uint8_t> multicast(const std::string& message);
    };

    class Room
    {
    public:
        static std::vector<uint8_t> join(uint64_t roomId);
        static std::vector<uint8_t> leave();
        static std::vector<uint8_t> create(const std::string& roomName);
    };

private:
    static std::vector<uint8_t> clientIdentification();

    static size_t m_clientId;
};

} // namespace nexilis

#endif
