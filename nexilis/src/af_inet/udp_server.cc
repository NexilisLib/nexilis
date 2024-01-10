#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/command.hh>
#include <nexilis/client_storage.hh>

namespace nexilis
{

AfInetUdpServer::AfInetUdpServer(unsigned port)
    : BaseUdpServer(port)
{
}

AfInetUdpServer::~AfInetUdpServer()
{
}

void AfInetUdpServer::start()
{
    BaseUdpServer::start();

    while (true)
    {
        BaseUdpServer::Message msg;

        if (BaseUdpServer::getNextMessage(msg))
        {
            Log::info("Received message: ", msg.message, " from ", msg.m_address);

            Client client(msg.m_address);
            if (!ClientStorage::contains(client))
            {
                ClientStorage::add(std::move(client));
            }

            if (!Command::read(msg.message.c_str(), msg.message.size(), client, *this, true))
            {
                Log::error("UDP server message reading error, message: ", msg.message);
            }
        }
    }
}

} // namespace nexilis
