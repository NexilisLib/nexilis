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
    /// \param id The server side id of the client.
    /// \param client_api Pointer to the client api.
    ClientSession(uint64_t id, ClientAPI* client_api);

    /// Virtual destructor.
    virtual ~ClientSession() = default;

    /// Copy constructor.
    ClientSession(const ClientSession& other)
        : BaseClient(other),
          m_clientAPI(other.m_clientAPI)
    {
    }

    /// Move constructor.
    ClientSession(ClientSession&& other) noexcept;

    /// Move assignment operator.
    ClientSession& operator=(ClientSession&& other) noexcept;

    /// Comparison operator overload.
    friend bool operator==(const ClientSession& lhs, const ClientSession& rhs);

    /// Non-comparison operator overload.
    friend bool operator!=(const ClientSession& lhs, const ClientSession& rhs)
    {
        return !(lhs == rhs);
    }

    void setUsername(const std::string& newUsername)
    {
        BaseClient::setBaseUsername(newUsername);
    }

    void setPosition3D(float x, float y, float z);
    Vector3<float> getPosition3D();

    // TODO setter and getter for 2D position.
private:
    /// ClientAPI instance what the client is using.
    ClientAPI* const m_clientAPI = nullptr;
};

} // namespace nexilis::client

#endif
