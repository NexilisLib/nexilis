#include <nexilis/logger/loggable.hh>
#include <nexilis/server/command.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>
#include <nexilis/util.hh>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>

namespace nexilis::server::nxboost
{

TCPServer::TCPServer(const Settings& settings) noexcept
    : ServerProtocol(settings),
      m_stopped(std::make_unique<std::atomic<bool>>(false)),
      m_mutex(std::make_unique<std::mutex>()),
      m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_acceptor(*m_ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 0))
{
    // Enable SO_REUSEADDR to allow port reuse.
    m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
}

TCPServer::TCPServer(TCPServer&& other) noexcept
    : Protocol(std::move(other)),
      ServerProtocol(std::move(other)),
      m_stopped(std::move(other.m_stopped) ? std::move(other.m_stopped) : std::make_unique<std::atomic<bool>>(false)),
      m_mutex(std::move(other.m_mutex)),
      m_ioContext(std::move(other.m_ioContext)),
      m_acceptor(std::move(other.m_acceptor)),
      m_listenThread(std::move(other.m_listenThread)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_serverPort(std::move(other.m_serverPort))
{
}

TCPServer& TCPServer::operator=(TCPServer&& other) noexcept
{
    if (this != &other)
    {
        m_stopped = std::move(other.m_stopped);
        if (!m_stopped)
        {
            m_stopped = std::make_unique<std::atomic<bool>>(false);
        }
        m_mutex = std::move(other.m_mutex);
        m_ioContext = std::move(other.m_ioContext);
        m_acceptor = std::move(other.m_acceptor);
        m_listenThread = std::move(other.m_listenThread);
        m_ioContextThread = std::move(other.m_ioContextThread);
        m_serverPort = std::move(other.m_serverPort);

        Protocol::operator=(std::move(other));
        ServerProtocol::operator=(std::move(other));
    }
    return *this;
}

TCPServer::~TCPServer()
{
    stop();
    Util::cleanupPortFile(getType());
}

void TCPServer::start()
{
    // clang-format off
    m_ioContextThread = std::thread([this]()
        { m_ioContext->run(); });

    m_listenThread = std::thread([this]()
    {
        if (startListening())
        {
            acceptClients();
        }
    });
    // clang-format on
}

void TCPServer::stop()
{
    if (!m_stopped)
    {
        Log::error("m_stopped is null, preventing crash");
        return;
    }

    if (m_stopped->exchange(true, std::memory_order_relaxed))
    {
        Log::debug("Already stopped TCPServer.");
        return;
    }

    if (m_acceptor.is_open())
    {
        boost::system::error_code ec;
        if (m_acceptor.cancel(ec))
        {
            Log::error("Error cancelling socket operations: ", ec.message());
        }
        if (m_acceptor.close(ec))
        {
            Log::error("Error closing socket: ", ec.message());
        }
    }

    if (m_ioContext)
    {
        m_ioContext->stop();
    }
    if (m_ioContextThread.joinable())
    {
        m_ioContextThread.join();
    }
    if (m_listenThread.joinable())
    {
        m_listenThread.join();
    }
    for (auto& client_thread : m_clientThreads)
    {
        if (client_thread.joinable())
        {
            client_thread.join();
        }
    }
    m_clientThreads.clear();
}

bool TCPServer::startListening()
{
    m_acceptor.listen();
    m_serverPort = m_acceptor.local_endpoint().port();
    Log::debug("Boost TCP server started on port: ", m_serverPort);
    if (!Util::writePortToFile(m_serverPort, getType()))
    {
        Log::error("Failed to write Boost TCP server port to a file.");
    }
    return true;
}

bool TCPServer::acceptClients()
{
    try
    {
        while (!m_stopped->load())
        {
            // Create a new socket for each client connection
            boost::asio::ip::tcp::socket newSocket(*m_ioContext);
            boost::system::error_code accept_error;
            m_acceptor.accept(newSocket);

            if (accept_error)
            {
                Log::error("Error accepting client connection: ", accept_error.message());
                continue; // Proceed to accept the next client
            }

            // Handle each client in a separate thread
            // clang-format off
            auto client_thread = std::thread([this, newSocket = std::move(newSocket)]() mutable
            {
            // clang-format on
                try
                {
                    std::string clientAddress;
                    uint16_t clientPort;

                    try
                    {
                        boost::asio::ip::tcp::endpoint remoteEndpoint = newSocket.remote_endpoint();
                        boost::asio::ip::address remoteAddress = remoteEndpoint.address();
                        clientAddress = remoteAddress.to_string();
                        clientPort = remoteEndpoint.port();
                        Log::debug("Remote IP address: ", clientAddress);
                    }
                    catch (const std::exception& e)
                    {
                        Log::debug("Error getting info from remote, reason: ", e.what());
                    }

                    while (true)
                    {
                        boost::asio::streambuf receiveBuffer;
                        boost::system::error_code error_code;

                        boost::asio::read(newSocket, receiveBuffer, boost::asio::transfer_at_least(1), error_code);

                        if (error_code == boost::asio::error::eof)
                        {
                            Log::debug("End receive ", clientAddress);
                            break;
                        }
                        else if (error_code)
                        {
                            // Handle other errors
                            Log::error("TCPServer Error reading from client: ", error_code.message());
                            break;
                        }

                        // Create a vector to hold the data.
                        nx_data data;

                        // Get the sequence of const buffers from the streambuf.
                        const boost::asio::const_buffer& receive_buffer = receiveBuffer.data();

                        // Check if the buffer is empty.
                        if (receive_buffer.size() == 0)
                        {
                            Log::info("Empty data");
                            continue;
                        }

                        // Extract the data.
                        const uint8_t* buffer_data = static_cast<const uint8_t*>(receive_buffer.data());
                        size_t buffer_size = receive_buffer.size();

                        // Insert the data from the buffer into the vector.
                        data.insert(data.end(), buffer_data, buffer_data + buffer_size);

                        auto handledMessage = getMessageHandler().readMessage(clientAddress, data, clientPort, &getCommand().getSettings());

                        if (!handledMessage.getClient()->isBoostTCPSet())
                        {
                            handledMessage.getClient()->setBoostTCPSend([this, &newSocket](const nx_data& bytes)
                            {
                                if (sendToClient(bytes, newSocket))
                                {
                                    Log::info("Sent message to client succesfully");
                                }
                                else
                                {
                                    Log::error("Error sending message to client");
                                }
                            });
                        }

                        Command::Result passCommand = getCommand().read(handledMessage.getData(), *handledMessage.getClient(), *this, handledMessage.getMessageId());

                        // TODO Command handling.

                        if (passCommand == Command::Result::success)
                        {
                            Log::info("Passed");
                        }
                        else
                        {
                            Log::info("Failed");
                        }
                    }
                }
                catch (const boost::system::system_error& e)
                {
                    // Handle errors or client disconnect here
                    Log::error("Error in client thread: ", e.what());
                }
                catch (const std::exception& e)
                {
                    Log::error("Exception in client thread: ", e.what());
                } });

            client_thread.detach();

            {
                std::lock_guard<std::mutex> lock(*m_mutex);
                m_clientThreads.emplace_back(std::move(client_thread));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    catch (const std::exception& e)
    {
        Log::error("Exception in acceptClients: ", e.what());
        return false;
    }
    return true;
}

bool TCPServer::sendToClient(const nx_data& data, boost::asio::ip::tcp::socket& clientSocket)
{
    if (clientSocket.is_open())
    {
        boost::system::error_code ec;
        boost::asio::write(clientSocket, boost::asio::buffer(data), ec);
        if (ec)
        {
            Log::error("Failed to send data: ", ec.message());
            return false;
        }
        return true;
    }
    return false;
}

} // namespace nexilis::server::nxboost
