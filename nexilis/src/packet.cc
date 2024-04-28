#include <nexilis/common/util.hh>
#include <nexilis/log.hh>
#include <nexilis/packet.hh>

namespace nexilis
{

uint64_t Packet::m_clientId = 0;

std::vector<uint8_t> Packet::Get::clientId()
{
    auto id = clientIdentification();
    id.emplace_back(1);
    id.emplace_back(0);
    return id;
}

std::vector<uint8_t> Packet::Info::general()
{
    auto id = clientIdentification();
    id.emplace_back(3);
    id.emplace_back(0);
    return id;
}

std::vector<uint8_t> Packet::Info::clients()
{
    auto id = clientIdentification();
    id.emplace_back(3);
    id.emplace_back(1);
    return id;
}

std::vector<uint8_t> Packet::Info::rooms()
{
    auto id = clientIdentification();
    id.emplace_back(3);
    id.emplace_back(2);
    return id;
}

std::vector<uint8_t> Packet::Room::join(uint64_t roomId)
{
    auto id = clientIdentification();
    id.emplace_back(10);
    id.emplace_back(0);

    auto roomIdVector = Util::convertToByteVector(roomId);
    for (const auto& elem : roomIdVector)
    {
        id.emplace_back(elem);
    }
    assert(id.size() == 19);
    return id;
}

std::vector<uint8_t> Packet::Room::leave()
{
    auto id = clientIdentification();
    id.emplace_back(10);
    id.emplace_back(1);
    return id;
}

std::vector<uint8_t> Packet::Room::create(const std::string& roomName)
{
    auto id = clientIdentification();
    id.emplace_back(10);
    id.emplace_back(2);

    for (const char r : roomName)
    {
        id.emplace_back(static_cast<uint8_t>(r));
    }
    return id;
}

void Packet::_initialize(uint64_t clientId)
{
    m_clientId = clientId;
}

std::vector<uint8_t> Packet::clientIdentification()
{
    if (m_clientId == 0)
    {
        Log::error("Packet has not initialized clientIdentification");
        return {};
    }

    auto clientIdVector = Util::convertToByteVector(m_clientId);

    assert(!clientIdVector.empty());
    assert(Util::convertToType<uint64_t>(clientIdVector) != 0);

    clientIdVector.emplace_back(0xFF);
    assert(clientIdVector.back() == 0xFF);

    return clientIdVector;
}

} // namespace nexilis
