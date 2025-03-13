#include <nexilis/client/packet.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/types/vector2.hh>
#include <nexilis/util.hh>

namespace nexilis::client
{

ClientAPI* Packet::m_clientApi = nullptr;

nx_data Packet::Set::username(const std::string& name)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::setting));
    id.emplace_back(1);

    auto nameVector = Util::convertToByteVector(name.c_str(), name.size());
    id.reserve(id.size() + nameVector.size());
    std::copy(nameVector.begin(), nameVector.end(), std::back_inserter(id));
    return id;
}

nx_data Packet::Get::clientId()
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::getting));
    id.emplace_back(0);
    return id;
}

nx_data Packet::Info::general()
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::info));
    id.emplace_back(0);
    return id;
}

nx_data Packet::Info::clients()
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::info));
    id.emplace_back(1);
    return id;
}

nx_data Packet::Info::rooms()
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::info));
    id.emplace_back(2);
    return id;
}

/**
 *  2:1      Player2D
 *  2:1:0    Set position; Vec2f position
 *  2:1:1    Set dimensions; Vec2f dimensions
 *  2:1:2    2D movement vector; Vec2f movement
 */
nx_data Packet::Room::Player2D::position(Vector2f position)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player2D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Player2D::position));

    emplaceAll(id, position);
    return id;
}

nx_data Packet::Room::Player2D::dimensions(Vector2f dimensions)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player2D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Player2D::dimensions));

    emplaceAll(id, dimensions);
    return id;
}

nx_data Packet::Room::Player2D::movement(Vector2f movement, float deltaTime)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::player2D));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Player2D::movement));

    emplaceAll(id, movement, deltaTime);
    return id;
}

/**
 *  2:0      Management
 *  2:0:0    Join room; uint64_t roomId
 *  2:0:1    Leave room; void
 *  2:0:2    Create room; string roomName
 */
nx_data Packet::Room::Management::join(uint64_t roomId)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::management));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Management::join));

    emplaceAll(id, roomId);
    return id;
}

nx_data Packet::Room::Management::leave()
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::management));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Management::leave));
    return id;
}

nx_data Packet::Room::Management::create(RoomData::Context context, const std::string& roomName)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::management));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Management::create));
    id.emplace_back(static_cast<uint8_t>(context));

    id.reserve(id.size() + roomName.size());
    std::transform(roomName.begin(), roomName.end(), std::back_inserter(id), [](char c)
                   { return static_cast<uint8_t>(c); });

    return id;
}

nx_data Packet::Room::Communicate::broadcast(const std::string& message)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::communication));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Communication::broadcast));
    emplaceAll(id, message);
    return id;
}

nx_data Packet::Room::Communicate::othercast(const std::string& message)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::communication));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Communication::othercast));
    emplaceAll(id, message);
    return id;
}

nx_data Packet::Room::Communicate::unicast(uint64_t userId, const std::string& message)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Root::communication));
    id.emplace_back(static_cast<uint8_t>(RoomCommandType::Communication::unicast));
    emplaceAll(id, userId, message);
    return id;
}

void Packet::_initialize(ClientAPI& clientAPI)
{
    m_clientApi = &clientAPI;
}

void Packet::emplace(nx_data& originalData, const nx_data& newData)
{
    originalData.reserve(originalData.size() + newData.size());
    std::copy(newData.begin(), newData.end(), std::back_inserter(originalData));
}

nx_data Packet::clientIdentification()
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

} // namespace nexilis::client
