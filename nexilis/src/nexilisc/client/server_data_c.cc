#include <nexilisc/client/server_data_c.h>

nexilis_ServerData* nexilis_server_data_create()
{
    auto server_data = new nexilis_ServerData();
    auto cpp_data = new nexilis::client::ServerData();
    server_data->data = cpp_data;
    return server_data;
}

void nexilis_server_data_destroy(nexilis_ServerData* server_data)
{
    if (server_data)
    {
        if (server_data->data)
        {
            delete server_data->data;
            server_data->data = nullptr;
        }
        delete server_data;
    }
}

void nexilis_server_data_set_password(nexilis_ServerData* server_data, const char* password)
{
    if (server_data && server_data->data && password)
    {
        server_data->data->setPassword(password);
    }
}

const char* nexilis_server_data_get_password(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string password = server_data->data->getPassword();
        char* cstr = (char*)malloc(password.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, password.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_inet_udp(nexilis_ServerData* server_data, const char* server_address)
{
    if (server_data && server_data->data && server_address)
    {
        server_data->data->setInetUDP(server_address);
    }
}

const char* nexilis_server_data_get_inet_udp_server_address(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string address = server_data->data->getInetUDPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_inet_tcp(nexilis_ServerData* server_data, const char* server_address)
{
    if (server_data && server_data->data && server_address)
    {
        server_data->data->setInetTCP(server_address);
    }
}

const char* nexilis_server_data_get_inet_tcp_server_address(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string address = server_data->data->getInetTCPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_boost_tcp_address(nexilis_ServerData* server_data, const char* server_address)
{
    if (server_data && server_data->data && server_address)
    {
        server_data->data->setBoostTCPAddress(server_address);
    }
}

const char* nexilis_server_data_get_boost_tcp_server_address(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string address = server_data->data->getBoostTCPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_boost_udp(nexilis_ServerData* server_data, const char* server_address)
{
    if (server_data && server_data->data && server_address)
    {
        server_data->data->setBoostUDP(server_address);
    }
}

const char* nexilis_server_data_get_boost_udp_server_address(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string address = server_data->data->getBoostUDPServerAddress();
        char* cstr = (char*)malloc(address.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, address.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_unix_dgram_server_path(nexilis_ServerData* server_data, const char* socket_path)
{
    if (server_data && server_data->data && socket_path)
    {
        server_data->data->setUnixDgramServerPath(socket_path);
    }
}

const char* nexilis_server_data_get_unix_dgram_server_path(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string path = server_data->data->getUnixDgramServerPath();
        char* cstr = (char*)malloc(path.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, path.c_str());
        }
        return cstr;
    }
    return nullptr;
}

void nexilis_server_data_set_unix_stream_server_path(nexilis_ServerData* server_data, const char* socket_path)
{
    if (server_data && server_data->data)
    {
        server_data->data->setUnixStreamServerPath(socket_path);
    }
}

const char* nexilis_server_data_get_unix_stream_server_path(const nexilis_ServerData* server_data)
{
    if (server_data && server_data->data)
    {
        std::string path = server_data->data->getUnixStreamServerPath();
        char* cstr = (char*)malloc(path.size() + 1);
        if (cstr)
        {
            std::strcpy(cstr, path.c_str());
        }
        return cstr;
    }
    return nullptr;
}
