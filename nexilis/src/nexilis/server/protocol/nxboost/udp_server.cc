#include <nexilis/logger/log.hh>
#include <nexilis/server/command.hh>
#include <nexilis/server/protocol/nxboost/udp_server.hh>

#include <boost/asio/ip/address.hpp>
#include <boost/system/system_error.hpp>

namespace nexilis::server::nxboost
{

UDPServer::UDPServer(const Settings& settings)
    : ServerProtocol(settings),
      m_stopped(std::make_unique<std::atomic<bool>>(false)),
      m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_mutex(std::make_unique<std::mutex>()),
      m_remoteEndpoint(boost::asio::ip::udp::v4(), 0),
      m_socket(*m_ioContext, m_remoteEndpoint),
      m_receiveBuffer(NEXILIS_BUFFER)
{
    // Set socket option to allow address reuse
    boost::asio::ip::udp::socket::reuse_address reuse(true);
    m_socket.set_option(reuse);
}

UDPServer::~UDPServer()
{
    stop();
    Util::cleanupPortFile(getType());
}

UDPServer::UDPServer(UDPServer&& other)
    : Protocol(std::move(other)),
      ServerProtocol(std::move(other)),
      m_stopped(std::move(other.m_stopped) ? std::move(other.m_stopped) : std::make_unique<std::atomic<bool>>(false)),
      m_ioContext(std::move(other.m_ioContext)),
      m_mutex(std::move(other.m_mutex)),
      m_remoteEndpoint(std::move(other.m_remoteEndpoint)),
      m_socket(std::move(other.m_socket)),
      m_receiveBuffer(std::move(other.m_receiveBuffer)),
      m_ioContextThread(std::move(other.m_ioContextThread)),
      m_receiveThread(std::move(other.m_ioContextThread))
{
}

UDPServer& UDPServer::operator=(UDPServer&& other)
{
    if (this != &other)
    {
        m_stopped = std::move(other.m_stopped);
        if (!m_stopped)
        {
            m_stopped = std::make_unique<std::atomic<bool>>(false);
        }
        m_ioContext = std::move(other.m_ioContext);
        m_mutex = std::move(other.m_mutex);
        m_remoteEndpoint = std::move(other.m_remoteEndpoint);
        m_socket = std::move(other.m_socket);
        m_receiveBuffer = std::move(other.m_receiveBuffer);
        m_ioContextThread = std::move(other.m_ioContextThread);
        m_receiveThread = std::move(other.m_receiveThread);

        Protocol::operator=(std::move(other));
        ServerProtocol::operator=(std::move(other));
    }
    return *this;
}

void UDPServer::start()
{
    auto local_endpoint = m_socket.local_endpoint();
    m_serverPort = local_endpoint.port();
    Log::debug("Boost UDP server started on port: ", m_serverPort);
    if (!Util::writePortToFile(m_serverPort, getType()))
    {
        Log::error("Failed to write Boost UDP server port to a file");
    }

    m_ioContextThread = std::thread([this]()
                                    { m_ioContext->run(); });

    m_receiveThread = std::thread(&UDPServer::receiveFromClients, this);
}

void UDPServer::stop()
{
    if (!m_stopped || m_stopped->exchange(true))
    {
        Log::debug("BoostUDPServer stop already in progress or completed.");
        return;
    }

    boost::system::error_code ec;

    if (m_socket.cancel(ec))
    {
        Log::error("Error cancelling socket: ", ec.message());
    }
    if (ec)
    {
        Log::error("Cancelling error: ", ec.message());
    }

    if (m_ioContext)
    {
        Log::debug("BoostUDPServer stopping io_context");
        m_ioContext->stop();
    }

    if (m_socket.is_open())
    {
        if (m_socket.close(ec))
        {
            Log::error("Error closing socket: ", ec.message());
        }
        if (ec)
        {
            Log::error("Socket closing error: ", ec.message());
        }
    }

    if (m_ioContextThread.joinable())
    {
        Log::debug("BoostUDPServer closing ioContextThread");
        m_ioContextThread.join();
    }

    if (m_receiveThread.joinable())
    {
        Log::debug("BoostUDPServer closing receiveThread");
        m_receiveThread.join();
    }
    Log::debug("BoostUDPServer stopped");
}

void UDPServer::receiveFromClients()
{
    std::lock_guard<std::mutex> lock(*m_mutex);

    while (m_socket.is_open())
    {
        try
        {
            m_receiveBuffer.resize(NEXILIS_BUFFER);

            size_t bytes_received = m_socket.receive_from(boost::asio::buffer(m_receiveBuffer), m_remoteEndpoint);

            std::string address = m_remoteEndpoint.address().to_string();
            uint16_t port = m_remoteEndpoint.port();
            Log::info("Received from ", address, " port:", port, " ", bytes_received, " bytes.", " Data: ", m_receiveBuffer.data());

            // Create a new buffer containing only the received bytes.
            nx_data received_message(m_receiveBuffer.begin(), m_receiveBuffer.begin() + bytes_received);

            auto handledMessage = getMessageHandler().readMessage(address, received_message, port, &getCommand().getSettings());

            // clang-format off
            if (!handledMessage.getUser()->isBoostUDPSet())
            {
                handledMessage.getUser()->setBoostUDPSend([this](const nx_data& bytes)
                {
                    if (m_socket.send_to(boost::asio::buffer(bytes), m_remoteEndpoint) == 0)
                    {
                        Log::error("Failed to send message to client");
                    }
                });
            }
            // clang-format on

            Command::Result passCommand = getCommand().read(handledMessage.getData(), *handledMessage.getUser(), *this, handledMessage.getMessageId());

            switch (passCommand)
            {
                case Command::Result::success:
                    Log::info("UDPServer: Passed");
                    break;

                default:
                    Log::error("UDPServer: Something FAILED");
            }
        }
        catch (const boost::system::system_error& e)
        {
            Log::error("Error receiving message: ", e.what());
        }
    }

    Log::error("Socket is not open!");
}

} // namespace nexilis::server::nxboost
