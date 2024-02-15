#ifndef NEXILIS_BOOST_TCP_CLIENT_HH
#define NEXILIS_BOOST_TCP_CLIENT_HH

#include <nexilis/protocol.hh>
#include <nexilis/client_protocol.hh>
#include <nexilis/client_api.hh>

#include <boost/asio.hpp>

namespace nexilis::boost
{

class TCPClient : public Protocol, public ClientProtocol
{
public:
    /// Constructor.
    TCPClient(ClientAPI& api);

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

    /// ClientProtocol::sendMessage(const std::string&) implementation.
    void sendMessage(const std::string& message) override;

    /// ClientProtocol::sendMessage(const std::vector<uint8_t>&) implementation.
    void sendMessage(const std::vector<uint8_t>& message) override;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::BOOST_TCP_CLIENT;
    }

protected:    
    std::thread m_ioContextThread;
    std::thread m_receiveThread;
private:
    void receiveLoop();
    bool connectToServer();
    bool send(const std::string& data);
    bool receive(std::string& buffer);

    bool m_stopped = false;
private:
    std::unique_ptr<::boost::asio::io_context> m_ioContext;
    ::boost::asio::ip::tcp::socket m_socket;
    ::boost::asio::ip::tcp::resolver m_resolver;
    ::boost::asio::ip::tcp::resolver::iterator m_iterator;

    std::unique_ptr<std::mutex> m_mutex;

    ClientAPI m_api;
};

} // namespace nexilis::boost

#endif