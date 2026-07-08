#ifndef NEXILIS_AUTH_CONFIG_HH
#define NEXILIS_AUTH_CONFIG_HH

#include <nexilis/server/authentication_mode.hh>

#include <string>

namespace nexilis
{

using server::AuthenticationMode;

class AuthConfig
{
public:
    AuthConfig() = default;
    AuthConfig(AuthConfig&& other);
    AuthConfig& operator=(AuthConfig&& other);
    AuthConfig(const AuthConfig& other);
    AuthConfig& operator=(const AuthConfig& other);

    AuthConfig(AuthenticationMode mode, std::string password)
        : m_mode(mode),
          m_password(std::move(password))
    {
    }

    AuthenticationMode getMode() const
    {
        return m_mode;
    }

    void setMode(AuthenticationMode mode)
    {
        m_mode = mode;
    }

    const std::string& getPassword() const
    {
        return m_password;
    }

    void setPassword(const std::string& password)
    {
        m_password = password;
    }

    bool isPassword(const std::string& password) const
    {
        return password == m_password;
    }

    bool hasPassword() const
    {
        return !m_password.empty();
    }

private:
    AuthenticationMode m_mode = AuthenticationMode::empty;
    std::string m_password;
};

} // namespace nexilis

#endif
