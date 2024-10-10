#include <nexilis/client/client_protocol.hh>
#include <nexilis/util.hh>

namespace nexilis::client
{

ClientProtocol::ClientProtocol(ClientAPI* api)
    : m_api(api)
{
}

ClientProtocol::ClientProtocol(ClientProtocol&& other)
    : m_api(other.m_api)
{
    other.m_api = nullptr;
}

ClientProtocol& ClientProtocol::operator=(ClientProtocol&& other)
{
    if (this != &other)
    {
        m_api = other.m_api;
        other.m_api = nullptr;
    }
    return *this;
}

std::pair<uint64_t, std::function<void()>> ClientProtocol::createCallback(const nx_data& message, const std::function<void()>& callback)
{
    // Vector without client id (8 bytes).
    auto messageWithoutClientId = Util::removeAmountOfBytesFromVector(message, 8);

    // Next eight bytes is the message id.
    uint64_t messageId = Util::uint64FromFront(messageWithoutClientId);

    return std::make_pair(messageId, callback);
}

} // namespace nexilis::client
