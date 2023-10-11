#ifndef IMPL_UNIX_SOCKET_SERVER_HH
#define IMPL_UNIX_SOCKET_SERVER_HH

#include <nexilis/udp/unix_udp_server.hh>

#include <thread>

class Server
{
public:
    Server()
    {
        nexilis::UnixUDPServer server;

        std::thread serverThread([&server]()
        {
            while(true)
            {
                server.receiveMessage();
            }
        });

        serverThread.join();
    }
};

#endif



int main(int argc, char** argv)
{
    Server s;
    return 0;
}
