#include "server.hh"

int main()
{
    boost::asio::io_context io_context;

    unsigned short udpPort = 1999;

    // Initialize both UDP and TCP server.

    // Server udpServer(io_context, udpPort);

    io_context.run();

    return 0;
}
