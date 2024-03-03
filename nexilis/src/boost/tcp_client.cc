#include <nexilis/boost/tcp_client.hh>
#include <nexilis/log.hh>

#include <thread>

namespace nexilis::boost
{

TCPClient::TCPClient(ClientAPI& api) :
    ClientProtocol(&api),
    m_ioContext(std::make_unique<::boost::asio::io_context>()),
    m_socket(*m_ioContext),
    m_resolver(*m_ioContext),
    m_iterator(m_resolver.resolve({ 
        api.getBoostTCPServerAddress(), 
        std::to_string(api.getBoostTCPServerPortNumber()) 
    })),
    m_mutex(std::make_unique<std::mutex>())
{
}

TCPClient::TCPClient(TCPClient&& other) :
    Protocol(std::move(other)),
    ClientProtocol(std::move(other)),
    m_ioContextThread(std::move(other.m_ioContextThread)),
    m_receiveThread(std::move(other.m_receiveThread)),
    m_stopped(std::move(other.m_stopped)),
    m_ioContext(std::move(other.m_ioContext)),
    m_socket(std::move(other.m_socket)),
    m_resolver(std::move(other.m_resolver)),
    m_iterator(std::move(other.m_iterator)),
    m_mutex(std::move(other.m_mutex))
{
    other.m_ioContext = nullptr;
    other.m_mutex = nullptr;
}

TCPClient& TCPClient::operator=(TCPClient&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
        m_ioContextThread = std::move(other.m_ioContextThread);
        m_receiveThread = std::move(other.m_receiveThread);
        m_stopped = std::move(other.m_stopped);
        m_ioContext = std::move(other.m_ioContext);
        m_socket = std::move(other.m_socket);
        m_resolver = std::move(other.m_resolver);
        m_iterator = std::move(other.m_iterator);
        m_mutex = std::move(other.m_mutex);

        other.m_mutex = nullptr;
        other.m_ioContext = nullptr;
    }
    return *this;
}

TCPClient::~TCPClient()
{
    stop();
}

void TCPClient::stop()
{
    m_stopped = true;
    m_ioContext->stop();

    if (m_ioContextThread.joinable())
    {
        m_ioContextThread.join();
    }
    if (m_receiveThread.joinable())
    {
        m_receiveThread.join();
    }
}

void TCPClient::sendMessage(const std::string& message)
{
    send(message);
}

void TCPClient::sendMessage(const std::vector<uint8_t>& message)
{
    send(Util::convertToString(message));
}

bool TCPClient::connectToServer()
{
    ::boost::asio::connect(m_socket, m_iterator);
    return m_socket.is_open();
}

bool TCPClient::send(const std::string& data)
{
    if (m_socket.is_open())
    {
        // Asynchronously send data to the server.
        ::boost::asio::async_write(m_socket, ::boost::asio::buffer(data),
            [this](const ::boost::system::error_code& error, std::size_t /*bytes_transferred*/)
            {
                if (!error)
                {
                    Log::info("Message sent successfully.");
                    return true;
                }
                else
                {
                    Log::error("Send error: " + error.message());
                    return false;
                }
            });
        return false;
    }
    else
    {
        Log::error("TCPClient socket is not open SOCKET SEND");
        return false;
    }
}

bool TCPClient::receive(std::string& buffer)
{
    std::lock_guard<std::mutex> lock(*m_mutex);

    ::boost::asio::streambuf receiveBuffer;
    ::boost::system::error_code error;

    ::boost::asio::read(m_socket, receiveBuffer, ::boost::asio::transfer_at_least(1), error);

    if (receiveBuffer.data().size() <= 0)
    {
        return false;
    }

    buffer = ::boost::asio::buffer_cast<const char*>(receiveBuffer.data());
    return true;
}

void TCPClient::start()
{
    if (connectToServer())
    {
        Log::info(logName(), "Connected to server!");

        m_ioContextThread = std::thread([this]() { m_ioContext->run(); });

        // Start a separate thread to continuously receive messages.
        m_receiveThread = std::thread(&TCPClient::receiveLoop, this);
    }
    else
    {
        Log::error(logName(), "Failed to connect to the server");
    }
}

void TCPClient::receiveLoop()
{
    while (true)
    {
        std::string buffer;

        if (receive(buffer))
        {
            if (!buffer.empty())
            {
                auto message = Util::convertToByteVector(buffer.c_str(), buffer.size());
                ClientProtocol::getClientAPI()->readMessage(message);
            }
            else
            {
                Log::info(logName(), "Received empty message from server");
                break;
            }
        }
        else
        {
            Log::info(logName(), "Error receiving from server");
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

}
