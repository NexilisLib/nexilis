#include <nexilis/message_handler.hh>
#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/command.hh>
#include <nexilis/client_storage.hh>

namespace nexilis::af_inet
{

void UDPServer::start()
{
    m_receiveThread = std::thread([this]()
    {
        BaseUDPServer::start();

        while (true)
        {
            BaseUDPServer::Message msg;

            if (BaseUDPServer::getNextMessage(msg))
            {
                auto message = getMessageHandler().readMessage(msg.address, msg.message, msg.port, Command::getAuthentication());

                auto sendMsg = [this, &msg](const std::vector<uint8_t>& data)
                {
                    sendDataToClient(data, msg.clientAddr, msg.clientAddrLen);
                };

                if (message.getClient())
                {
                    if (!Command::read(message.getData(), *message.getClient(), *this, sendMsg))
                    {
                        Log::error("UDP server message reading error, message: ", msg.message);
                    }
                }
                else
                {
                    Log::info("Message from unauthorized client!");
                }
            }
        }
    });
}

void UDPServer::sendDataToClient(const std::vector<uint8_t>& data, const sockaddr* clientAddr, socklen_t clientAddrLen)
{
    sendto(BaseUDPServer::m_serverSocket, data.data(), data.size(), 0, clientAddr, clientAddrLen);
}

} // namespace nexilis
