#include <nexilis/protocol.hh>

namespace nexilis
{

std::string Protocol::typeToString(Type type)
{
    switch (type)
    {
        case Type::AF_INET_UDP_SERVER: return "af_inet::UDPServer";
        case Type::AF_INET_UDP_CLIENT: return "af_inet::UDPClient";
        case Type::AF_INET_TCP_SERVER: return "af_inet::TCPServer";
        case Type::AF_INET_TCP_CLIENT: return "af_inet::TCPClient";

        case Type::BOOST_UDP_SERVER: return "boost::UDPServer";
        case Type::BOOST_UDP_CLIENT: return "boost::UDPClient";
        case Type::BOOST_TCP_SERVER: return "boost::TCPServer";
        case Type::BOOST_TCP_CLIENT: return "boost::TCPClient";

        case Type::AF_UNIX_SOCK_DGRAM_CLIENT: return "af_unix::sock_dgram::Client";
        case Type::AF_UNIX_SOCK_DGRAM_SERVER: return "af_unix::sock_dgram::Server";
        case Type::AF_UNIX_SOCK_STREAM_CLIENT: return "af_unix::sock_stream::Client";
        case Type::AF_UNIX_SOCK_STREAM_SERVER: return "af_unix::sock_stream::Server";

        default: return "UNDEFINED";
    }
}

}
