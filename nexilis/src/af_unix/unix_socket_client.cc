#include <nexilis/af_unix/unix_socket_client.hh>

#include <sys/socket.h>
#include <unistd.h>

namespace nexilis::af_unix
{

UnixSocketClient::UnixSocketClient(ClientAPI& api) :
    Protocol(),
    m_api(api),
    m_serverSocketPath(m_api.getUnixSocketServerPath())
{
    createSocket();
    sendMessage(api.getClientPassword());
}

UnixSocketClient::~UnixSocketClient()
{
    if (m_clientSocket != -1)
    {
        close(m_clientSocket);
    }
}

void UnixSocketClient::sendMessage(const std::string& message)
{
    ssize_t sentBytes = sendto(m_clientSocket, message.c_str(), message.length(), 0,
           reinterpret_cast<const struct sockaddr*>(&m_serverAddr), sizeof(m_serverAddr));

    if (sentBytes == -1)
    {
        perror("sendto");
        std::cout << "Something went wrong with the client sending the message" << std::endl;
    }
}

std::string UnixSocketClient::receiveMessage()
{
    char buffer[1024];
    ssize_t bytesRead = recvfrom(m_clientSocket, buffer, sizeof(buffer), 0, nullptr, nullptr);

    if (bytesRead == -1)
    {
        perror("recvfrom");
    }
    else if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        return std::string(buffer);
    }
    return "";
}

void UnixSocketClient::start()
{
    while (true)
    {
        auto data = receiveMessage();
        std::cout << "data: " << data << std::endl;

        auto message = Util::convertToByteVector(data.c_str(), data.size());

        m_api.readMessage(message);
    }
}

void UnixSocketClient::stop()
{
    close(m_clientSocket);
}

void UnixSocketClient::createSocket()
{
    m_clientSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (m_clientSocket == -1)
    {
        std::cerr << "Error creating client socket" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sun_family = AF_UNIX;
    strncpy(m_serverAddr.sun_path, m_serverSocketPath.c_str(), sizeof(m_serverAddr.sun_path) - 1);

    std::cout << "Connected to server, address family: " << m_serverAddr.sun_family << std::endl;
    std::cout << "Server path: " << m_serverSocketPath << std::endl;
}

}
