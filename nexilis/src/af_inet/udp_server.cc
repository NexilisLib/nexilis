#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/command.hh>
#include <nexilis/connection_storage.hh>

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

            Connection connection(msg.m_address);
            if (!ConnectionStorage::contains(connection))
            {
                ConnectionStorage::add(std::move(connection));
            }

            if (!Command::read(msg.message.c_str(), msg.message.size(), connection, *this))
            {
                Log::error("UDP server message reading error, message: ", msg.message);
            }
        }
    }
}

} // namespace nexilis
