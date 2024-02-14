#include <nexilis/packet.hh>

namespace nexilis
{

std::vector<uint8_t> Packet::Get::clientId(ClientAPI& api)
{
    std::cout << "CLIENTID WHEN SENDING" << api.getClientId() << std::endl;
    std::vector<uint8_t> clientIdVector = Util::convertToByteVector(api.getClientId());
    clientIdVector.push_back(0xFF);
    clientIdVector.push_back(0x20);
    clientIdVector.push_back(0x10);
    return clientIdVector;
}


}