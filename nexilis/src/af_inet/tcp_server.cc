#include <nexilis/af_inet/tcp_server.hh>
#include <nexilis/server_manager.hh>
#include <nexilis/log.hh>

#include <unistd.h>
#include <arpa/inet.h>

#include <cstring>

namespace nexilis::af_inet
{

TCPServer::Client::Client(std::string address, uint16_t port, int socket) :
    m_address(address),
    m_port(port),
    m_socket(socket)
{
}

TCPServer::TCPServer(int serverPort)
{
    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_serverSocket == -1)
    {
        Log::critical("TCPServer: Error creating socket");
        close(m_serverSocket);
        return;
    }

    // Set SO_REUSEADDR option.
    int opt = 1;
    if (setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        Log::critical("TCPServer: Error setting SO_REUSEADDR option, reason: ", std::strerror(errno));
        close(m_serverSocket);
        return;
    }

    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;
    m_serverAddr.sin_port = htons(serverPort);

    if (bind(m_serverSocket, reinterpret_cast<sockaddr*>(&m_serverAddr), sizeof(m_serverAddr)) == -1)
    {
        Log::critical("TCPServer: Error binding socket, reason: ", std::strerror(errno));
        close(m_serverSocket);
        return;
    }

    if (!startListening())
    {
        Log::critical("TCPServer: Error listening on socket, reason: ", std::strerror(errno));
        close(m_serverSocket);
        return;
    }
    m_mutex = std::make_unique<std::mutex>();
}

TCPServer::~TCPServer()
{
    close(m_serverSocket);

    if (m_operatingThread.joinable())
    {
        m_operatingThread.join();
    }
}

TCPServer::TCPServer(TCPServer&& other) :
    Protocol(std::move(other)),
    m_serverSocket(std::move(other.m_serverSocket)),
    m_serverAddr(std::move(other.m_serverAddr)),
    m_operatingThread(std::move(other.m_operatingThread)),
    m_mutex(std::move(other.m_mutex))
{
}

TCPServer& TCPServer::operator=(TCPServer&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        m_serverSocket = std::move(other.m_serverSocket);
        m_serverAddr = std::move(other.m_serverAddr);
        m_operatingThread = std::move(other.m_operatingThread);
        m_mutex = std::move(other.m_mutex);
    }
    return *this;
}

bool TCPServer::startListening()
{
    return listen(m_serverSocket, ServerManager::getMaxAmountOfClients()) != 1;
}

TCPServer::Client TCPServer::acceptClient()
{
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(m_serverSocket, (sockaddr*)&clientAddr, &clientAddrLen);

    if (clientSocket == -1)
    {
        Log::error("TCPServer: Error accepting client");
        return {};
    }

    Log::info("Client connected: ", inet_ntoa(clientAddr.sin_addr), ":", ntohs(clientAddr.sin_port));

    return Client
    (
        inet_ntoa(clientAddr.sin_addr),
        ntohs(clientAddr.sin_port),
        clientSocket
    );
}

bool TCPServer::sendToClient(int clientSocket, const char* data, size_t dataSize) 
{
    return write(clientSocket, data, dataSize) == static_cast<long>(dataSize);
}

void TCPServer::start()
{
    std::lock_guard<std::mutex> lock(*m_mutex);
    m_operatingThread = std::thread(&TCPServer::operatingLoop, this);
}

void TCPServer::operatingLoop()
{
    while (true)
    {
        char buffer[NEXILIS_BUFFER];
        auto client = acceptClient();

        ssize_t bytesRead = read(client.getSocket(), buffer, sizeof(buffer));

        buffer[bytesRead] = '\0';
        std::string receivedData(buffer);
        receivedData.resize(bytesRead);

        if (bytesRead > 0)
        {
            auto message = getMessageHandler().readMessage(client.getAddress(), receivedData, client.getPort(), Command::getAuthentication());

            auto sendMsg = [this, &client](const std::vector<uint8_t> data)
            {
                auto charData = reinterpret_cast<const char*>(data.data());
                sendToClient(client.getSocket(), charData, sizeof(charData));
            };

            if (message.getClient())
            {
                if (!Command::read(message.getData(), *message.getClient(), *this, sendMsg))
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
