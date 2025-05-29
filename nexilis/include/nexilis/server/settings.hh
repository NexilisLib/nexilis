#ifndef NEXILIS_AUTHENTICATION_HH
#define NEXILIS_AUTHENTICATION_HH

#include <nexilis/server/authentication_mode.hh>

#include <cassert>
#include <string>

namespace nexilis::server
{

/// Nexilis Server-side API.
class Settings
{
public:
    // Normal password.
    void setPassphrase(const std::string& password);
    bool isPassphrase(const std::string& password);
    bool hasPassphrase();
    const std::string& getPassphrase() const
    {
        return m_password;
    }

    // Root password.
    void setRootPassword(const std::string& password);
    bool isRootPassword(const std::string& password);
    bool hasRootPassword();
    const std::string& getRootPassword() const
    {
        return m_rootPassword;
    }

    void setMode(AuthenticationMode mode)
    {
        m_mode = mode;
    }

    AuthenticationMode getMode() const
    {
        return m_mode;
    }

    // Tickrate
    void setTickrate(float tickrate);
    float getTickrate() const
    {
        return m_tickrate;
    }

private:
    std::string m_rootPassword;
    std::string m_password;
    AuthenticationMode m_mode = AuthenticationMode::empty;
    float m_tickrate = 60.f;
};

} // namespace nexilis::server

#endif
