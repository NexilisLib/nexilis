#ifndef NEXILIS_CLIENT_CREATE_CLIENT_CONFIG_HH
#define NEXILIS_CLIENT_CREATE_CLIENT_CONFIG_HH

#include <nexilis/client/client_config.hh>

namespace nexilis::client
{

ClientConfig createClientConfig(const std::string& ip_address, const std::string& password);

}

#endif
