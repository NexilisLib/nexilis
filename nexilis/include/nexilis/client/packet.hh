#ifndef NEXILIS_PACKET_HH
#define NEXILIS_PACKET_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/types/vector2.hh>
#include <nexilis/nexilis_macros.hh>
#include <nexilis/room_data.hh>
#include <nexilis/command_type.hh>
#include <nexilis/room_command_type.hh>

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

                emplace(id, Util::convertToByteVector(position));
                emplace(id, Util::convertToByteVector(dimensions));
                emplace(id, Util::convertToByteVector(filePath));
                return id;
            }

            static nx_data move(uint64_t objectId, VectorType newPosition)
            {
                auto id = clientIdentification();
                id.emplace_back(static_cast<uint8_t>(CommandType::room));
                id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object2D));
                id.emplace_back(static_cast<uint8_t>(RoomCommandType::Object2D::move));

                emplace(id, Util::convertToByteVector(objectId));
                emplace(id, Util::convertToByteVector(newPosition));
                return id;
            }
        };

        class Object2D : public Object<Vector2f>
        {
        };

        class Object3D : public Object<Vector3>
        {
        };

        class Management
        {
        public:
            static nx_data join(uint64_t roomId);
            static nx_data leave();
            static nx_data create(RoomData::Context context, const std::string& roomName);
        };
    };

    // Internal initilization function.
    static void _initialize(ClientAPI& clientApi);

private:
    static void emplace(nx_data& originalData, const nx_data& newData);
    static nx_data clientIdentification();
    static ClientAPI* m_clientApi;
};

} // namespace nexilis::client

#endif
