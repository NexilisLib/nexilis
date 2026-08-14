#ifndef NEXILIS_PACKET_HH
#define NEXILIS_PACKET_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/command_packet_base.hh>
#include <nexilis/movement_type.hh>

namespace nexilis::client
{

struct ClientImpl
{
    // Set::General
    static nx_data set_general_clientId(ClientAPI& api, uint64_t newId);
    static nx_data set_general_username(ClientAPI& api, const std::string& name);

    // Get::General
    static nx_data get_general_clientId(ClientAPI& api);
    static nx_data get_general_roomId(ClientAPI& api, uint64_t client_id);

    // Get::Info
    static nx_data get_info_general(ClientAPI& api);
    static nx_data get_info_clients(ClientAPI& api);
    static nx_data get_info_rooms(ClientAPI& api);

    // Room::Management
    static nx_data room_management_join(ClientAPI& api, uint64_t roomId);
    static nx_data room_management_leave(ClientAPI& api);
    static nx_data room_management_create(ClientAPI& api, const std::string& roomName, RoomData::Context ctx = RoomData::Context::_3D);
    static nx_data room_management_remove(ClientAPI& api, uint64_t roomId);

    // Room::Communicate
    static nx_data room_communicate_broadcast(ClientAPI& api, const std::string& message);
    static nx_data room_communicate_othercast(ClientAPI& api, const std::string& message);
    static nx_data room_communicate_unicast(ClientAPI& api, uint64_t userId, const std::string& message);

    // Room::Player2D
    static nx_data room_player2d_position(ClientAPI& api, Vector2f position);

    static nx_data room_player2d_dimension(ClientAPI& api, Vector2f dimensions);

    static nx_data room_player2d_movement(ClientAPI& api, Vector2f movement, float deltatime);

    // Room::Object2D
    static nx_data room_object2d_create(ClientAPI& api, Vector2f position, Vector2f dimensions, const std::string& filePath);

    static nx_data room_object2d_destroy(ClientAPI& api, uint64_t objectId);

    static nx_data room_object2d_move(ClientAPI& api, uint64_t objectId, Vector2f newPosition);

    static nx_data room_object2d_createMoving(ClientAPI& api, Vector2f startingPosition, Vector2f dimensions,
                                              Vector2f movement, float deltaTime,
                                              MovementType movementType, const std::string& filepath);

    // Room::Player3D
    static nx_data room_player3d_position(ClientAPI& api, Vector3f position);

    static nx_data room_player3d_dimension(ClientAPI& api, Vector3f dimensions);

    static nx_data room_player3d_movement(ClientAPI& api, Vector3f movement, float deltatime);

    // Room::Object3D
    static nx_data room_object3d_create(ClientAPI& api, Vector3f position, Vector3f dimensions, const std::string& filePath);

    static nx_data room_object3d_destroy(ClientAPI& api, uint64_t objectId);

    static nx_data room_object3d_move(ClientAPI& api, uint64_t objectId, Vector3f newPosition);

    static nx_data room_object3d_createmoving(ClientAPI& api, Vector3f startingPosition, Vector3f dimensions,
                                              Vector3f movement, float deltaTime,
                                              MovementType movementType, const std::string& filepath);

    static nx_data room_gameitem_create(ClientAPI& api, Vector3f position, Vector3f dimensions,
                                        const std::string& type, const std::string& status,
                                        const std::string& filepath);
    static nx_data room_gameitem_update(ClientAPI& api, uint64_t itemId, const std::string& status);
    static nx_data room_gameitem_destroy(ClientAPI& api, uint64_t itemId);
};
using Packet = CommandPacketBase<ClientImpl>;

class _Packet
{
public:
    // Client identification and message id prefix for an outgoing packet.
    static nx_data clientIdentification(ClientAPI& api);
    static void emplace(nx_data& originalData, const nx_data& newData);

    template <typename... Args>
    static void emplaceAll(nx_data& originalData, Args&&... args)
    {
        (emplace(originalData, Util::convertToByteVector(std::forward<Args>(args))), ...);
    }
};

} // namespace nexilis::client

#endif
