#ifndef NEXILIS_PACKET_HH
#define NEXILIS_PACKET_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/types/vector2.hh>
#include <nexilis/nexilis_macros.hh>

namespace nexilis::client
{

class Packet
{
public:
    class Get
    {
    public:
        // The server sends the client identification to server.
        // This is mandatory packet to establish client connection.
        static nx_data clientId();
    };

    class Set
    {
    public:
        static nx_data username(const std::string& name);
    };

    class Info
    {
    public:
        static nx_data general();
        static nx_data clients();
        static nx_data rooms();
    };

    class Room
    {
    public:
        class Player2D
        {
        public:
            static nx_data position(Vector2f position);
            static nx_data dimensions(Vector2f dimensions);
            static nx_data movement(Vector2f movement, float deltaTime);
        };

        class Management
        {
        public:
            static nx_data join(uint64_t roomId);
            static nx_data leave();
            static nx_data create(const std::string& roomName);
        };
    };

    // Internal initilization function.
    static void _initialize(ClientAPI& clientApi);

private:
    static nx_data clientIdentification();
    static ClientAPI* m_clientApi;
};

} // namespace nexilis::client

#endif
