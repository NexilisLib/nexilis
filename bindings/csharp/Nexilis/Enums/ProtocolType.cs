namespace Nexilis
{
    public enum ProtocolType
    {
        UNKNOWN = 0,

        AF_INET_UDP_SERVER = 1,
        AF_INET_UDP_CLIENT = 2,
        AF_INET_TCP_SERVER = 3,
        AF_INET_TCP_CLIENT = 4,

        BOOST_UDP_SERVER = 5,
        BOOST_UDP_CLIENT = 6,
        BOOST_TCP_SERVER = 7,
        BOOST_TCP_CLIENT = 8,

        AF_UNIX_SOCK_DGRAM_CLIENT = 9,
        AF_UNIX_SOCK_DGRAM_SERVER = 10,
        AF_UNIX_SOCK_STREAM_CLIENT = 11,
        AF_UNIX_SOCK_STREAM_SERVER = 12,
    }
}
