#include <nexilis/af_unix/unix_socket_client.hh>

namespace nexilis::af_unix
{

UnixSocketClient::UnixSocketClient(ClientAPI& api) :
    Protocol(),
    m_api(api),
    m_serverSocketPath(m_api.getUnixSocketServerPath())
{
    createSocket();
    connectToServer();
    sendMessage(api.getClientPassword());
}

void UnixSocketClient::sendMessage(const std::string& message)
{
    sendto(m_clientSocket, message.c_str(), message.length(), 0,
           reinterpret_cast<const struct sockaddr*>(&m_serverAddr), sizeof(m_serverAddr));
}

std::string UnixSocketClient::receiveMessage()
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
}

void UnixSocketClient::createSocket()
{
    m_clientSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (m_clientSocket == -1)
    {
        std::cerr << "Error creating client socket" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void UnixSocketClient::connectToServer()
{
    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sun_family = AF_UNIX;
    strncpy(m_serverAddr.sun_path, m_serverSocketPath.c_str(), sizeof(m_serverAddr.sun_path) - 1);

    // Note: In a real-world scenario, error handling after connect should be more robust.
    if (connect(m_clientSocket, reinterpret_cast<const struct sockaddr*>(&m_serverAddr), sizeof(m_serverAddr)) == -1)
    {
        std::cerr << "Error connecting to server" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

}
