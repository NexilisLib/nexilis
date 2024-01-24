#include <nexilis/af_inet/tcp_server.hh>
#include <nexilis/server_manager.hh>
#include <nexilis/log.hh>

#include <unistd.h>
#include <arpa/inet.h>

#include <cstring>

namespace nexilis::af_inet
{

TCPServer::TCPServer(int serverPort)
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_serverSocket == -1)
    {
        Log::critical("TCPServer: Error creating socket");
    }

    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;
    m_serverAddr.sin_port = htons(serverPort);

    if (bind(m_serverSocket, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == -1)
    {
        Log::critical("TCPServer: Error binding socket");
    }

    startListening();
}

TCPServer::~TCPServer()
{
    close(m_serverSocket);
}

bool TCPServer::startListening()
{
    return listen(m_serverSocket, ServerManager::getMaxAmountOfClients()) != 1;
}

TCPServer::Client TCPServer::acceptClient()
{
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(m_serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (clientSocket == -1)
    {
        Log::error("TCPServer: Error accepting client");
        return {};
    }

    Log::info("Client connected: ", inet_ntoa(clientAddr.sin_addr), ":", ntohs(clientAddr.sin_port));

    return Client
    {
        inet_ntoa(clientAddr.sin_addr),
        ntohs(clientAddr.sin_port),
        clientSocket
    };
}

bool TCPServer::sendToClient(int clientSocket, const char* data, size_t dataSize) 
{
    return write(clientSocket, data, dataSize) == dataSize;
}

void TCPServer::start()
{
    while (true)
    {
        char buffer[1024];
        auto client = acceptClient();

        ssize_t bytesRead = read(client.socket, buffer, sizeof(buffer));

        buffer[bytesRead] = '\0';
        std::string receivedData(buffer);

        if (bytesRead > 0)
        {
            auto message = m_messageHandler.readMessage(client.address, receivedData, client.port, Command::getAuthentication());

            auto sendMsg = [this, &client, &buffer](const std::vector<uint8_t> data)
            {
                sendToClient(client.socket, buffer, sizeof(buffer));
            };

            if (message.client)
            {
                if (!Command::read(message.message, *message.client, *this, sendMsg))
                {
                    Log::error("TCPServer: message reading error, message: ", receivedData);
                }
            }
            else
            {
                Log::error("TCPServer: Message from unauthorized client");
            }
        }
    }
}

}