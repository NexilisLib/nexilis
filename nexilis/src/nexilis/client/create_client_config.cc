#include <nexilis/client/create_client_config.hh>

namespace nexilis::client
{

ClientConfig createClientConfig(const std::string& ip_address, const std::string& password)
{
    ClientConfig config;

    // We use these protocols by default.
    config.setBoostTCPAddress(ip_address);
    config.setBoostUDPAddress(ip_address);

    // We use password protection by default.
    config.setMode(nexilis::server::AuthenticationMode::password_protected);
    config.setPassword(password);

    return config;
}

} // namespace nexilis::client
