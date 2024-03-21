#ifndef NEXILIS_PACKET_HH
#define NEXILIS_PACKET_HH

#include <cstdint>
#include <cstddef>
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
        static std::vector<uint8_t> generalInfo();
    };

    static size_t m_clientId;
};

} // namespace nexilis

#endif
