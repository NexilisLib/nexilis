#ifndef NEXILIS_BOOST_TCP_CLIENT_HH
#define NEXILIS_BOOST_TCP_CLIENT_HH

#include <nexilis/client/client_protocol.hh>
#include <nexilis/nx_class.hh>
#include <nexilis/protocol.hh>
#include <nexilis/protocol_status.hh>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <thread>

namespace nexilis::client::nxboost
{

class TCPClient : public virtual NxClass,
                  public Protocol,
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

    /// ClientProtocol::sendMessage(const nx_data&, const std::function<void()>&) implementation.
    void sendMessage(const nx_data& message, const std::function<void()>& callback) override;

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
    std::thread m_portSwitchingThread;

private:
    void receiveLoop();
    bool connectToServer();
    bool send(const nx_data& data);
    void receive(const std::function<void(nx_data)>& buffer);
    void handlePortSwitch();

private:
    std::unique_ptr<std::atomic<ProtocolStatus>> m_protocolStatus;
    std::unique_ptr<std::atomic<bool>> m_stopped;
    std::shared_ptr<boost::asio::io_context> m_ioContext;
    std::unique_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> m_workGuard;
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::resolver m_resolver;

    std::shared_ptr<std::mutex> m_sendMutex;
    std::shared_ptr<std::mutex> m_receiveMutex;
    std::shared_ptr<std::mutex> m_portSwitchingMutex;
    std::unique_ptr<std::atomic<bool>> m_portSwitchingInProgress;
    std::shared_ptr<std::condition_variable> m_portSwitchCV;
    uint16_t m_serverPort;
};

} // namespace nexilis::client::nxboost

#endif
