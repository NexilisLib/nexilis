#include <boost/asio.hpp>

#include <iostream>

#include <nexilis/command.hh>

class UDPServer
{
public:

    /// Constructor.
    /// \param io_context io_context.
    /// \param port The port number.
    UDPServer(boost::asio::io_context& io_context, short port) :
        m_socket(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
    {
        startReceive();
    }

private:

    void startReceive()
    {
        m_socket.async_receive_from(
            boost::asio::buffer(m_buffer), m_remote_endpoint,
            [this](boost::system::error_code ec, std::size_t bytes_received)
            {
                if (!ec && bytes_received > 0)
                {
                    // Process the received UDP data.
                    nexilis::Command::read(transfromCharPtr(m_buffer.data(), bytes_received));

                    // Continue receiving UDP data.
                    startReceive();
                }
            }
        );
    }

    std::vector<unsigned char> transfromCharPtr(const char* input, size_t lenght)
    {
        // Create a vector and reserve space for the character.
        std::vector<unsigned char> result;
        result.reserve(lenght);

        for(size_t i = 0; i < lenght; i++)
        {
            result.emplace_back(static_cast<unsigned char>(input[i]));
        }

        return result;
    }

    boost::asio::ip::udp::socket m_socket;
    boost::asio::ip::udp::endpoint m_remote_endpoint;

    // TODO Proof of concept, this is going to be changed.
    std::array<char, 1024> m_buffer;
};
