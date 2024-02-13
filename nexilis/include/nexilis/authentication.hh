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

    void setRootPassword(const std::string& password);

    bool isRootPassword(const std::string& password);

    void setCommonPassword(const std::string& password);

    bool isCommonPassword(const std::string& password);

    void setMode(Mode mode)
    {
        m_mode = mode;
    }

    Mode getMode() const
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
