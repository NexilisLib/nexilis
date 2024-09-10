#include <nexilis/packet.hh>
#include <nexilis/command_type.hh>
#include <nexilis/common/util.hh>
#include <nexilis/log.hh>

namespace nexilis
{

ClientAPI* Packet::m_clientApi = nullptr;

std::vector<uint8_t> Packet::Set::username(const std::string& name)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::setting));
    id.emplace_back(1);

    auto nameVector = Util::convertToByteVector(name.c_str(), name.size());
    for (const auto& elem : nameVector)
    {
        id.emplace_back(elem);
    }
    return id;
}

std::vector<uint8_t> Packet::Get::clientId()
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::getting));
    id.emplace_back(0);
    return id;
}

std::vector<uint8_t> Packet::Info::general()
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::info));
    id.emplace_back(0);
    return id;
}

std::vector<uint8_t> Packet::Info::clients()
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::info));
    id.emplace_back(1);
    return id;
}

std::vector<uint8_t> Packet::Info::rooms()
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::info));
    id.emplace_back(2);
    return id;
}

/**
 *  2:0      Management
 *  2:0:0    Join room; uint64_t roomId
 *  2:0:1    Leave room; void
 *  2:0:2    Create room; string roomName
 *
 *  2:1      Object2D
 *  2:1:0    Set position; Vec2f position
 *  2:1:1    Set dimensions; Vec2f dimensions
 *
 *  2:2    Communication.
 *  2:2:0
 */

std::vector<uint8_t> Packet::Room::Object2D::position(Vector2f position)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(1);
    id.emplace_back(0);

    auto positionVector = Util::convertToByteVector(position);
    for (const auto& byte : positionVector)
    {
        id.emplace_back(byte);
    }
    return id;
}

std::vector<uint8_t> Packet::Room::Object2D::dimensions(Vector2f dimensions)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(1);
    id.emplace_back(1);

    auto dimensionVector = Util::convertToByteVector(dimensions);
    for (const auto& byte : dimensionVector)
    {
        id.emplace_back(byte);
    }
    return id;
}

std::vector<uint8_t> Packet::Room::join(uint64_t roomId)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(0);
    id.emplace_back(0);

    auto roomIdVector = Util::convertToByteVector(roomId);
    for (const auto& elem : roomIdVector)
    {
        id.emplace_back(elem);
    }
    return id;
}

std::vector<uint8_t> Packet::Room::leave()
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(0);
    id.emplace_back(1);
    return id;
}

std::vector<uint8_t> Packet::Room::create(const std::string& roomName)
{
    auto id = clientIdentification();
    id.emplace_back(static_cast<uint8_t>(CommandType::room));
    id.emplace_back(0);
    id.emplace_back(2);

    for (const char r : roomName)
    {
        id.emplace_back(static_cast<uint8_t>(r));
    }
    return id;
}

void Packet::_initialize(ClientAPI& clientAPI)
{
    m_clientApi = &clientAPI;
}

std::vector<uint8_t> Packet::clientIdentification()
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
    assert(Util::convertToType<uint64_t>(clientIdVector) != 0);

    clientIdVector.emplace_back(0xFF);
    assert(clientIdVector.back() == 0xFF);

    auto messageIdVector = Util::convertToByteVector(m_clientApi->getNewMessageId());

    for (const auto& byte : messageIdVector)
    {
        clientIdVector.emplace_back(byte);
    }

    clientIdVector.emplace_back(0xFF);
    assert(clientIdVector.back() == 0xFF);

    return clientIdVector;
}

} // namespace nexilis
