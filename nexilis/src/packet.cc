#include <cstdint>
#include <nexilis/common/util.hh>
#include <nexilis/log.hh>
#include <nexilis/packet.hh>

namespace nexilis
{

size_t Packet::m_clientId = 0;

std::vector<uint8_t> Packet::Get::clientId()
{
    if (m_clientId == 0)
    {
        Log::error("Client has not been initialized");
        return {};
    }
    std::vector<uint8_t> cliendIdVector = Util::convertToByteVector(m_clientId);
    cliendIdVector.emplace_back(0xFF);
    cliendIdVector.emplace_back(1);
    cliendIdVector.emplace_back(0);
    return cliendIdVector;
}

std::vector<uint8_t> Packet::Info::general()
{
    if (m_clientId == 0)
    {
        Log::error("Client has not been intialized");
        return {};
    }
    std::vector<uint8_t> cliendIdVector = Util::convertToByteVector(m_clientId);
    cliendIdVector.emplace_back(0xFF);
    cliendIdVector.emplace_back(3);
    cliendIdVector.emplace_back(0);
    return cliendIdVector;
}

std::vector<uint8_t> Packet::Info::clients()
{
    if (m_clientId == 0)
    {
        Log::error("Client has not been intialized");
        return {};
    }
    std::vector<uint8_t> cliendIdVector = Util::convertToByteVector(m_clientId);
    cliendIdVector.emplace_back(0xFF);
    cliendIdVector.emplace_back(3);
    cliendIdVector.emplace_back(1);
    return cliendIdVector;
}

std::vector<uint8_t> Packet::Info::rooms()
{
    if (m_clientId == 0)
    {
        Log::error("Client has not been intialized");
        return {};
    }
    std::vector<uint8_t> cliendIdVector = Util::convertToByteVector(m_clientId);
    cliendIdVector.emplace_back(0xFF);
    cliendIdVector.emplace_back(3);
    cliendIdVector.emplace_back(2);
    return cliendIdVector;
}

std::vector<uint8_t> Packet::Room::join(uint64_t roomId)
{
    if (m_clientId == 0)
    {
        Log::error("Client has not been intialized");
        return {};
    }
    std::vector<uint8_t> clientIdVector = Util::convertToByteVector(m_clientId);
    clientIdVector.emplace_back(0xFF);
    clientIdVector.emplace_back(10);
    clientIdVector.emplace_back(0);
    auto roomIdVector = Util::convertToByteVector(roomId);

    for (const auto& elem : roomIdVector)
    {
        clientIdVector.emplace_back(elem);
    }

    return clientIdVector;
}

void Packet::_initialize(size_t clientId)
{
    m_clientId = clientId;
}

} // namespace nexilis
