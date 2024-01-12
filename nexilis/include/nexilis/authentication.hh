#ifndef NEXILIS_AUTHENTICATION_HH
#define NEXILIS_AUTHENTICATION_HH

#include <string>
#include <cassert>

namespace nexilis
{

class Authentication
{
public:
    void setRootPassword(const std::string& password)
    {
        assert(password != "");
        m_rootPassword = password;
    }

    bool checkRootPassword(const std::string& password)
    {
        assert(m_rootPassword != "");
        return m_rootPassword == password;
    }

    void setCommonPassword(const std::string& password)
    {
        assert(password != "");
        m_password = password;
    }

    bool checkCommonPassword(const std::string& password)
    {
        assert(m_rootPassword != "");
        return password == m_password;
    }

private:
    std::string m_rootPassword = "";
    std::string m_password = "";
};

}

#endif
