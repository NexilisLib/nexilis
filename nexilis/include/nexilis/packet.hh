#ifndef NEXILIS_PACKET_HH
#define NEXILIS_PACKET_HH

#include <cstdint>
#include <vector>

namespace nexilis
{

class Packet
{
public:
    class Set
    {
    public:
        // The server sends the client identification to server.
        // This is mandatory packet to establish client connection.
        static std::vector<uint8_t> clientId(size_t clientId);
    };

    class Info
    {
    public:
        static std::vector<uint8_t> generalInfo(size_t clientId);
    };

};

} // namespace nexilis

#endif
