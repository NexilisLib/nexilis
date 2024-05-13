#ifndef NEXILIS_BOOST_TCP_CLIENT_HH
#define NEXILIS_BOOST_TCP_CLIENT_HH

#include <nexilis/client_protocol.hh>
#include <nexilis/loggable.hh>
#include <nexilis/protocol.hh>

#include <boost/asio.hpp>
#include <boost/json.hpp>

namespace nexilis
{

class BoostTCPClient : public Protocol,
                  public ClientProtocol
{
public:
    /// Constructor.
    BoostTCPClient(ClientAPI& api);

    /// Destructor.
    ~BoostTCPClient();

    /// Move constructor.
    BoostTCPClient(BoostTCPClient&& other);

    /// Move assignment operator.
    BoostTCPClient& operator=(BoostTCPClient&& other);

    /// Deleted copy constructor.
    BoostTCPClient(const BoostTCPClient& other) = delete;

    /// Deleted copy assignment operator.
    BoostTCPClient& operator=(const BoostTCPClient& other) = delete;

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
    bool send(const std::vector<uint8_t>& data);
    bool receive(std::vector<uint8_t>& buffer);

private:
    bool m_stopped = false;
    std::unique_ptr<boost::asio::io_context> m_ioContext;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::resolver m_resolver;
    boost::asio::ip::tcp::resolver::iterator m_iterator;

    std::unique_ptr<std::mutex> m_mutex;
};

} // namespace nexilis

#endif
