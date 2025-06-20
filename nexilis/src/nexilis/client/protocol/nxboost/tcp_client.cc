#include <nexilis/client/protocol/nxboost/tcp_client.hh>

#include <nexilis/logger/log.hh>

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
      NxClass("server::nxboost::TCPClient"),
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
      NxClass(std::move(other)),
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
        NxClass::operator=(std::move(other));
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
    if (!m_stopped || m_stopped->exchange(true))
    {
        return;
    }

    m_workGuard.reset();

    boost::system::error_code ec;
    if (m_socket.cancel(ec))
    {
        Log::error(header(), "Error cancelling socket operations: ", ec.message());
    }
    if (ec)
    {
        Log::error(header(), "Cancel error: ", ec.message());
    }
    if (m_ioContext)
    {
        m_ioContext->stop();
        Log::debug(header(), "BoostTCPClient io_context stopped");
    }
    if (m_ioContextThread.joinable())
    {
        m_ioContextThread.join();
    }
    if (m_receiveThread.joinable())
    {
        m_receiveThread.join();
    }
    if (m_socket.close(ec))
    {
        Log::error(header(), "Error closing socket: ", ec.message());
    }
    if (ec)
    {
        Log::error(header(), "Closing error: ", ec.message());
    }
}

void TCPClient::sendMessage(const nx_data& message)
{
    send(message);
}

void TCPClient::sendMessage(const nx_data& message, const std::function<void()>& callback)
{
    sendMessageWithCallback(message, callback);
}

bool TCPClient::connectToServer()
{
    try
    {
        auto port = Util::readPortFromFile(Protocol::Type::BOOST_TCP_SERVER);
        if (!port)
        {
            Log::error(header(), "Could not read TCP server port from a file.");
            return false;
        }
        try
        {
            boost::asio::connect(m_socket, m_resolver.resolve(getClientAPI()->getBoostTCPServerAddress(), std::to_string(*port)));
        }
        catch (...)
        {
            Log::error(header(), "Connection failed!");
        }
    }
    catch (...)
    {
        Log::error(header(), "Could not connect to server!");
    }
    return m_socket.is_open();
}

bool TCPClient::send(const nx_data& data)
{
    if (!m_sendMutex)
    {
        Log::error(header(), "Send mutex is invalid");
        return false;
    }
    std::unique_lock<std::mutex> lock(*m_sendMutex, std::defer_lock);
    if (!m_socket.is_open())
    {
        Log::error(header(), "TCPClient socket is not open SOCKET SEND");
        return false;
    }

    boost::asio::async_write(m_socket, boost::asio::buffer(data),
                             [this](const boost::system::error_code& error, std::size_t /*bytes_transferred*/)
                             {
                                 if (!error)
                                 {
                                     Log::info(header(), "Message sent successfully.");
                                 }
                                 else
                                 {
                                     Log::error(header(), "Send error: " + error.message());
                                     m_socket.close();
                                 }
                             });

    return true;
}

void TCPClient::receive(const std::function<void(nx_data)>& callback)
{
    if (!m_socket.is_open())
    {
        Log::error(header(), "TCPClient socket is not open for receiving.");
        return;
    }
    if (m_stopped->load())
    {
        return;
    }

    auto receiveBuffer = std::make_shared<boost::asio::streambuf>();

    // clang-format off
    boost::asio::async_read(m_socket, *receiveBuffer, boost::asio::transfer_at_least(1),
        [this, receiveBuffer, callback](const boost::system::error_code& ec, size_t bytes_transferred)
        {
            if (ec || m_stopped->load())
            {
                if (ec != boost::asio::error::operation_aborted)
                {
                    m_socket.close();
                }
                Log::error(header(), "Receive error: ", ec.message());
                return;
            }

            if (bytes_transferred == 0)
            {
                Log::warning(header(), "No data received!");
                return;
            }

            nx_data buffer(bytes_transferred);
            std::istream is(receiveBuffer.get());
            is.read(reinterpret_cast<char*>(buffer.data()), bytes_transferred);
            Log::info(header(), "Received message of size: ", bytes_transferred);

            if (callback)
            {
                callback(buffer);
            }

            if (m_socket.is_open())
            {
                receive(callback);
            }
    });
    // clang-format on
}

void TCPClient::start()
{
    if (connectToServer())
    {
        Log::info(header(), "Connected to server!");

        // clang-format off
        m_ioContextThread = std::thread([this]()
        {
            Log::debug(header(), "BoostTCPClient io_context thread started.");
            m_ioContext->run();
            Log::debug(header(), "BoostTCPClient io_context thread stopped.");
        });

        m_receiveThread = std::thread([this]()
        {
            Log::debug(header(), "BoostTCPClient Receive loop started");
            receiveLoop();
            Log::debug(header(), "BoostTCPClient Receive loop stopped");
        });
        // clang-format on

        ClientProtocol::start(getType());
    }
    else
    {
        Log::error(header(), "Failed to connect to the server");
    }
}

void TCPClient::receiveLoop()
{
    while (!m_stopped->load())
    {
        auto cb = [this](const nx_data& buffer)
        {
            if (m_stopped->load())
            {
                return;
            }
            try
            {
                auto result = ClientProtocol::getClientAPI()->readMessage(buffer);
                if (result == ClientAPI::ReadResult::success)
                {
                    Log::info(header(), "Message read successfully");
                }
                else
                {
                    Log::warning(header(), "Received unexpected message: ");
                    Log::warning(header(), "Type: ", ClientAPI::readResultStr(result));
                    Util::debugUint8Vector(buffer);
                }
            }
            catch (const std::exception& e)
            {
                Log::error(header(), "Exception in receive callback: ", e.what());
            }
            catch (...)
            {
                Log::error(header(), "Unknown error in receive callback");
            }
        };

        receive(cb);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

} // namespace nexilis::client::nxboost
