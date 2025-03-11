#ifndef NEXILIS_BOOST_TCP_CLIENT_HH
#define NEXILIS_BOOST_TCP_CLIENT_HH

#include <nexilis/client/client_protocol.hh>
#include <nexilis/logger/loggable.hh>
#include <nexilis/protocol.hh>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <thread>

namespace nexilis::client::nxboost
{

class TCPClient : public Protocol,
                  public ClientProtocol
{
public:
    /// Constructor.
    explicit TCPClient(ClientAPI& api);

    /// Destructor.
    ~TCPClient();

    /// Move constructor.
    TCPClient(TCPClient&& other);

    /// Move assignment operator.
    TCPClient& operator=(TCPClient&& other);

    /// Deleted copy constructor.
    TCPClient(const TCPClient& other) = delete;

    /// Deleted copy assignment operator.
    TCPClient& operator=(const TCPClient& other) = delete;

    /// ClientProtocol::sendMessage(const nx_data&) implementation.
    void sendMessage(const nx_data& message) override;

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::BOOST_TCP_CLIENT;
    }

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

protected:
    std::thread m_ioContextThread;
    std::thread m_receiveThread;

private:
    void receiveLoop();
    bool connectToServer();
    bool send(const nx_data& data);
    bool receive(nx_data& buffer);

private:
    bool m_stopped = false;
    std::unique_ptr<boost::asio::io_context> m_ioContext;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::resolver m_resolver;
    std::unique_ptr<std::mutex> m_mutex;
};

} // namespace nexilis::client::nxboost

#endif
