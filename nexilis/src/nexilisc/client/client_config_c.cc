#include <nexilisc/client/client_config_c.h>

nexilis_ClientConfigC* nexilis_client_config_create()
{
    auto config = new nexilis_ClientConfigC();
    auto cpp_data = new nexilis::client::ClientConfig();
    config->data = cpp_data;
    return config;
}

void nexilis_client_config_destroy(nexilis_ClientConfigC* config)
{
    if (config)
    {
        if (config->data)
        {
            delete config->data;
            config->data = nullptr;
        }
        delete config;
    }
}

void nexilis_client_config_set_password(nexilis_ClientConfigC* config, const char* password)
{
    if (config && config->data && password)
    {
        config->data->setPassword(password);
    }
}

const char* nexilis_client_config_get_password(const nexilis_ClientConfigC* config)
{
    if (config && config->data)
    {
        std::string password = config->data->getPassword();
        char* cstr = (char*)malloc(password.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, password.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_client_config_set_mode(nexilis_ClientConfigC* config, int mode)
{
    if (config && config->data)
    {
        config->data->setMode(static_cast<nexilis::server::AuthenticationMode>(mode));
    }
}

void nexilis_client_config_set_inet_udp(nexilis_ClientConfigC* config, const char* server_address)
{
    if (config && config->data && server_address)
    {
        config->data->setInetUDP(server_address);
    }
}

const char* nexilis_client_config_get_inet_udp_server_address(const nexilis_ClientConfigC* config)
{
    if (config && config->data)
    {
        std::string address = config->data->getInetUDPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_client_config_set_inet_tcp(nexilis_ClientConfigC* config, const char* server_address)
{
    if (config && config->data && server_address)
    {
        config->data->setInetTCP(server_address);
    }
}

const char* nexilis_client_config_get_inet_tcp_server_address(const nexilis_ClientConfigC* config)
{
    if (config && config->data)
    {
        std::string address = config->data->getInetTCPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_client_config_set_boost_tcp_address(nexilis_ClientConfigC* config, const char* server_address)
{
    if (config && config->data && server_address)
    {
        config->data->setBoostTCPAddress(server_address);
    }
}

const char* nexilis_client_config_get_boost_tcp_server_address(const nexilis_ClientConfigC* config)
{
    if (config && config->data)
    {
        std::string address = config->data->getBoostTCPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_client_config_set_boost_udp_address(nexilis_ClientConfigC* config, const char* server_address)
{
    if (config && config->data && server_address)
    {
        config->data->setBoostUDPAddress(server_address);
    }
}

const char* nexilis_client_config_get_boost_udp_server_address(const nexilis_ClientConfigC* config)
{
    if (config && config->data)
    {
        std::string address = config->data->getBoostUDPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_client_config_set_unix_dgram_server_path(nexilis_ClientConfigC* config, const char* socket_path)
{
    if (config && config->data && socket_path)
    {
        config->data->setUnixDgramServerPath(socket_path);
    }
}

const char* nexilis_client_config_get_unix_dgram_server_path(const nexilis_ClientConfigC* config)
{
    if (config && config->data)
    {
        std::string path = config->data->getUnixDgramServerPath();
        char* cstr = (char*)malloc(path.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, path.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_client_config_set_unix_stream_server_path(nexilis_ClientConfigC* config, const char* socket_path)
{
    if (config && config->data)
    {
        config->data->setUnixStreamServerPath(socket_path);
    }
}

const char* nexilis_client_config_get_unix_stream_server_path(const nexilis_ClientConfigC* config)
{
    if (config && config->data)
    {
        std::string path = config->data->getUnixStreamServerPath();
        char* cstr = (char*)malloc(path.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, path.c_str());
        }
        return cstr;
    }
    return nullptr;
}
