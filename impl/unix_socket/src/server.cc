#ifndef IMPL_UNIX_SOCKET_SERVER_HH
#define IMPL_UNIX_SOCKET_SERVER_HH

#include <nexilis/protocol_manager.hh>

#include <thread>

class Server
{
public:
    Server()
    {
        nexilis::ProtocolManager manager;

        auto server = manager.addConnection<nexilis::UnixUDPServer>();

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
