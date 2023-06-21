#include "udp_server.hh"

int main()
{
    boost::asio::io_context io_context;

    short udpPort = 1999;

    UDPServer udpServer(io_context, static_cast<unsigned short>(udpPort));

    io_context.run();

    return 0;
}
