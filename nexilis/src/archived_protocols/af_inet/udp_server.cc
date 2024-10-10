#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/command.hh>
#include <nexilis/message_handler.hh>

namespace nexilis::af_inet
{

UDPServer::UDPServer(const Authentication& authentication, unsigned port)
    : BaseUDPServer(port),
      Command(authentication)
{
}

UDPServer::UDPServer(UDPServer&& other)
    : BaseUDPServer(std::move(other)),
      Command(std::move(other)),
      m_receiveThread(std::move(other.m_receiveThread))
{
}

UDPServer& UDPServer::operator=(UDPServer&& other)
{
    if (this != &other)
    {
        BaseUDPServer::operator=(std::move(other));
        Command::operator=(std::move(other));
        m_receiveThread = std::move(other.m_receiveThread);
    }
    return *this;
}

UDPServer::~UDPServer()
{
    if (m_receiveThread.joinable())
    {
        m_receiveThread.join();
    }
}

void UDPServer::start()
{
    m_receiveThread = std::thread([this]()
                                  {
        BaseUDPServer::start();

        while (true)
        {
            BaseUDPServer::Message msg;

            // TODO fix
            nx_data data;

            if (BaseUDPServer::getNextMessage(msg))
            {
                auto message = getMessageHandler().readMessage(msg.address, data, msg.port, &Command::getAuthentication());

                auto sendMsg = [this, &msg](const nx_data& data)
                {
                    sendDataToClient(data, msg.clientAddr, msg.clientAddrLen);
                };

                (void)sendMsg;

                if (message.getClient())
                {
                    if (Command::read(message.getData(), *message.getClient(), *this, message.getMessageId()) != Command::Result::success)
                    {
                        Log::error("UDP server message reading error, message: ", msg.message);
                    }
                }
                else
                {
                    Log::info("Message from unauthorized client!");
                }
            }
        } });
}

void UDPServer::sendDataToClient(const nx_data& data, const sockaddr* clientAddr, socklen_t clientAddrLen)
{
    sendto(BaseUDPServer::m_serverSocket, data.data(), data.size(), 0, clientAddr, clientAddrLen);
}

} // namespace nexilis::af_inet
