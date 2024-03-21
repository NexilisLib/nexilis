#include <nexilis/packet.hh>
#include <nexilis/log.hh>
#include <nexilis/common/util.hh>

namespace nexilis
{

std::vector<uint8_t> Packet::Set::clientId(size_t clientId)
{
    std::vector<uint8_t> cliendIdVector = Util::convertToByteVector(clientId);
    cliendIdVector.emplace_back(0xFF);
    cliendIdVector.emplace_back(0x20);
    cliendIdVector.emplace_back(0x10);
    return cliendIdVector;
}

std::vector<uint8_t> Packet::Info::generalInfo(size_t clientId)
{
    std::vector<uint8_t> cliendIdVector = Util::convertToByteVector(clientId);
    cliendIdVector.emplace_back(0xFF);
    cliendIdVector.emplace_back(0x40);
    cliendIdVector.emplace_back(0x10);
    return cliendIdVector;
}


} // namespace nexilis
