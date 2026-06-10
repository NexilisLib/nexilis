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
    static nx_data set_general_clientId(uint64_t newId);
    static nx_data set_general_username(const std::string& name);

    // Get::General
    static nx_data get_general_clientId();
    static nx_data get_general_roomId(uint64_t client_id);

    // Get::Info
    static nx_data get_info_general();
    static nx_data get_info_clients();
    static nx_data get_info_rooms();

    // Room::Management
    static nx_data room_management_join(uint64_t roomId);
    static nx_data room_management_leave();
    static nx_data room_management_create(const std::string& roomName, RoomData::Context ctx = RoomData::Context::_3D);
    static nx_data room_management_remove(uint64_t roomId);

    static nx_data room_communicate_broadcast(const std::string& message);
    static nx_data room_communicate_othercast(const std::string& message);
    static nx_data room_communicate_unicast(uint64_t userId, const std::string& message);

    static nx_data room_player2d_position(Vector2f position);

    static nx_data room_player2d_dimension(Vector2f dimensions);

    static nx_data room_player2d_movement(Vector2f movement, float deltatime);

    static nx_data room_object2d_create(Vector2f position, Vector2f dimensions, const std::string& filePath);

    static nx_data room_object2d_destroy(uint64_t objectId);

    static nx_data room_object2d_move(uint64_t objectId, Vector2f newPosition);

    static nx_data room_object2d_createMoving(Vector2f startingPosition, Vector2f dimensions,
                                              Vector2f movement, float deltaTime,
                                              MovementType movementType, const std::string& filepath);

    static nx_data room_player3d_position(Vector3f position);

    static nx_data room_player3d_dimension(Vector3f dimensions);

    static nx_data room_player3d_movement(Vector3f movement, float deltatime);

    static nx_data room_object3d_create(Vector3f position, Vector3f dimensions, const std::string& filePath);

    static nx_data room_object3d_destroy(uint64_t objectId);

    static nx_data room_object3d_move(uint64_t objectId, Vector3f newPosition);

    static nx_data room_object3d_createmoving(Vector3f startingPosition, Vector3f dimensions,
                                              Vector3f movement, float deltaTime,
                                              MovementType movementType, const std::string& filepath);
};
using Packet = CommandPacketBase<ClientImpl>;

class _Packet
{
public:
    // Internal initilization function.
    static void _initialize(ClientAPI& clientApi);
    static nx_data clientIdentification();
    static void emplace(nx_data& originalData, const nx_data& newData);

    template <typename... Args>
    static void emplaceAll(nx_data& originalData, Args&&... args)
    {
        (emplace(originalData, Util::convertToByteVector(std::forward<Args>(args))), ...);
    }

private:
    static ClientAPI* m_clientApi;
};

} // namespace nexilis::client

#endif
