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
    enum class Status
    {
        undefined,
        unconnected,
        connecting,
        connected
    };

    template <typename T, typename... Args>
    T createProtocol(Args&&... args)
    {
        static_assert(std::is_base_of<Protocol, T>::value,
                      "Type must be derived class of nexilis::Protocol");

        m_index++;
        //auto a = T::Type;
        m_items.insert(std::pair<size_t, Status>(m_index, Status::connecting));
        return T(std::forward<Args>(args)...);
    }

private:
    std::unordered_map<size_t, Status> m_items;

    size_t m_index;
};

} // namespace nexilis

#endif
