#ifndef NEXILIS_UNIX_SOCKET_CLIENT_HH
#define NEXILIS_UNIX_SOCKET_CLIENT_HH

#include <nexilis/protocol.hh>
#include <nexilis/common/util.hh>
#include <nexilis/client_api/client_api.hh>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <iostream>
#include <cstring>

namespace nexilis::af_unix
{

class UnixSocketClient : Protocol
{
public:
    UnixSocketClient(ClientAPI& api) :
        Protocol(),
        m_api(api),
        m_socketPath(m_api.getUnixSocketPath())
    {
        createSocket();
        connectToServer();
        sendMessage("moiiak");
    }

    void sendMessage(const std::string& message)
    {
        sendto(m_clientSocket, message.c_str(), message.length(), 0,
               reinterpret_cast<const struct sockaddr*>(&m_serverAddr), sizeof(m_serverAddr));
    }

    std::string receiveMessage()
    {
        char buffer[1024];
        ssize_t bytesRead = recv(m_clientSocket, buffer, sizeof(buffer), 0);

        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            return std::string(buffer);
        }
        return "";
    }

    void start() override
    {
        while (true)
        {
            auto data = receiveMessage();
            std::cout << "data: " << data << std::endl;

            auto message = Util::convertToByteVector(data.c_str(), data.size());

            m_api.readMessage(message);
        }
    }

    void stop() override
    {
    }

    Protocol::Type getType() override
    {
        return Protocol::Type::UnixSocket;
    }

private:
    void createSocket()
    {
        m_clientSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
        if (m_clientSocket == -1) {
            std::cerr << "Error creating client socket" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

    void connectToServer()
    {
        memset(&m_serverAddr, 0, sizeof(m_serverAddr));
        m_serverAddr.sun_family = AF_UNIX;
        strncpy(m_serverAddr.sun_path, m_socketPath.c_str(), sizeof(m_serverAddr.sun_path) - 1);

        // Note: In a real-world scenario, error handling after connect should be more robust.
        if (connect(m_clientSocket, reinterpret_cast<const struct sockaddr*>(&m_serverAddr), sizeof(m_serverAddr)) == -1)
        {
            std::cerr << "Error connecting to server" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }

private:
    ClientAPI& m_api;

private:
    std::string m_socketPath;
    int m_clientSocket;
    struct sockaddr_un m_serverAddr;

};

}

#endif
