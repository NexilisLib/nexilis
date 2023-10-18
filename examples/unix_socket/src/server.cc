#include <nexilis/protocol_manager.hh>
#include <nexilis/af_unix/unix_socket_sender.hh>
#include <nexilis/af_inet/udp_server.hh>

#include <thread>
#include <mutex>

class Server
{
public:
    Server()
    {
        std::mutex mtx;

        nexilis::ProtocolManager manager;

        auto server = manager.addProtocol<nexilis::UnixSocketServer>("/tmp/nexilis");

        std::thread serverThread([&server, &mtx]()
        {
            while (true)
            {
                std::lock_guard<std::mutex> lock(mtx);
                server.receiveMessage();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });


        nexilis::UnixSocketSender sender("/tmp/nexilis");
        std::thread senderThread([&sender]()
        {
            sender.sendMessage("hello from myself");
        });

        serverThread.join();
        senderThread.join();
    }
};

int main()
{
    Server s;
    return 0;
}
