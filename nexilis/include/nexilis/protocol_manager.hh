#ifndef NEXILIS_CONNECTION_MANAGER_HH
#define NEXILIS_CONNECTION_MANAGER_HH

#include <nexilis/af_unix/unix_socket_server.hh>

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
        af_inet,
        af_unix
    };

    enum class Status
    {
        undefined,
        connecting,
        connected
    };

    template <typename Protocol, typename... Args>
    Protocol addConnection(Args&&... args)
    {
        Type type;
        if constexpr (std::is_same<Protocol, UnixSocketServer>::value)
        {
            type = Type::af_unix;
        }

        m_items.insert(std::pair<Type, Status>(type, Status::connecting));

        return Protocol(std::forward<Args>(args)...);
    }

private:

    std::unordered_map<Type, Status> m_items;
};

}

#endif
