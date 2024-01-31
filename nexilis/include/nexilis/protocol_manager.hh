#ifndef NEXILIS_CONNECTION_MANAGER_HH
#define NEXILIS_CONNECTION_MANAGER_HH

#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/af_unix/sock_dgram/unix_socket_server.hh>
#include <nexilis/protocol.hh>

#include <unordered_map>

namespace nexilis
{

class ProtocolManager
{
public:
    enum class Type
    {
        websocket,
        boost_udp,
        af_inet_server,
        af_inet_client,
        af_unix
    };

    // TODO not implemented
    enum class Status
    {
        undefined,
        connecting,
        connected
    };

    template <typename T, typename... Args>
    T createProtocol(Args&&... args)
    {
        static_assert(std::is_base_of<Protocol, T>::value,
                      "Type must be derived class of nexilis::Protocol");

        Type type;
        if constexpr (std::is_same<T, af_unix::UnixSocketServer>::value)
        {
            type = Type::af_unix;
        }
        else if constexpr (std::is_same<T, af_inet::UDPServer>::value)
        {
            type = Type::af_inet_server;
        }
        else if constexpr (std::is_same<T, af_inet::UDPClient>::value)
        {
            type = Type::af_inet_client;
        }
        m_items.insert(std::pair<Type, Status>(type, Status::connecting));

        return T(std::forward<Args>(args)...);
    }

private:
    std::unordered_map<Type, Status> m_items;
};

} // namespace nexilis

#endif
