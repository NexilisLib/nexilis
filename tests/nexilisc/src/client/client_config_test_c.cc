#include <gtest/gtest.h>

#include <nexilisc/client/client_config_c.h>
#include <nexilisc/server/authentication_mode_c.h>

#include <cstdlib>
#include <cstring>

TEST(ClientConfigTest_c, CreateDestroy)
{
    nexilis_ClientConfigC* config = nexilis_client_config_create();
    ASSERT_NE(config, nullptr);
    nexilis_client_config_destroy(config);
}

TEST(ClientConfigTest_c, PasswordSetGet)
{
    nexilis_ClientConfigC* config = nexilis_client_config_create();

    nexilis_client_config_set_password(config, "my_password");
    char* password = (char*)nexilis_client_config_get_password(config);
    EXPECT_STREQ(password, "my_password");
    free(password);

    nexilis_client_config_destroy(config);
}

TEST(ClientConfigTest_c, Mode)
{
    nexilis_ClientConfigC* config = nexilis_client_config_create();

    nexilis_client_config_set_mode(config, AUTHENTICATION_MODE_PASSWORD_PROTECTED);

    nexilis_client_config_destroy(config);
}

TEST(ClientConfigTest_c, BoostTCPAddress)
{
    nexilis_ClientConfigC* config = nexilis_client_config_create();

    nexilis_client_config_set_boost_tcp_address(config, "192.168.1.100");
    char* addr = (char*)nexilis_client_config_get_boost_tcp_server_address(config);
    EXPECT_STREQ(addr, "192.168.1.100");
    free(addr);

    nexilis_client_config_destroy(config);
}

TEST(ClientConfigTest_c, BoostUDPAddress)
{
    nexilis_ClientConfigC* config = nexilis_client_config_create();

    nexilis_client_config_set_boost_udp_address(config, "10.0.0.1");
    char* addr = (char*)nexilis_client_config_get_boost_udp_server_address(config);
    EXPECT_STREQ(addr, "10.0.0.1");
    free(addr);

    nexilis_client_config_destroy(config);
}

TEST(ClientConfigTest_c, InetUDPAddress)
{
    nexilis_ClientConfigC* config = nexilis_client_config_create();

    nexilis_client_config_set_inet_udp(config, "172.16.0.1");
    char* addr = (char*)nexilis_client_config_get_inet_udp_server_address(config);
    EXPECT_STREQ(addr, "172.16.0.1");
    free(addr);

    nexilis_client_config_destroy(config);
}

TEST(ClientConfigTest_c, InetTCPAddress)
{
    nexilis_ClientConfigC* config = nexilis_client_config_create();

    nexilis_client_config_set_inet_tcp(config, "172.16.0.2");
    char* addr = (char*)nexilis_client_config_get_inet_tcp_server_address(config);
    EXPECT_STREQ(addr, "172.16.0.2");
    free(addr);

    nexilis_client_config_destroy(config);
}

TEST(ClientConfigTest_c, UnixDgramPath)
{
    nexilis_ClientConfigC* config = nexilis_client_config_create();

    nexilis_client_config_set_unix_dgram_server_path(config, "/tmp/nexilis/dgram");
    char* path = (char*)nexilis_client_config_get_unix_dgram_server_path(config);
    EXPECT_STREQ(path, "/tmp/nexilis/dgram");
    free(path);

    nexilis_client_config_destroy(config);
}

TEST(ClientConfigTest_c, UnixStreamPath)
{
    nexilis_ClientConfigC* config = nexilis_client_config_create();

    nexilis_client_config_set_unix_stream_server_path(config, "/tmp/nexilis/stream");
    char* path = (char*)nexilis_client_config_get_unix_stream_server_path(config);
    EXPECT_STREQ(path, "/tmp/nexilis/stream");
    free(path);

    nexilis_client_config_destroy(config);
}

TEST(ClientConfigTest_c, TlsToggle)
{
    nexilis_ClientConfigC* config = nexilis_client_config_create();

    EXPECT_EQ(nexilis_client_config_get_tls(config), 0);

    nexilis_client_config_set_tls(config, 1);
    EXPECT_EQ(nexilis_client_config_get_tls(config), 1);

    nexilis_client_config_set_tls(config, 0);
    EXPECT_EQ(nexilis_client_config_get_tls(config), 0);

    nexilis_client_config_destroy(config);
}
