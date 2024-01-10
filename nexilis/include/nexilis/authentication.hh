#ifndef NEXILIS_AUTHENTICATION_HH
#define NEXILIS_AUTHENTICATION_HH

#include <string>
#include <cassert>

class Authentication
{
public:
    void setPassword(const std::string& password)
    {
        assert(password != "");
        m_password = password;
    }

    bool checkPassword(const std::string& password)
    {
        assert(m_password != "");
        return m_password == password;
    }

private:
    std::string m_password = "";
};

#endif
