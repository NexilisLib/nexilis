#include <boost/asio.hpp>

#include <iostream>

#include <nexilis/command.hh>
#include <nexilis/core.hh>
#include <nexilis/context.hh>

class Server
{
public:

    /// Constructor.
    /// \param io_context io_context.
    /// \param port The port number.
    Server(boost::asio::io_context& io_context, unsigned short port) :
        m_UDP_socket(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
		m_TCP_socket(m_io_service)
    {
        startAccept();
        startUDPReceive();

        m_core.setPrint([](std::string text)
        {
            std::cout << text << std::endl;
        });
    }

private:

    void startUDPReceive()
    {
        m_UDP_socket.async_receive_from(
            boost::asio::buffer(m_UDP_buffer), m_remote_endpoint,
            [this](boost::system::error_code ec, size_t bytes_received)
            {
                if (!ec && bytes_received > 0)
                {
                    // Process the received UDP data.
                    nexilis::Command::read(m_UDP_buffer.data(), bytes_received);

                    sendMessage(m_remote_endpoint, "nii");

                    // Continue receiving UDP data.
                    startUDPReceive();
                }
            });
    }

	void startAccept()
	{

	}
	
	/*
    void startTCPReceive()
    {
        m_TCP_socket.async_receive_from(
            boost::asio::buffer(m_TCP_buffer), m_remote_endpoint,
            [this](boost::system::error_code ec, std::size_t bytes_received)
            {
                if (!ec && bytes_received > 0)
                {
                    // Process the received TCP data
                    std::string receivedTCPData(m_tcpBuffer.data(), bytes_received);
                    std::cout << "Received TCP data:" << receivedTCPData << std::endl;

                    // Perform TCP handshake validation here


                    // Send TCP handshakeResponse
                    std::string handshakeResponse = "handshake Response";
                    boost::asio::async_write(m_socket, boost::asio::buffer(handshakeResponse),
                            [this](boost::system::error_code ec, size_t )
                            {
                                if (!ec)
                                {
                                    std::cout << "TCP handshake response send." << std::endl;
                                }
                                else
                                {
                                    std::cerr << "TCP write error:" << ec.message() << std::endl;
                                }
                            });


                    // Process the received UDP data.
                    nexilis::Command::read(transfromCharPtr(m_buffer.data(), bytes_received));

                    sendMessage(m_remote_endpoint, "nii");

                    // Continue receiving UDP data.
                    startReceive();
                }
                else
                {
                    std::cerr << "ERROR:" << ec.message() << std::endl;
                }
            }
        );
    }

	*/

    void sendMessage(boost::asio::ip::udp::endpoint remote_endpoint, const std::string& message)
    {
        boost::asio::ip::udp::socket sendSocket(m_UDP_socket.get_executor());
        sendSocket.open(boost::asio::ip::udp::v4());

        sendSocket.async_send_to(boost::asio::buffer(message), remote_endpoint,
                                 [this, sendSocket = std::move(sendSocket)](boost::system::error_code /*ec*/,
                                                                             std::size_t /*bytesTransferred*/) {
                                   // Message sent
                                    std::cout << "message sent" << std::endl;
                                 });
    }

	//boost::asio::io_service& m_io_service;

    // TODO Proof of concept, this is going to be changed.
    std::array<char, 1024> m_UDP_buffer;
    boost::asio::ip::udp::socket m_UDP_socket;

   	std::array<char, 1024> m_TCP_buffer;
	boost::asio::ip::tcp::socket m_TCP_socket;


    boost::asio::ip::udp::endpoint m_remote_endpoint;

    nexilis::Core m_core;
    //nexilis::Context m_context;
};
