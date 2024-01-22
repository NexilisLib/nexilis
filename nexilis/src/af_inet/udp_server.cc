#include <nexilis/message_handler.hh>
#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/command.hh>
#include <nexilis/client_storage.hh>

namespace nexilis::af_inet
{

void UDPServer::start()
{
    BaseUDPServer::start();

    while (true)
    {
        BaseUDPServer::Message msg;

        if (BaseUDPServer::getNextMessage(msg))
        {
            auto message = m_messageHandler.readMessage(msg.address, msg.message, msg.port, Command::getAuthentication());

            if (message.client)
            {
                // TODO
                if (!Command::read(message.message, *message.client, *this, [](const std::vector<uint8_t>&){}))
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
}

} // namespace nexilis
