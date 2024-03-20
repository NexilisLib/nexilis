#include <cstdint>
#include <nexilis/log.hh>
#include <nexilis/packet.hh>
#include <nexilis/json.hh>

namespace nexilis
{

std::vector<uint8_t> Packet::Get::clientId(ClientAPI& api)
{
    size_t clientId = api.getClientId();

    if (clientId != 0)
    {
        Log::debug("Client id when sending: ", clientId);
        std::vector<uint8_t> clientIdVector = Util::convertToByteVector(clientId);
        clientIdVector.emplace_back(0xFF);
        clientIdVector.emplace_back(0x20);
        clientIdVector.emplace_back(0x10);
        return clientIdVector;
    }
    else
    {
        Log::error("Client id is not set!");
        return {};
    }
}

std::vector<uint8_t> Packet::Info::generalInfo(ClientAPI& api)
{
    size_t clientId = api.getClientId();
    if (clientId != 0)
    {
        std::vector<uint8_t> cliendIdVector = Util::convertToByteVector(clientId);
        cliendIdVector.emplace_back(0xFF);
        cliendIdVector.emplace_back(0x40);
        cliendIdVector.emplace_back(0x10);
        return cliendIdVector;
    }
    else
    {
        Log::error("Client id is not set!");
        return {};
    }
    /*
    auto info = Json::getServerData();
    auto data = Util::convertToByteVector(info);
    return data;
    */
}

} // namespace nexilis
