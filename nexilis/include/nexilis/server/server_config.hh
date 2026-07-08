#ifndef NEXILIS_SERVER_CONFIG_HH
#define NEXILIS_SERVER_CONFIG_HH

#include <nexilis/auth_config.hh>

#include <cassert>
#include <string>

namespace nexilis::server
{

class ServerConfig : public AuthConfig
{
public:
    ServerConfig() = default;

    using AuthConfig::AuthConfig;

    // Normal password.
    void setPassphrase(const std::string& password)
    {
        setPassword(password);
    }
    bool isPassphrase(const std::string& password)
    {
        return AuthConfig::isPassword(password);
    }
    bool hasPassphrase()
    {
        return AuthConfig::hasPassword();
    }
    const std::string& getPassphrase() const
    {
        return getPassword();
    }

    // Root password.
    void setRootPassword(const std::string& password);
    bool isRootPassword(const std::string& password);
    bool hasRootPassword();
    const std::string& getRootPassword() const
    {
        return m_rootPassword;
    }

    // Tickrate
    void setTickrate(float tickrate);
    float getTickrate() const
    {
        return m_tickrate;
    }

private:
    std::string m_rootPassword;
    float m_tickrate = 60.f;
};

} // namespace nexilis::server

#endif
