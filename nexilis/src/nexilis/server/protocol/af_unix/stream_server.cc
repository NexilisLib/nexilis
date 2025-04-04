#ifdef __linux__

#include <nexilis/nexilis_constants.hh>
#include <nexilis/server/command.hh>
#include <nexilis/server/protocol/af_unix/stream_server.hh>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

namespace nexilis::server::af_unix
{

StreamServer::StreamServer(const Settings& settings, const std::string& socketPath)
    : ServerProtocol(settings),
      m_socketPath(socketPath),
      m_buffer(NEXILIS_BUFFER)
{
    createSocket();
    bindSocket();
}

StreamServer::~StreamServer()
{
    if (m_receiveThread.joinable())
    {
        m_receiveThread.join();
    }

    close(m_serverSocket);
}

StreamServer::StreamServer(StreamServer&& other)
    : Protocol(std::move(other)),
      ServerProtocol(std::move(other)),
      m_socketPath(std::move(other.m_socketPath)),
      m_serverSocket(std::move(other.m_serverSocket)),
      m_buffer(std::move(std::move(other.m_buffer))),
      m_receiveThread(std::move(other.m_receiveThread))
{
}

StreamServer& StreamServer::operator=(StreamServer&& other)
{
    if (this != &other)
    {
        m_socketPath = std::move(other.m_socketPath);
        m_serverSocket = std::move(other.m_serverSocket);
        m_buffer = std::move(other.m_buffer);
        m_receiveThread = std::move(other.m_receiveThread);

        Protocol::operator=(std::move(other));
        ServerProtocol::operator=(std::move(other));
    }
    return *this;
}

void StreamServer::start()
{
    m_receiveThread = std::thread([this]()
                                  {
        while (true)
        {
            handleMessages();
        } });
}

void StreamServer::createSocket()
{
    m_serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_serverSocket == -1)
    {
        Log::error("Couldn't create socket");
    }
}

void StreamServer::bindSocket()
{
    sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strcpy(serverAddr.sun_path, m_socketPath.c_str());

    // Remove old socket file.
    // This operation will fail if this is the first usage and it's okay.
    unlink(m_socketPath.c_str());

    auto address = reinterpret_cast<sockaddr*>(&serverAddr);
    if (bind(m_serverSocket, address, sizeof(serverAddr)) == -1)
    {
        Log::error("Failed to bind socket");
        close(m_serverSocket);
    }

    if (listen(m_serverSocket, 30) == -1)
    {
        Log::error("Failed to listen to socket");
        close(m_serverSocket);
    }
}

void StreamServer::sendMessage(int clientSocket, const nx_data& message)
{
    ssize_t sentBytes = send(clientSocket, message.data(), sizeof(message), 0);

    if (sentBytes == -1)
    {
        Log::error("Failed to send message");
    }
}

std::string StreamServer::receiveMessage(int socket)
{
    std::string message;
    char buffer[NEXILIS_BUFFER];

    while (true)
    {
        auto bytesRead = recv(socket, buffer, sizeof(buffer), 0);

        if (bytesRead > 0)
        {
            message.append(buffer, bytesRead);

            // Check if the message contains the null terminator.
            size_t nullPos = message.find('\0');

            if (nullPos != std::string::npos)
            {
                return message.substr(0, nullPos);
            }
            else
            {
                Log::error("Received message that does",
                           " not contain the null-termination character");
                break;
            }
        }

        else if (bytesRead == 0)
        {
            Log::info("Connection closed by peer");
            break;
        }
        else
        {
            Log::error("Error receiving message");
            break;
        }
    }
    return "";
}

void StreamServer::handleMessages()
{
    memset(m_buffer.data(), '\0', m_buffer.size());

    int clientSocket = accept(m_serverSocket, nullptr, nullptr);
    if (clientSocket == -1)
    {
        Log::error("Failed to accept connection");
        close(m_serverSocket);
    }

    while (true)
    {
        std::string message = receiveMessage(clientSocket);

        if (message == "")
        {
            break;
        }
        else
        {
            nx_data payload = Util::convertToByteVector(message);
            auto msg = getMessageHandler().readMessage("127.0.0.1", payload, -1, &getCommand().getSettings());

            if (msg.getClient())
            {
                auto handledMessage = getMessageHandler().readMessage(msg.getAddress(), payload, msg.getPort(), &getCommand().getSettings());

                if (!handledMessage.getClient()->isUnixStreamSet())
                {
                    handledMessage.getClient()->setUnixStreamSend([this, &clientSocket](const nx_data& bytes)
                                                                  { sendMessage(clientSocket, bytes); });
                }

                Command::Result result = getCommand().read(handledMessage.getData(), *handledMessage.getClient(), *this, handledMessage.getMessageId());
                getCommand().checkResult(result);

                if (result == Command::Result::success)
                {
                    Log::info("Passed");
                }
            }
            else
            {
                Log::error("Message from unauthorized client!");
            }
        }
    }
}

} // namespace nexilis::server::af_unix

#endif
