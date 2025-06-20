#ifndef NEXILIS_SERVER_AUTHETICATION_MODE_HH
#define NEXILIS_SERVER_AUTHETICATION_MODE_HH

namespace nexilis::server
{

enum class AuthenticationMode
{
    empty,
    skip,
    password_protected,
    admin_access,
    root_access
};

}

#endif
