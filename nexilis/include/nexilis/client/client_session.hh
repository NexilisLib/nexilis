#ifndef NEXILIS_CLIENT_CLIENT_SESSION_HH
#define NEXILIS_CLIENT_CLIENT_SESSION_HH

#include <nexilis/base_client.hh>

namespace nexilis::client
{

class ClientAPI;

class ClientSession : public BaseClient
{
public:
    /// Constuctor.
    ClientSession(uint64_t id, ClientAPI* clientAPI);

    /// Virtual destructor.
    virtual ~ClientSession() = default;

    /// Move constructor.
    ClientSession(ClientSession&& other);

    /// Move assignment operator.
    ClientSession& operator=(ClientSession&& other);

    /// Deleted copy constructor.
    ClientSession(const ClientSession& other) = delete;

    /// Deleted copy assignment operator.
    ClientSession& operator=(const ClientSession& other) = delete;

    /// Comparison operator overload.
    friend bool operator==(const ClientSession& lhs, const ClientSession& rhs);

    /// Non-comparison operator overload.
    friend bool operator!=(const ClientSession& lhs, const ClientSession& rhs)
    {
        return !(lhs == rhs);
    }

    void setUsername(const std::string& username)
    {
        BaseClient::setUsername(username);
    }

private:
    /// ClientAPI instance what the client is using.
    ClientAPI* const m_clientAPI = nullptr;
};

} // namespace nexilis::client

#endif
