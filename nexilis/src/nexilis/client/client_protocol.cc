#include <nexilis/client/client_protocol.hh>
#include <nexilis/client/packet.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/util.hh>

namespace nexilis::client
{

ClientProtocol::ClientProtocol(ClientAPI* api)
    : NxClass("ClientProtocol"),
      m_api(api),
      m_protocolStatus(std::make_unique<std::atomic<ProtocolStatus>>(ProtocolStatus::undefined))
{
}

ClientProtocol::ClientProtocol(ClientProtocol&& other)
    : NxClass(std::move(other)),
      m_api(other.m_api),
      m_protocolStatus(std::move(other.m_protocolStatus))
{
    other.m_api = nullptr;
    other.m_protocolStatus.reset();
}

ClientProtocol& ClientProtocol::operator=(ClientProtocol&& other)
{
    if (this != &other)
    {
        m_api = other.m_api;
        m_protocolStatus = std::move(other.m_protocolStatus);
        other.m_api = nullptr;
        other.m_protocolStatus.reset();
        NxClass::operator=(std::move(other));
    }
    return *this;
}

ProtocolStatus ClientProtocol::getProtocolStatus() const
{
    return m_protocolStatus.get()->load();
}

void ClientProtocol::updateProtocolStatus(ProtocolStatus status)
{
    if (m_protocolStatus)
    {
        m_protocolStatus->store(status);
    }
}

void ClientProtocol::sendMessageWithCallback(const nx_data& message, const std::function<void()>& callback)
{
    m_api->addCallback(ClientProtocol::createCallback(message, callback));
    sendMessage(message);
}

std::pair<uint64_t, std::function<void()>> ClientProtocol::createCallback(const nx_data& message, const std::function<void()>& callback)
{
    // Vector without client id (8 bytes).
    auto messageWithoutClientId = Util::removeAmountOfBytesFromVector(message, 8);

    // Next eight bytes is the message id.
    uint64_t messageId = Util::uint64FromFront(messageWithoutClientId);

    return std::make_pair(messageId, callback);
}

void ClientProtocol::start(Protocol::Type type)
{
    if (m_api->isInitialized())
    {
        Log::error(header(), "Password already sent using another protocol!");
        return;
    }

    auto password = m_api->getClientPassword();
    Log::debug(header(), "Trying server password: ", password);

    // TODO We're assuming here that the server is password protected.
    auto message = Util::convertToByteVector(password.c_str(), password.size());
    sendMessage(message);

    Log::debug(header(), "Client protocol type: ", Protocol::typeToString(type));
    switch (type)
    {
        case Protocol::Type::BOOST_TCP_CLIENT:
            Log::debug(header(), "Waiting for Boost TCP to be ready");
            getClientAPI()->waitUntilBoostTCPReady();
            Log::debug(header(), "Boost TCP is ready");
            break;
        case Protocol::Type::BOOST_UDP_CLIENT:
            Log::debug(header(), "Waiting for Boost UDP to be ready");
            getClientAPI()->waitUntilBoostUDPReady();
            Log::debug(header(), "Boost UDP is ready");
            break;
        case Protocol::Type::AF_INET_TCP_CLIENT:
            getClientAPI()->waitUntilInetTCPReady();
            break;
        case Protocol::Type::AF_INET_UDP_CLIENT:
            getClientAPI()->waitUntilInetUDPReady();
            break;
        case Protocol::Type::AF_UNIX_SOCK_STREAM_CLIENT:
            getClientAPI()->waitUntilUnixStreamReady();
            break;
        case Protocol::Type::AF_UNIX_SOCK_DGRAM_CLIENT:
            getClientAPI()->waitUntilUnixDgramReady();
            break;

        case Protocol::Type::AF_INET_TCP_SERVER:
        case Protocol::Type::AF_INET_UDP_SERVER:
        case Protocol::Type::AF_UNIX_SOCK_DGRAM_SERVER:
        case Protocol::Type::AF_UNIX_SOCK_STREAM_SERVER:
        case Protocol::Type::BOOST_TCP_SERVER:
        case Protocol::Type::BOOST_UDP_SERVER:
            Log::error(header(), "This function cannot be called via server protocol");
            return;

        case Protocol::Type::UNKNOWN:
            Log::error(header(), "This function called via unknown protocol");
            return;
    }
    // sendMessage(nexilis::client::Packet::Set::username(username));
}

} // namespace nexilis::client
