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
    auto msg = BaseUdpServer::receiveMessage();

    // Implicit conversion from const char* -> string?
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

} // namespace nexilis
