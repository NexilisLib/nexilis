#include <nexilis/packet.hh>
#include <nexilis/log.hh>

namespace nexilis
{

std::vector<uint8_t> Packet::Get::clientId(ClientAPI& api)
{
    size_t clientId = api.getClientId();

    if (clientId != 0)
    {
        Log::info("Client id when sending: ", clientId); 
        std::vector<uint8_t> clientIdVector = Util::convertToByteVector(clientId);
        clientIdVector.push_back(0xFF);
        clientIdVector.push_back(0x20);
        clientIdVector.push_back(0x10);
        return clientIdVector;
    }
    else
    {
        Log::error("Client id is not set!");
        return {};
    }
}


}