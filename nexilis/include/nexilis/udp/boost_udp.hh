#ifndef NEXILIS_UPD_BOOST_UDP_HH
#define NEXILIS_UPD_BOOST_UDP_HH

#include <boost/asio.hpp>

class BoostUDP
{
public:
    BoostUDP(unsigned short port) :
        m_socket(m_io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
        m_port(port)
    {
    }

    void start()
    {
        m_io_context.run();
    }

private:
    boost::asio::io_context m_io_context;
    boost::asio::ip::udp::socket m_socket;
    unsigned short m_port;

};

#endif
