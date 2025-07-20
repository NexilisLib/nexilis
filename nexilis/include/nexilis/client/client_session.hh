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
