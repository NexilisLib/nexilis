#ifndef NEXILIS_BOOST_TCP_CLIENT_HH
#define NEXILIS_BOOST_TCP_CLIENT_HH

#include <nexilis/protocol.hh>

#include <boost/asio.hpp>

namespace nexilis::boost
{

class TCPClient : public Protocol
{
public:
    /// Constructor.
    TCPClient(const std::string& serverIP, const std::string& serverPort);

    /// Destructor.
    ~TCPClient();

    /// Move constructor.
    TCPClient(TCPClient&& other);

    /// Move assignment operator.
    TCPClient& operator=(TCPClient other);

    TCPClient(const TCPClient& other) = delete;
    TCPClient& operator=(const TCPClient& other) = delete;

    bool connectToServer();
    bool send(const std::string& data);
    bool receive(std::string& buffer);
    
    void start() override;
    void stop() override;

    Type getType() override
    {
        return Type::BOOST_TCP_CLIENT;
    }

protected:    
    std::thread m_ioContextThread;
    std::thread m_receiveThread;
private:
    void receiveLoop();

    bool m_stopped = false;
private:
    ::boost::asio::io_context m_ioContext;
    ::boost::asio::ip::tcp::socket m_socket;
    ::boost::asio::ip::tcp::resolver m_resolver;
    ::boost::asio::ip::tcp::resolver::iterator m_iterator;

    std::mutex m_mutexLock;
};

} // namespace nexilis::boost

#endif