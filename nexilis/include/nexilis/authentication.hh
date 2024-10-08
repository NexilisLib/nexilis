#ifndef NEXILIS_AUTHENTICATION_HH
#define NEXILIS_AUTHENTICATION_HH

#include <cassert>
#include <string>

namespace nexilis
{

/// Note:
// 1. Luokan nimi?
// 2. Luokan sisältö verrattuna nimeen?
//


/// Nexilis Server-side API.
class Authentication
{
public:
    // Normal password.
    void setPassphrase(const std::string& password);
    bool isPassphrase(const std::string& password);
    bool hasPassphrase();

    // Root password.
    void setRootPassword(const std::string& password);
    bool isRootPassword(const std::string& password);
    bool hasRootPassword();

    enum class AuthenticationMode
    {
        free,
        passwordProtected,
        whiteListed
    };

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
    float getTickrate() const { return m_tickrate; }

private:
    std::string m_rootPassword;
    std::string m_password;
    AuthenticationMode m_mode = AuthenticationMode::free;
    float m_tickrate = 60.f;
};

} // namespace nexilis

#endif
