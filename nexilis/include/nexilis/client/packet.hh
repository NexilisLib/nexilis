#ifndef NEXILIS_PACKET_HH
#define NEXILIS_PACKET_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/command_type.hh>
#include <nexilis/movement_type.hh>
#include <nexilis/nexilis_constants.hh>
#include <nexilis/nx_data.hh>
#include <nexilis/room_command_type.hh>
#include <nexilis/room_data.hh>
#include <nexilis/types/vector2.hh>

namespace nexilis::client
{

class Packet
{
public:
    class Set
    {
    public:
        static nx_data clientId(uint64_t newId);
        static nx_data username(const std::string& name);
    };

    class Get
    {
    public:
        // The server sends the client identification to server.
        // This is mandatory packet to establish client connection.
        static nx_data clientId();
    };

    class Room
    {
    public:
        class Management
        {
        public:
            static nx_data join(uint64_t roomId);
            static nx_data leave();
            static nx_data create(RoomData::Context context, const std::string& roomName);
        };

        class Communicate
        {
        public:
            /// Send message to everyone in room context.
            static nx_data broadcast(const std::string& message);

            /// Send message to everyone except yourself in room context.
            static nx_data othercast(const std::string& message);

            /// Send message to specific user.
            static nx_data unicast(uint64_t userId, const std::string& message);
        };

        class Player2D
        {
        public:
            static nx_data position(Vector2f position);
            static nx_data dimensions(Vector2f dimensions);
            static nx_data movement(Vector2f movement, float deltatime);
        };

        class Object2D
        {
        public:
            static nx_data create(Vector2f position, Vector2f dimensions, const std::string& filePath);
            static nx_data destroy(uint64_t objectId);
            static nx_data move(uint64_t objectId, Vector2f newPosition);
            static nx_data createMoving(Vector2f startingPosition, Vector2f dimensions, Vector2f movement,
                                        float deltaTime, MovementType movementType, const std::string& filepath);
        };

        class Player3D
        {
        public:
            static nx_data position(Vector3f position);
            static nx_data dimensions(Vector3f dimensions);
            static nx_data movement(Vector3f movement, float deltatime);
        };

        class Object3D
        {
        public:
            static nx_data create(Vector3f position, Vector3f dimensions, const std::string& filePath);
            static nx_data destroy(uint64_t objectId);
            static nx_data move(uint64_t objectId, Vector3f newPosition);
            static nx_data createMoving(Vector3f startingPosition, Vector3f dimensions, Vector3f movement,
                                        float deltaTime, MovementType movementType, const std::string& filepath);
        };
    };

    class Info
    {
    public:
        static nx_data general();
        static nx_data clients();
        static nx_data rooms();
    };

    // Internal initilization function.
    static void _initialize(ClientAPI& clientApi);

private:
    static void emplace(nx_data& originalData, const nx_data& newData);

    template <typename... Args>
    static void emplaceAll(nx_data& originalData, Args&&... args)
    {
        (emplace(originalData, Util::convertToByteVector(std::forward<Args>(args))), ...);
    }

    static nx_data clientIdentification();
    static ClientAPI* m_clientApi;
};

} // namespace nexilis::client

#endif
