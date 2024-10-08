#include <nexilis/boost/tcp_server.hh>
#include <nexilis/command.hh>
#include <nexilis/common/util.hh>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/write.hpp>

namespace nexilis
{

BoostTCPServer::BoostTCPServer(const Authentication& authentication, int serverPort)
    : Command(authentication),
      Loggable(Protocol::typeToString(getType()), __FILE__),
      m_mutex(std::make_unique<std::mutex>()),
      m_ioContext(std::make_unique<boost::asio::io_context>()),
      m_acceptor(*m_ioContext,
                 boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), std::stoi(std::to_string(serverPort))))
{
}

BoostTCPServer::BoostTCPServer(BoostTCPServer&& other)
    : Protocol(std::move(other)),
      ServerProtocol(std::move(other)),
      Command(std::move(other)),
      Loggable(std::move(other)),
      m_mutex(std::move(other.m_mutex)),
      m_ioContext(std::move(other.m_ioContext)),
      m_acceptor(std::move(other.m_acceptor)),
      m_listenThread(std::move(other.m_listenThread)),
      m_ioContextThread(std::move(other.m_ioContextThread))
{
}

BoostTCPServer& BoostTCPServer::operator=(BoostTCPServer&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ServerProtocol::operator=(std::move(other));
        Command::operator=(std::move(other));
        Loggable::operator=(std::move(other));
        m_mutex = std::move(other.m_mutex);
        m_ioContext = std::move(other.m_ioContext);
        m_acceptor = std::move(other.m_acceptor);
        m_listenThread = std::move(other.m_listenThread);
        m_ioContextThread = std::move(other.m_ioContextThread);
    }
    return *this;
}

BoostTCPServer::~BoostTCPServer()
{
    stop();
}

void BoostTCPServer::start()
{
    m_ioContextThread = std::thread([this]()
                                    { m_ioContext->run(); });

    m_listenThread = std::thread([this]()
                                 {
        if (startListening())
        {
            acceptClients();
        } });
}

void BoostTCPServer::stop()
{
    m_ioContext->stop();

    if (m_ioContextThread.joinable())
    {
        m_ioContextThread.join();
    }
    if (m_listenThread.joinable())
    {
        m_listenThread.join();
    }
}

bool BoostTCPServer::startListening()
{
    m_acceptor.listen();
    return true;
}

bool BoostTCPServer::acceptClients()
{
    while (true)
    {
        std::lock_guard<std::mutex> lock(*m_mutex);

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
        std::thread([this, newSocket = std::move(newSocket)]() mutable
        {
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

                    // Create a vector to hold the data
                    std::vector<uint8_t> data;

                    // Get the sequence of const buffers from the streambuf
                    const boost::asio::const_buffers_1& buffers = receiveBuffer.data();

                    // Check if the buffer is empty
                    if (buffers.size() == 0)
                    {
                        Log::info("Getting empty data");
                        continue;  // Skip processing and wait for more data
                    }

                    // Iterate over each const buffer and copy its data into the vector
                    for (const auto& buffer : buffers)
                    {
                        const uint8_t* bufferData = boost::asio::buffer_cast<const uint8_t*>(buffer);
                        std::size_t bufferSize = boost::asio::buffer_size(buffer);
                        data.insert(data.end(), bufferData, bufferData + bufferSize);
                    }

                    auto handledMessage = getMessageHandler().readMessage(clientAddress, data, clientPort, &Command::getAuthentication());

                    if (!handledMessage.getClient()->isBoostTCPSet())
                    {
                        handledMessage.getClient()->setBoostTCPSend([this, &newSocket](const std::vector<uint8_t>& bytes)
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

                    Command::Result passCommand = Command::read(handledMessage.getData(), *handledMessage.getClient(), *this, handledMessage.getMessageId());

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
            } })
                .detach();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

bool BoostTCPServer::sendToClient(const std::vector<uint8_t>& data, boost::asio::ip::tcp::socket& clientSocket)
{
    if (clientSocket.is_open())
    {
        boost::asio::write(clientSocket, boost::asio::buffer(data));
        return true;
    }
    return false;
}

} // namespace nexilis
