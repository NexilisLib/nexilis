#ifndef NEXILIS_UDP_SERVER_HH
#define NEXILIS_UDP_SERVER_HH

#include <boost/asio.hpp>

#include <iostream>

#include <nexilis/command.hh>

class UDPServer
{
public:
    UDPServer(const boost::asio::io_context& io_context, unsigned short port)
        : m_socket(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
    {
        startAccept();
        startReceive(m_socket.remote_endpoint());
    }

    void startAccept()
    {
    }

    void startReceive(boost::asio::ip::udp::endpoint remote_endpoint)
    {
        m_socket.async_receive_from(
            boost::asio::buffer(m_buffer), remote_endpoint,
            [this, &remote_endpoint](boost::system::error_code ec, size_t bytes_received)
            {
                if (!ec && bytes_received > 0)
                {
                    // Process the received UDP data.
                    nexilis::Command::read(m_buffer.data(), bytes_received);

                    // sendMessage(remote_endpoint, "nii");

                    // Continue receiving UDP data.
                    startReceive(remote_endpoint);
                }
            });
    }

    void sendMessage(boost::asio::ip::udp::endpoint remote_endpoint, const std::string& message)
    {
        boost::asio::ip::udp::socket sendSocket(m_socket.get_executor());
        sendSocket.open(boost::asio::ip::udp::v4());

        sendSocket.async_send_to(boost::asio::buffer(message), remote_endpoint,
                                 [this, sendSocket = std::move(sendSocket)](boost::system::error_code /*ec*/,
                                                                            size_t /*bytesTransferred*/)
                                 {
                                     // Message sent
                                     std::cout << "message sent" << std::endl;
                                 });
    }

private:
    std::array<char, 1024> m_buffer;
    boost::asio::ip::udp::socket m_socket;
};

#endif
