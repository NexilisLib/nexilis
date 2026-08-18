#include <gtest/gtest.h>

#include <nexilisc/protocol_manager_c.h>
#include <nexilisc/protocol_type_c.h>
#include <nexilisc/server/protocol/boost_tcp_server_c.h>
#include <nexilisc/server/protocol/unix_stream_server_c.h>
#include <nexilisc/server/server_config_c.h>

class ProtocolManagerExtendedTest_c : public ::testing::Test
{
protected:
    nexilis_ProtocolManagerC* manager = nullptr;
    nexilis_server_ConfigC* settings = nullptr;

    void SetUp() override
    {
        manager = nexilis_protocol_manager_create();
        settings = nexilis_server_config_create();
        nexilis_server_config_set_mode(settings, AUTHENTICATION_MODE_PASSWORD_PROTECTED);
        nexilis_server_config_set_passphrase(settings, "salasana");
        nexilis_server_config_set_root_password(settings, "root");
    }

    void TearDown() override
    {
        nexilis_server_config_destroy(settings);
        nexilis_protocol_manager_destroy(manager);
    }
};

TEST_F(ProtocolManagerExtendedTest_c, ProtocolDataCreateDestroy)
{
    nexilis_ProtocolDataC* data = nexilis_protocol_data_create(PROTOCOL_TYPE_BOOST_TCP_SERVER);
    ASSERT_NE(data, nullptr);
    EXPECT_EQ(nexilis_protocol_data_get_type(data), PROTOCOL_TYPE_BOOST_TCP_SERVER);
    EXPECT_NE(nexilis_protocol_data_get_id(data), 0u);
    nexilis_protocol_data_destroy(data);
}

TEST_F(ProtocolManagerExtendedTest_c, ProtocolDataIdUniqueness)
{
    nexilis_ProtocolDataC* a = nexilis_protocol_data_create(PROTOCOL_TYPE_BOOST_TCP_SERVER);
    nexilis_ProtocolDataC* b = nexilis_protocol_data_create(PROTOCOL_TYPE_BOOST_TCP_SERVER);

    EXPECT_NE(nexilis_protocol_data_get_id(a), nexilis_protocol_data_get_id(b));

    nexilis_protocol_data_destroy(a);
    nexilis_protocol_data_destroy(b);
}

TEST_F(ProtocolManagerExtendedTest_c, CreateMultipleServers)
{
    nexilis_BoostTCPServer* tcp = nexilis_create_boost_tcp_server(manager, settings);
    ASSERT_NE(tcp, nullptr);

    auto stream_settings = nexilis_server_config_create();
    nexilis_server_config_set_mode(stream_settings, AUTHENTICATION_MODE_PASSWORD_PROTECTED);
    nexilis_server_config_set_passphrase(stream_settings, "pass");
    nexilis_server_config_set_root_password(stream_settings, "root");

    nexilis_UnixStreamServer* unix_stream = nexilis_create_unix_stream_server(
            manager, stream_settings, "/tmp/nexilis/ext_test_stream");
    ASSERT_NE(unix_stream, nullptr);

    EXPECT_EQ(nexilis_boost_tcp_server_get_type(tcp), PROTOCOL_TYPE_BOOST_TCP_SERVER);
    EXPECT_EQ(nexilis_unix_stream_server_get_type(unix_stream), PROTOCOL_TYPE_AF_UNIX_SOCK_STREAM_SERVER);

    nexilis_boost_tcp_server_destroy(tcp);
    nexilis_unix_stream_server_destroy(unix_stream);
    nexilis_server_config_destroy(stream_settings);
}

TEST_F(ProtocolManagerExtendedTest_c, ServerSettingsPropagation)
{
    nexilis_server_ConfigC* custom_settings = nexilis_server_config_create();
    nexilis_server_config_set_mode(custom_settings, AUTHENTICATION_MODE_ROOT_ACCESS);
    nexilis_server_config_set_passphrase(custom_settings, "custom_pass");
    nexilis_server_config_set_root_password(custom_settings, "custom_root");
    nexilis_server_config_set_tickrate(custom_settings, 120.0f);

    nexilis_BoostTCPServer* server = nexilis_create_boost_tcp_server(manager, custom_settings);

    nexilis_server_ConfigC retrieved = nexilis_boost_tcp_server_get_settings(server);
    EXPECT_EQ(retrieved.settings->getPassphrase(), "custom_pass");
    EXPECT_EQ(retrieved.settings->getRootPassword(), "custom_root");
    EXPECT_EQ(retrieved.settings->getMode(), nexilis::server::AuthenticationMode::root_access);
    EXPECT_FLOAT_EQ(retrieved.settings->getTickrate(), 120.0f);

    nexilis_boost_tcp_server_destroy(server);
    nexilis_server_config_destroy(custom_settings);
}
