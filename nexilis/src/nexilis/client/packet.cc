#include <nexilis/client/packet.hh>
#include <nexilis/command_type.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/movement_type.hh>
#include <nexilis/room_command_type.hh>
#include <nexilis/types/vector2.hh>
#include <nexilis/util.hh>

namespace nexilis::client
{

ClientAPI* _Packet::m_clientApi = nullptr;

void _Packet::_initialize(ClientAPI& clientAPI)
{
    m_clientApi = &clientAPI;
}

void _Packet::emplace(nx_data& originalData, const nx_data& newData)
{
    originalData.reserve(originalData.size() + newData.size());
    std::copy(newData.begin(), newData.end(), std::back_inserter(originalData));
}

nx_data _Packet::clientIdentification()
{
    if (!m_clientApi)
    {
        Log::error("Packet has not initialized ClientAPI");
        return {};
    }

    uint64_t clientId = m_clientApi->getClientId();
    if (clientId == 0)
    {
        Log::error("Error creating new message id");
        return {};
    }

    auto clientIdVector = Util::convertToByteVector(clientId);

    assert(!clientIdVector.empty());
    assert(clientIdVector.size() == 8);
    assert(Util::convertToType<uint64_t>(clientIdVector) != 0);

    auto messageIdVector = Util::convertToByteVector(m_clientApi->getNewMessageId());
    assert(messageIdVector.size() == 8);

    clientIdVector.reserve(clientIdVector.size() + messageIdVector.size());
    std::copy(messageIdVector.begin(), messageIdVector.end(), std::back_inserter(clientIdVector));

    return clientIdVector;
}

nx_data ClientImpl::set_general_clientId(uint64_t newId)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::setting));
    id.emplace_back(0);
    id.emplace_back(0);

    auto id_vector = Util::convertToByteVector(newId);
    id.reserve(id.size() + id_vector.size());
    std::copy(id_vector.begin(), id_vector.end(), std::back_inserter(id));
    return id;
}

// Set: General
nx_data ClientImpl::set_general_username(const std::string& name)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::setting));
    id.emplace_back(0);
    id.emplace_back(1);

    auto nameVector = Util::convertToByteVector(name.c_str(), name.size());
    _Packet::emplace(id, nameVector);
    return id;
}

// Set: Protocol::BoostTCP
nx_data set_protocol_boosttcp_port(uint16_t port)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::setting));
    id.emplace_back(1);
    id.emplace_back(0);

    auto portVector = Util::convertToByteVector(port);
    _Packet::emplace(id, portVector);
    return id;
}

// Get: General
nx_data ClientImpl::get_general_clientId()
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::getting));
    id.emplace_back(0);
    id.emplace_back(0);
    return id;
}

nx_data ClientImpl::get_general_roomId(uint64_t client_id)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::getting));
    id.emplace_back(0);
    id.emplace_back(2);

    auto clientBytes = Util::convertToByteVector(client_id);
    _Packet::emplace(id, clientBytes);
    return id;
}

// Get: Info
nx_data ClientImpl::get_info_general()
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::getting));
    id.emplace_back(1);
    id.emplace_back(0);
    return id;
}

nx_data ClientImpl::get_info_clients()
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::getting));
    id.emplace_back(1);
    id.emplace_back(1);
    return id;
}

nx_data ClientImpl::get_info_rooms()
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::getting));
    id.emplace_back(1);
    id.emplace_back(2);
    return id;
}

// Helper lambda to emplace any number of serializable arguments
static auto emplaceAll = [](nx_data& dest, const auto&... args)
{
    (_Packet::emplace(dest, Util::convertToByteVector(args)), ...);
};

// Room::Management
nx_data ClientImpl::room_management_join(uint64_t roomId)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::management));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Management::join));
    emplaceAll(id, roomId);
    return id;
}

nx_data ClientImpl::room_management_leave()
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::management));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Management::leave));
    return id;
}

