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

    bool connectToServer();
    bool send(const std::string& data);
    bool receive(std::string& buffer);
    
    void start() override;
    void stop() override;

    Type getType() override
    {
        return Type::TCP;
    }

protected:    
    std::thread m_ioServiceThread;
    std::thread m_receiveThread;
private:
    void receiveLoop();

    bool m_stopped = false;
private:
    std::mutex m_socketMutex;
    ::boost::asio::io_service m_ioService;
    ::boost::asio::ip::tcp::socket m_socket;
    ::boost::asio::ip::tcp::resolver m_resolver;
    ::boost::asio::ip::tcp::resolver::iterator m_iterator;
};

} // namespace nexilis::boost

#endif