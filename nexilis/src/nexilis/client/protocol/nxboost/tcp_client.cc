#include <nexilis/client/protocol/nxboost/tcp_client.hh>

#include <boost/asio/buffers_iterator.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>

namespace nexilis::client::nxboost
{

TCPClient::TCPClient(ClientAPI& api)
    : Protocol(),
      ClientProtocol(&api),
      m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_socket(*m_ioContext),
      m_resolver(*m_ioContext),
      m_mutex(std::make_unique<std::mutex>())
{
}

TCPClient::TCPClient(TCPClient&& other)
    : Protocol(std::move(other)),
      ClientProtocol(std::move(other)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_receiveThread(std::move(other.m_receiveThread)),
      m_stopped(std::move(other.m_stopped)),
      m_ioContext(std::move(other.m_ioContext)),
      m_socket(std::move(other.m_socket)),
      m_resolver(std::move(other.m_resolver)),
      m_mutex(std::move(other.m_mutex))
{
    other.m_ioContext = nullptr;
    other.m_mutex = nullptr;
}

TCPClient& TCPClient::operator=(TCPClient&& other)
{
    if (this != &other)
    {
        m_ioContextThread = std::move(other.m_ioContextThread);
        m_receiveThread = std::move(other.m_receiveThread);
        m_stopped = std::move(other.m_stopped);
        m_ioContext = std::move(other.m_ioContext);
        m_socket = std::move(other.m_socket);
        m_resolver = std::move(other.m_resolver);
        m_mutex = std::move(other.m_mutex);

        other.m_mutex = nullptr;
        other.m_ioContext = nullptr;

        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
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

    if (m_socket.is_open())
    {
        boost::system::error_code ec;
        m_socket.cancel(ec);
        m_socket.close(ec);
    }

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

void TCPClient::sendMessage(const nx_data& message)
{
    send(message);
}

bool TCPClient::connectToServer()
{
    try
    {
        try
        {
            boost::asio::connect(m_socket, m_resolver.resolve(getClientAPI()->getBoostTCPServerAddress(), std::to_string(getClientAPI()->getBoostTCPServerPortNumber())));
        }
        catch (...)
        {
            Log::error("Connection failed!");
        }
    }
    catch (...)
    {
        Log::error("Could not connect to server!");
    }
    return m_socket.is_open();
}

bool TCPClient::send(const nx_data& data)
{
    if (!m_socket.is_open())
    {
        Log::error("TCPClient socket is not open SOCKET SEND");
        return false;
    }

    boost::asio::async_write(m_socket, boost::asio::buffer(data),
                             [this](const boost::system::error_code& error, std::size_t /*bytes_transferred*/)
                             {
                                 if (!error)
                                 {
                                     Log::info("Message sent successfully.");
                                 }
                                 else
                                 {
                                     Log::error("Send error: " + error.message());
                                     m_socket.close();
                                 }
                             });

    return true;
}

bool TCPClient::receive(nx_data& buffer)
{
    std::lock_guard<std::mutex> lock(*m_mutex);

    boost::asio::streambuf receiveBuffer;
    boost::system::error_code error;

    boost::asio::read(m_socket, receiveBuffer, boost::asio::transfer_at_least(1), error);

    // Handle the case where the socket is closed or an error occurs
    if (error == boost::asio::error::operation_aborted || error == boost::asio::error::eof)
    {
        return false;
    }

    if (error)
    {
        Log::error("Receive error: " + error.message());
        return false; // Some other error occurred
    }

    uint64_t size = receiveBuffer.size();
    if (size == 0)
    {
        return false;
    }

    // Allocate enough space in the buffer vector and copy data.
    buffer.resize(size);
    std::istream is(&receiveBuffer);
    is.read(reinterpret_cast<char*>(buffer.data()), size);

    return true;
}

void TCPClient::start()
{
    if (connectToServer())
    {
        Log::info("Connected to server!");

        m_ioContextThread = std::thread([this]()
                                        { m_ioContext->run(); });

        // Start a separate thread to continuously receive messages.
        m_receiveThread = std::thread(&TCPClient::receiveLoop, this);
    }
    else
    {
        Log::error("Failed to connect to the server");
    }
    ClientProtocol::start(getType());
}

void TCPClient::receiveLoop()
{
    while (!m_stopped)
    {
        nx_data buffer;

        if (!m_stopped && receive(buffer))
        {
            if (!buffer.empty())
            {
                auto a = ClientProtocol::getClientAPI()->readMessage(buffer);

                if (a == ClientAPI::ReadResult::success)
                {
                    Log::info("Read success");
                }
                else
                {
                    Util::debugUint8Vector(buffer);
                }
            }
            else
            {
                Log::info("Received empty message from server");
                break;
            }
        }
        else
        {
            if (m_stopped)
            {
                Log::info("Receive loop stopped due to stop signal");
                break;
            }
            Log::info("Error receiving from server");
            break;
        }
    }
}

} // namespace nexilis::client::nxboost
