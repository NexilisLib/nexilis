#ifndef NEXILIS_CONNECTION_MANAGER_HH
#define NEXILIS_CONNECTION_MANAGER_HH

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

    class ProtocolData
    {
    public:
        /// Constructor.
        ProtocolData(Protocol::Type type);

        /// Copy constructor.
        ProtocolData(const ProtocolData& other);

        /// Move constructor.
        ProtocolData(ProtocolData&& other);

        /// Copy assignment operator.
        ProtocolData& operator=(const ProtocolData& other);

        /// Move assignment operator.
        ProtocolData& operator=(ProtocolData&& other);

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

        // This is little hacky. I'd much rather prefer if this was std::move call.
        // However this would require that the parameters cannot be references.
        // So this is technically always move call and it works and the api is nice.
        //
        // It's also important to note that this gives compile-time error
        // if the construction fails.
        auto protocol = T(std::forward<Args>(args)...);
        m_items.emplace_back(ProtocolData(protocol.getType()));

        return protocol;
    }

private:
    std::vector<ProtocolData> m_items;
};

} // namespace nexilis

#endif
