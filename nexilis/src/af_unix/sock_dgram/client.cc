#include <nexilis/af_unix/sock_dgram/client.hh>
#include <nexilis/common/util.hh>

#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>

namespace nexilis::af_unix::sock_dgram
{

Client::Client(ClientAPI& clientApi)
    : ClientProtocol(&clientApi),
      m_serverSocketPath(clientApi.getUnixDgramPath())
{
    createSocket();
}

Client::~Client()
{
    if (m_clientSocket != -1)
    {
        close(m_clientSocket);
    }
}

void Client::sendMessage(const std::vector<uint8_t>& message)
{
    (void)message;
    std::cout << "SENDING MESSAGE: " << m_serverAddr.sun_path << std::endl;

    // TODO
    // Perform without string conversion.
    /*
    ssize_t sentBytes = sendto(m_clientSocket, message.c_str(), message.size(), 0,
                               (struct sockaddr*)&m_serverAddr, sizeof(m_serverAddr));

    if (sentBytes == -1)
    {
        perror("sendto");
        std::cout << "Something went wrong with the client sending the message" << std::endl;
    }
    */
}

std::string Client::receiveMessage()
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

void Client::start()
{
    while (true)
    {
        auto data = receiveMessage();
        std::cout << "data: " << data << std::endl;

        auto message = Util::convertToByteVector(data.c_str(), data.size());

        ClientProtocol::getClientAPI()->readMessage(message);
    }
}

void Client::stop()
{
}

void Client::createSocket()
{
    m_clientSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (m_clientSocket == -1)
    {
        std::cerr << "Error creating client socket" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sun_family = AF_UNIX;
    strcpy(m_serverAddr.sun_path, m_serverSocketPath.c_str());

    std::cout << "SERVER SOCKET" << std::endl;
    std::cout << "address path: " << m_serverAddr.sun_path << std::endl;
    std::cout << "address family: " << m_serverAddr.sun_family << std::endl;
}

} // namespace nexilis::af_unix::sock_dgram
