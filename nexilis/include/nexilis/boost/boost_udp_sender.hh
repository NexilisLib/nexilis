#ifndef NEXILIS_BOOST_UDP_SENDER_HH
#define NEXILIS_BOOST_UDP_SENDER_HH

#include "../ports.hh"

#include "boost_io_context.hh"

#include <boost/asio.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>
#include <string>

namespace nexilis
{

class BoostUDPSender
{
public:
    BoostUDPSender(boost::asio::io_context& io_context, const std::string& ip)
        : m_target_endpoint(boost::asio::ip::address::from_string(ip), static_cast<unsigned short>(Port::UDP)),
          m_socket(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), static_cast<unsigned short>(Port::UDP)))
    {
    }

    void sendMessage(const std::string& message)
    {
        m_socket.send_to(boost::asio::buffer(message), m_target_endpoint);
    }

private:
    boost::asio::ip::udp::endpoint m_target_endpoint;
    boost::asio::ip::udp::socket m_socket;
};

} // namespace nexilis

#endif
