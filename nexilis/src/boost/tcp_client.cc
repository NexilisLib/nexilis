#include <nexilis/boost/tcp_client.hh>
#include <nexilis/log.hh>

#include <iostream>
#include <thread>

namespace nexilis::boost
{

TCPClient::TCPClient(const std::string& serverIP, const std::string& serverPort) :
    m_socket(m_ioService),
    m_resolver(m_ioService),
    m_iterator(m_resolver.resolve({serverIP, serverPort}))
{
}

TCPClient::~TCPClient()
{
    stop();
}

void TCPClient::stop()
{
    m_stopped = true;
    m_ioService.stop();

    if (m_ioServiceThread.joinable())
    {
        m_ioServiceThread.join();
    }
    if (m_receiveThread.joinable())
    {
        m_receiveThread.join();
    }
}

bool TCPClient::connectToServer()
{
    ::boost::asio::connect(m_socket, m_iterator);
    return m_socket.is_open();
}

bool TCPClient::send(const std::string& data)
{
    std::lock_guard<std::mutex> lock(m_socketMutex);

    if (m_socket.is_open())
    {
        // Asynchronously send data to the server
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
    std::lock_guard<std::mutex> lock(m_socketMutex);

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
        Log::info("Connected to server!");

        m_ioServiceThread = std::thread([this]() { m_ioService.run(); });

        // Start a separate thread to continuously receive messages.
        m_receiveThread = std::thread(&TCPClient::receiveLoop, this);
    }
    else
    {
        Log::error("Failed to connect to the server");
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
                Log::error("Reveived from server: ", buffer);
            }
            else
            {
                Log::info("Received empty message from server");
                break;
            }
        }
        else
        {
            Log::info("Error receiving from server");
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

}
