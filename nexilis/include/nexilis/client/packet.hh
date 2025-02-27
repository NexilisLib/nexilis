#ifndef NEXILIS_PACKET_HH
#define NEXILIS_PACKET_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/command_type.hh>
#include <nexilis/movement_type.hh>
#include <nexilis/nexilis_constants.hh>
#include <nexilis/room_command_type.hh>
#include <nexilis/room_data.hh>
#include <nexilis/types/vector2.hh>

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

        template <typename VectorType>
        class Object
        {
        public:
            static nx_data create(VectorType position, VectorType dimensions, const std::string& filePath)
            {
                auto id = clientIdentification();
                id.emplace_back(static_cast<uint8_t>(CommandType::room));
                id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object2D));
                id.emplace_back(static_cast<uint8_t>(RoomCommandType::Object2D::create));

                emplaceAll(id, position, dimensions, filePath);
                return id;
            }

            static nx_data destroy(uint64_t objectId)
            {
                auto id = clientIdentification();
                id.emplace_back(static_cast<uint8_t>(CommandType::room));
                id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object2D));
                id.emplace_back(static_cast<uint8_t>(RoomCommandType::Object2D::destroy));

                emplaceAll(id, objectId);
                return id;
            }

            static nx_data move(uint64_t objectId, VectorType newPosition)
            {
                auto id = clientIdentification();
                id.emplace_back(static_cast<uint8_t>(CommandType::room));
                id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object2D));
                id.emplace_back(static_cast<uint8_t>(RoomCommandType::Object2D::move));

                emplaceAll(id, objectId, newPosition);
                return id;
            }

            static nx_data createMoving(VectorType startingPosition, VectorType dimensions, VectorType movement,
                                        float deltaTime, MovementType movementType, const std::string& filepath)
            {
                auto id = clientIdentification();
                id.emplace_back(static_cast<uint8_t>(CommandType::room));
                id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object2D));
                id.emplace_back(static_cast<uint8_t>(RoomCommandType::Object2D::createMoving));

                emplaceAll(id, startingPosition, dimensions, movement, deltaTime, movementType, filepath);
                return id;
            }
        };

        class Object2D : public Object<Vector2f>
        {
        };

        class Object3D : public Object<Vector3f>
        {
        };

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