nx_data ClientImpl::room_management_create(RoomData::Context context, const std::string& roomName)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::management));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Management::create));
    id.emplace_back(static_cast<uint8_t>(context));
    emplaceAll(id, roomName);
    return id;
}

// Room::Communicate
nx_data ClientImpl::room_communicate_broadcast(const std::string& message)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::communication));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Communication::broadcast));
    emplaceAll(id, message);
    return id;
}

nx_data ClientImpl::room_communicate_othercast(const std::string& message)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::communication));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Communication::othercast));
    emplaceAll(id, message);
    return id;
}

nx_data ClientImpl::room_communicate_unicast(uint64_t userId, const std::string& message)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::communication));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Communication::unicast));
    emplaceAll(id, userId, message);
    return id;
}

// Room::Player2D
nx_data ClientImpl::room_player2d_position(Vector2f position)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player2D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::PlayerType::position));
    emplaceAll(id, position);
    return id;
}

nx_data ClientImpl::room_player2d_dimension(Vector2f dimensions)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player2D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::PlayerType::dimension));
    emplaceAll(id, dimensions);
    return id;
}

nx_data ClientImpl::room_player2d_movement(Vector2f movement, float deltatime)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player2D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::PlayerType::movement));
    emplaceAll(id, movement, deltatime);
    return id;
}

// Room::Object2D
nx_data ClientImpl::room_object2d_create(Vector2f position, Vector2f dimensions, const std::string& filePath)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object2D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::ObjectType::create));
    emplaceAll(id, position, dimensions, filePath);
    return id;
}

nx_data ClientImpl::room_object2d_destroy(uint64_t objectId)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object2D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::ObjectType::destroy));
    emplaceAll(id, objectId);
    return id;
}

nx_data ClientImpl::room_object2d_move(uint64_t objectId, Vector2f newPosition)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object2D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::ObjectType::move));
    emplaceAll(id, objectId, newPosition);
    return id;
}

nx_data ClientImpl::room_object2d_createMoving(Vector2f startingPosition, Vector2f dimensions,
                                               Vector2f movement, float deltaTime,
                                               MovementType movementType, const std::string& filepath)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object2D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::ObjectType::createMoving));
    emplaceAll(id, startingPosition, dimensions, movement, deltaTime, movementType, filepath);
    return id;
}

// Room::Player3D
nx_data ClientImpl::room_player3d_position(Vector3f position)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player3D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::PlayerType::position));
    emplaceAll(id, position);
    return id;
}

nx_data ClientImpl::room_player3d_dimension(Vector3f dimensions)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player3D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::PlayerType::dimension));
    emplaceAll(id, dimensions);
    return id;
}

nx_data ClientImpl::room_player3d_movement(Vector3f movement, float deltatime)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player3D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::PlayerType::movement));
    emplaceAll(id, movement, deltatime);
    return id;
}

nx_data ClientImpl::room_object3d_create(Vector3f position, Vector3f dimensions, const std::string& filePath)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object3D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::ObjectType::create));
    emplaceAll(id, position, dimensions, filePath);
    return id;
}

nx_data ClientImpl::room_object3d_destroy(uint64_t objectId)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object3D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::ObjectType::destroy));
    emplaceAll(id, objectId);
    return id;
}

nx_data ClientImpl::room_object3d_move(uint64_t objectId, Vector3f newPosition)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object3D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::ObjectType::move));
    emplaceAll(id, objectId, newPosition);
    return id;
}

nx_data ClientImpl::room_object3d_createmoving(Vector3f startingPosition, Vector3f dimensions,
                                               Vector3f movement, float deltaTime,
                                               MovementType movementType, const std::string& filepath)
{
    auto id = _Packet::clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::object3D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::ObjectType::createMoving));
    emplaceAll(id, startingPosition, dimensions, movement, deltaTime, movementType, filepath);
    return id;
}

} // namespace nexilis::client
