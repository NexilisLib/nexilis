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
    cliendIdVector.emplace_back(0x20);
    cliendIdVector.emplace_back(0x10);
    return cliendIdVector;
}

std::vector<uint8_t> Packet::Info::generalInfo()
{
    if (m_clientId == 0)
    {
        Log::error("Client has not been intialized");
        return {};
    }
    std::vector<uint8_t> cliendIdVector = Util::convertToByteVector(m_clientId);
    cliendIdVector.emplace_back(0xFF);
    cliendIdVector.emplace_back(0x40);
    cliendIdVector.emplace_back(0x10);
    return cliendIdVector;
}

void Packet::_initialize(size_t clientId)
{
    m_clientId = clientId;
}

} // namespace nexilis
