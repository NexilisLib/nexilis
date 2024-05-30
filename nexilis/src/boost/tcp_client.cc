#include <nexilis/boost/tcp_client.hh>
#include <nexilis/log.hh>

#include <boost/asio/connect.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffers_iterator.hpp>

namespace nexilis
{

BoostTCPClient::BoostTCPClient(ClientAPI& api)
    : ClientProtocol(&api),
      m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_socket(*m_ioContext),
      m_resolver(*m_ioContext),
      m_iterator(m_resolver.resolve({api.getBoostTCPServerAddress(),
                                     std::to_string(api.getBoostTCPServerPortNumber())})),
      m_mutex(std::make_unique<std::mutex>())
{
}

BoostTCPClient::BoostTCPClient(BoostTCPClient&& other)
    : Protocol(std::move(other)),
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

BoostTCPClient& BoostTCPClient::operator=(BoostTCPClient&& other)
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

BoostTCPClient::~BoostTCPClient()
{
    stop();
}

void BoostTCPClient::stop()
{
    m_stopped = true;
    m_socket.close();
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

void BoostTCPClient::sendMessage(const std::vector<uint8_t>& message)
{
    send(message);
}

void BoostTCPClient::sendMessage(const std::vector<uint8_t>& message, const std::function<void()>& callback)
{
    uint64_t messageId = Util::getMessageIdFromNexilisMessage(message);
    std::pair<uint64_t, std::function<void()>> pair = std::make_pair(messageId, callback);
    ClientProtocol::getClientAPI()->addCallback(pair);
    send(message);
}

bool BoostTCPClient::connectToServer()
{
    try
    {
        boost::asio::connect(m_socket, m_iterator);
    }
    catch (...)
    {
        Log::error("Could not connect to server!");
    }
    return m_socket.is_open();
}

bool BoostTCPClient::send(const std::vector<uint8_t>& data)
{
    if (m_socket.is_open())
    {
        // Asynchronously send data to the server.
        boost::asio::async_write(m_socket, boost::asio::buffer(data),
                                 [](const boost::system::error_code& error, std::size_t /*bytes_transferred*/)
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

bool BoostTCPClient::receive(std::vector<uint8_t>& buffer)
{
    assert(buffer.size() == 0);

    std::lock_guard<std::mutex> lock(*m_mutex);

    boost::asio::streambuf receiveBuffer;
    boost::system::error_code error;

    boost::asio::read(m_socket, receiveBuffer, boost::asio::transfer_at_least(1), error);

    if (receiveBuffer.data().size() <= 0)
    {
        return false;
    }

    // Extract data from the receive buffer and copy it into the buffer vector
    std::vector<uint8_t> result;
    for (auto it = boost::asio::buffers_begin(receiveBuffer.data()); it != boost::asio::buffers_end(receiveBuffer.data()); ++it)
    {
        result.emplace_back(*it);
    }

    assert(buffer != result);
    buffer = result;

    return true;
}

void BoostTCPClient::start()
{
    if (connectToServer())
    {
        Log::info("Connected to server!");

        m_ioContextThread = std::thread([this]()
                                        { m_ioContext->run(); });

        // Start a separate thread to continuously receive messages.
        m_receiveThread = std::thread(&BoostTCPClient::receiveLoop, this);
    }
    else
    {
        Log::error("Failed to connect to the server");
    }
}

void BoostTCPClient::receiveLoop()
{
    while (true)
    {
        std::vector<uint8_t> buffer;

        if (receive(buffer))
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
            Log::info("Error receiving from server");
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} // namespace nexilis
