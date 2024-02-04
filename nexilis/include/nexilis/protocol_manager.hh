#ifndef NEXILIS_CONNECTION_MANAGER_HH
#define NEXILIS_CONNECTION_MANAGER_HH

#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/af_unix/sock_dgram/unix_socket_server.hh>
#include <nexilis/protocol.hh>
#include <nexilis/nexilis_macros.hh>

#include <unordered_map>
#include <climits>

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

    class ProtocolData
    {
    public:
        ProtocolData(Protocol::Type type) :
            m_type(type),
            m_status(Status::connecting),
            m_id(Util::getRandomSizeT(0, NEXILIS_MAX))
        {
        }

        /// Copy constructor.
        ProtocolData(const ProtocolData& other) :
            m_type(other.m_type),
            m_status(other.m_status),
            m_id(other.m_id)
        {
        }

        /// Move constructor.
        ProtocolData(ProtocolData&& other) :
            m_type(std::move(other.m_type)),
            m_status(std::move(other.m_status)),
            m_id(std::move(m_id))
        {
        }

        /// Copy assignment operator.
        ProtocolData& operator=(const ProtocolData& other)
        {
            if (this != &other)
            {
                m_type = other.m_type;
                m_status = other.m_status;
                m_id = other.m_id;
            }
            return *this;
        }

        /// Move assignment operator.
        ProtocolData& operator=(ProtocolData&& other)
        {
            if (this != &other)
            {
                m_type = std::move(other.m_type);
                m_status = std::move(other.m_status);
                m_id = std::move(other.m_id);
            }
            return *this;
        }

    private:
        Protocol::Type m_type;
        Status m_status = Status::undefined;
        size_t m_id;
    };

    template <typename T, typename... Args>
    T createProtocol(Args&&... args)
    {
        static_assert(std::is_base_of<Protocol, T>::value,
                      "Type must be derived class of nexilis::Protocol");

        auto a = T(std::forward<Args>(args)...);
        m_items.emplace_back(ProtocolData(a.getType()));

        return std::move(a);
    }

private:
    std::vector<ProtocolData> m_items;
};

} // namespace nexilis

#endif
