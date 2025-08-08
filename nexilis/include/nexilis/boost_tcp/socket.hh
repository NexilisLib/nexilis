#ifndef NEXILIS_BOOST_TCP_SOCKET_HH
#define NEXILIS_BOOST_TCP_SOCKET_HH

#include <nexilis/nx_class.hh>
#include <nexilis/nx_data.hh>

#include <boost/asio/ip/tcp.hpp>

namespace nexilis::boost_tcp
{

class Socket : public NxClass
{
public:
    /// Server-side constructor.
    explicit Socket(boost::asio::io_context& io) noexcept;

    /// Client-side constructor.
    explicit Socket(boost::asio::io_context& io, const std::string& host, uint16_t port) noexcept;

    /// Move constructor.
    Socket(Socket&& other) noexcept;

    /// Move assigment operator.
    Socket& operator=(Socket&& other) noexcept;

    /// Connect for both the server and the client.
    bool connect(const std::string& host, uint16_t port);

    /// Server-side acceptance.
    void assign(boost::asio::ip::tcp::socket&& socket);

    bool send(const nx_data& data);
    bool receive(nx_data& data, size_t timeout_ms = 100);
    void close();
    void forceClose();

    bool isOpen() const;
    void cancelAllSocketOperations();

    std::string getRemoteAddress() const;
    std::string getEndpointId() const;

private:
    void setSocketOptions();

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> m_socket;
    mutable std::mutex m_mutex;
    std::atomic<bool> m_connected{false};
    std::string m_endpointId;
};

} // namespace nexilis::boost_tcp

#endif
