#ifndef NEXILIS_PACKET_HH
#define NEXILIS_PACKET_HH

#include <nexilis/client_api.hh>

#include <vector>

namespace nexilis
{

class Packet
{
public:
    class Get
    {
    public:
        static std::vector<uint8_t> clientId(ClientAPI& api);
    };
};

} // namespace nexilis

#endif