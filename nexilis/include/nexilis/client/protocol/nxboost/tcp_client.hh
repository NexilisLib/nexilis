#ifndef NEXILIS_BOOST_TCP_CLIENT_HH
#define NEXILIS_BOOST_TCP_CLIENT_HH

#include <nexilis/client/client_protocol.hh>
#include <nexilis/nx_class.hh>
#include <nexilis/protocol.hh>
#include <nexilis/tls.hh>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/smart_ptr/atomic_shared_ptr.hpp>

#include <thread>

namespace nexilis::client::nxboost
{

class TCPClient : public virtual NxClass,
                  public Protocol,
                  public ClientProtocol
{
public:
    /// TLS transport used for the TCP connections (TLS-PSK when enabled).
    using TlsSocket = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;

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

    /// ClientProtocol::sendMessageAsync(const nx_data&) implementation.
    std::future<void> sendMessageAsync(const nx_data& message) override;

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

private:
    bool connectToServer();
    bool connectToMainPort();
    bool connectToSwitchedPort(uint16_t port);
    bool send(const nx_data& data);
    void startAsyncRead();
    void doAsyncRead(std::shared_ptr<TlsSocket> socket,
                     std::shared_ptr<boost::asio::streambuf> buffer);
    void handleAsyncReadError(const boost::system::error_code& ec);
    void initiatePortSwitch(uint16_t port);

    /// Builds the TLS context from the ClientAPI configuration.
    static std::shared_ptr<boost::asio::ssl::context> createTlsContext(ClientAPI& api);

    // Thread-safe socket access
    std::shared_ptr<TlsSocket> loadSocket() const;
    void storeSocket(std::shared_ptr<TlsSocket> socket);
    std::shared_ptr<TlsSocket> exchangeSocket(
            std::shared_ptr<TlsSocket> socket);

private:
    std::unique_ptr<std::atomic<bool>> m_stopped;
    std::shared_ptr<boost::asio::io_context> m_ioContext;
    std::shared_ptr<boost::asio::io_context::strand> m_strand;
    std::unique_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> m_workGuard;

    std::shared_ptr<boost::asio::ssl::context> m_tlsContext;
    std::shared_ptr<TlsSocket> m_mainSocket;
    std::shared_ptr<TlsSocket> m_switchedSocket;
    boost::atomic_shared_ptr<TlsSocket> m_activeSocket;
    boost::asio::ip::tcp::resolver m_resolver;

    std::shared_ptr<std::mutex> m_sendMutex;
    std::shared_ptr<std::mutex> m_receiveMutex;
    std::shared_ptr<std::mutex> m_portSwitchingMutex;

    uint16_t m_mainPort;
    uint16_t m_switchedPort;
    std::atomic<bool> m_useSwitchedPort;

    struct PendingSend
    {
        uint64_t messageId = 0;
        std::shared_ptr<std::promise<void>> promise = nullptr;
    };

    std::unordered_map<uint64_t, std::shared_ptr<std::promise<void>>> m_pendingSends;
    std::shared_ptr<std::mutex> m_pendingSendsMutex;
};

} // namespace nexilis::client::nxboost

#endif
