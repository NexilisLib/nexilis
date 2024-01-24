#ifndef NEXILIS_BASE_UDP_SERVER_HH
#define NEXILIS_BASE_UDP_SERVER_HH

#include <nexilis/protocol.hh>
#include <nexilis/ports.hh>

#include <sys/socket.h>

#include <atomic>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace nexilis::af_inet
{

class BaseUDPServer : public Protocol
{
public:
    struct Message
    {
        const char* address;
        std::string message;
        uint16_t port;
        const struct sockaddr* clientAddr;
        socklen_t clientAddrLen;
    };

    BaseUDPServer(unsigned port = static_cast<unsigned>(Port::UDP));

    virtual ~BaseUDPServer();

    /// Start listening to incoming messages.
    void start() override;

    /// Stop the server.
    void stop() override;

    /// Retrieve message from the queue (if available).
    bool getNextMessage(Message& msg);

protected:
    int m_serverSocket;

private:
    std::atomic<bool> m_running = false;
    std::thread m_recvThread;
    std::mutex m_mtx;
    std::queue<Message> m_messageQueue;
    std::condition_variable m_condition;

    // Thread function to handle incoming messages.
    void receiverThread();
};

}

#endif
