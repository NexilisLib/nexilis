#ifndef NEXILIS_AUTHENTICATION_HH
#define NEXILIS_AUTHENTICATION_HH

#include <string>
#include <cassert>

namespace nexilis
{

class Authentication
{
public:
    enum class Mode
    {
        free,
        passwordProtected,
        whiteListed
    };

    void setRootPassword(const std::string& password)
    {
        assert(!password.empty());
        m_rootPassword = password;
    }

    bool isRootPassword(const std::string& password)
    {
        assert(!m_rootPassword.empty());
        return m_rootPassword == password;
    }

    void setCommonPassword(const std::string& password)
    {
        assert(!password.empty());
        m_password = password;
    }

    bool isCommonPassword(const std::string& password)
    {
        assert(!m_password.empty());
        return password == m_password;
    }

    void setMode(Mode mode)
    {
        m_mode = mode;
    }

    Mode getMode()
    {
        return m_mode;
    }

private:
    std::string m_rootPassword;
    std::string m_password;
    Mode m_mode = Mode::free;
};

}

#endif
