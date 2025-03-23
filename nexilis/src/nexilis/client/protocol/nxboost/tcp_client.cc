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
      m_stopped(std::make_unique<std::atomic<bool>>(false)),
      m_ioContext(std::make_shared<boost::asio::io_context>()),
      m_workGuard(std::make_unique<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(
        boost::asio::make_work_guard(*m_ioContext))),
      m_socket(*m_ioContext),
      m_resolver(*m_ioContext),
      m_sendMutex(std::make_shared<std::mutex>()),
      m_receiveMutex(std::make_shared<std::mutex>())
{
}

TCPClient::TCPClient(TCPClient&& other)
    : Protocol(std::move(other)),
      ClientProtocol(std::move(other)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_receiveThread(std::move(other.m_receiveThread)),
      m_stopped(std::move(other.m_stopped)),
      m_ioContext(std::move(other.m_ioContext)),
      m_workGuard(std::move(other.m_workGuard)),
      m_socket(std::move(other.m_socket)),
      m_resolver(std::move(other.m_resolver)),
      m_sendMutex(std::move(other.m_sendMutex)),
      m_receiveMutex(std::move(other.m_receiveMutex))
{
}

TCPClient& TCPClient::operator=(TCPClient&& other)
{
    if (this != &other)
    {
        if (m_ioContextThread.joinable())
        {
            m_ioContextThread.join();
        }
        if (m_receiveThread.joinable())
        {
            m_receiveThread.join();
        }

        m_ioContextThread = std::move(other.m_ioContextThread);
        m_receiveThread = std::move(other.m_receiveThread);
        m_stopped = std::move(other.m_stopped);
        m_ioContext = std::move(other.m_ioContext);
        m_workGuard = std::move(other.m_workGuard);
        m_socket = std::move(other.m_socket);
        m_resolver = std::move(other.m_resolver);
        m_sendMutex = std::move(other.m_sendMutex);
        m_receiveMutex = std::move(other.m_receiveMutex);

        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
    }
    return *this;
}

TCPClient::~TCPClient()
{
    stop();
    m_workGuard.reset();
}

void TCPClient::stop()
{
    if (!m_stopped)
    {
        Log::error("Stopped is null");
        return;
    }

    if (m_stopped->load())
    {
        return;
    }

    m_stopped->store(true, std::memory_order_relaxed);

    if (m_ioContext)
    {
        m_ioContext->stop();
    }

    if (m_socket.is_open())
    {
        boost::system::error_code ec;
        if (m_socket.cancel(ec))
        {
            Log::error("Error cancelling socket operations: ", ec.message());
        }
        if (m_socket.close(ec))
        {
            Log::error("Error closing socket: ", ec.message());
        }
    }
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
            boost::asio::connect(m_socket, m_resolver.resolve(getClientAPI()->getBoostTCPServerAddress(),
                                                              std::to_string(getClientAPI()->getBoostTCPServerPortNumber())));
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
    if (!m_sendMutex)
    {
        Log::error("Send mutex is invalid");
        return false;
    }
    std::unique_lock<std::mutex> lock(*m_sendMutex, std::defer_lock);
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

void TCPClient::receive(const std::function<void(nx_data)>& callback)
{
    if (!m_socket.is_open())
    {
        Log::error("TCPClient socket is not open for receiving.");
        return;
    }

    auto receiveBuffer = std::make_shared<boost::asio::streambuf>();

    boost::asio::async_read(m_socket, *receiveBuffer, boost::asio::transfer_at_least(1),
            [this, receiveBuffer, callback](const boost::system::error_code& ec, size_t bytes_transferred)
    {
        if (ec)
        {
            Log::error("Receive error: ", ec.message());
            m_socket.close();
            return;
        }

        if (bytes_transferred == 0)
        {
            Log::warning("No data received!");
            return;
        }

        nx_data buffer(bytes_transferred);
        std::istream is(receiveBuffer.get());
        is.read(reinterpret_cast<char*>(buffer.data()), bytes_transferred);
        Log::info("Received message of size: ", bytes_transferred);

        if (callback)
        {
            Log::debug("Calling callback");
            callback(buffer);
        }

        if (m_socket.is_open())
        {
            receive(callback);
        }
    });
}

void TCPClient::start()
{
    if (connectToServer())
    {
        Log::info("Connected to server!");

        m_ioContextThread = std::thread([this]()
        {
            Log::info("io_context thread started.");
            m_ioContext->run();
            Log::info("io_context thread stopped.");
        });

        // Start a separate thread to continuously receive messages.
        m_receiveThread = std::thread(&TCPClient::receiveLoop, this);
        Log::info("Receive thread started");

        ClientProtocol::start(getType());
    }
    else
    {
        Log::error("Failed to connect to the server");
    }
}

void TCPClient::receiveLoop()
{
    Log::info("Receive started");

    // Start the asynchronous receive operation
    auto cb = [this](const nx_data& buffer)
    {
        try
        {
            auto result = ClientProtocol::getClientAPI()->readMessage(buffer);
            if (result == ClientAPI::ReadResult::success)
            {
                Log::info("Message read successfully");
            }
            else
            {
                Log::info("Received unexpected message: ");
                Util::debugUint8Vector(buffer);
            }
        }
        catch (const std::exception& e)
        {
            Log::error("Exception in receive callback: ", e.what());
        }
        catch (...)
        {
            Log::error("Unknown error in receive callback");
        }
    };

    receive(cb);
}

} // namespace nexilis::client::nxboost
