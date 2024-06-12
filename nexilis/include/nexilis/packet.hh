#ifndef NEXILIS_PACKET_HH
#define NEXILIS_PACKET_HH

#include <nexilis/client_api.hh>

namespace nexilis
{

class Packet
{
public:
    // Internal initilization function.
    static void _initialize(ClientAPI& clientApi);

    class Get
    {
    public:
        // The server sends the client identification to server.
        // This is mandatory packet to establish client connection.
        static std::vector<uint8_t> clientId();
    };

    class Set
    {
    public:
        static std::vector<uint8_t> userName(const std::string& name);
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
        static std::vector<uint8_t> roomMessage(const std::string& message);
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

    static ClientAPI* m_clientApi;
};

} // namespace nexilis

#endif
