#include "udp_server.hh"

int main()
{
    boost::asio::io_context io_context;

    short udpPort = 1234;

    UDPServer udpServer(io_context, udpPort);

    io_context.run();

    return 0;
}
