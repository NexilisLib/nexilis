#include <gtest/gtest.h>

#include <nexilisc/server/authentication_mode_c.h>
#include <nexilisc/server/server_config_c.h>

#include <cstring>

TEST(ServerConfigTest_c, CreateDestroy)
{
    nexilis_server_ConfigC* config = nexilis_server_config_create();
    ASSERT_NE(config, nullptr);
    nexilis_server_config_destroy(config);
}

TEST(ServerConfigTest_c, PassphraseSetGet)
{
    nexilis_server_ConfigC* config = nexilis_server_config_create();

    nexilis_server_config_set_passphrase(config, "secret");
    EXPECT_TRUE(nexilis_server_config_has_passphrase(config));

    const char* passphrase = nexilis_server_config_get_passphrase(config);
    EXPECT_STREQ(passphrase, "secret");

    nexilis_server_config_destroy(config);
}

TEST(ServerConfigTest_c, PassphraseMatchesCorrect)
{
    nexilis_server_ConfigC* config = nexilis_server_config_create();
    nexilis_server_config_set_passphrase(config, "secret");

    EXPECT_TRUE(nexilis_server_config_is_passphrase(config, "secret"));
    EXPECT_FALSE(nexilis_server_config_is_passphrase(config, "sec"));
    EXPECT_FALSE(nexilis_server_config_is_passphrase(config, "secretpassword"));
    EXPECT_FALSE(nexilis_server_config_is_passphrase(config, "secrex"));

    nexilis_server_config_destroy(config);
}

TEST(ServerConfigTest_c, PassphraseEmptyString)
{
    nexilis_server_ConfigC* config = nexilis_server_config_create();

    EXPECT_FALSE(nexilis_server_config_has_passphrase(config));

    nexilis_server_config_destroy(config);
}

TEST(ServerConfigTest_c, RootPasswordSetGet)
{
    nexilis_server_ConfigC* config = nexilis_server_config_create();

    nexilis_server_config_set_root_password(config, "rootsecret");
    EXPECT_TRUE(nexilis_server_config_has_root_password(config));

    const char* root_password = nexilis_server_config_get_root_password(config);
    EXPECT_STREQ(root_password, "rootsecret");

    nexilis_server_config_destroy(config);
}

TEST(ServerConfigTest_c, RootPasswordMatchesCorrect)
{
    nexilis_server_ConfigC* config = nexilis_server_config_create();
    nexilis_server_config_set_root_password(config, "rootsecret");

    EXPECT_TRUE(nexilis_server_config_is_root_password(config, "rootsecret"));
    EXPECT_FALSE(nexilis_server_config_is_root_password(config, "rootsec"));
    EXPECT_FALSE(nexilis_server_config_is_root_password(config, "rootsecretpassword"));
    EXPECT_FALSE(nexilis_server_config_is_root_password(config, "rootsecr"));

    nexilis_server_config_destroy(config);
}

TEST(ServerConfigTest_c, RootPasswordEmpty)
{
    nexilis_server_ConfigC* config = nexilis_server_config_create();

    EXPECT_FALSE(nexilis_server_config_has_root_password(config));

    nexilis_server_config_destroy(config);
}

TEST(ServerConfigTest_c, AuthenticationMode)
{
    nexilis_server_ConfigC* config = nexilis_server_config_create();

    nexilis_server_config_set_mode(config, AUTHENTICATION_MODE_PASSWORD_PROTECTED);
    EXPECT_EQ(nexilis_server_config_get_mode(config), AUTHENTICATION_MODE_PASSWORD_PROTECTED);

    nexilis_server_config_set_mode(config, AUTHENTICATION_MODE_SKIP);
    EXPECT_EQ(nexilis_server_config_get_mode(config), AUTHENTICATION_MODE_SKIP);

    nexilis_server_config_set_mode(config, AUTHENTICATION_MODE_EMPTY);
    EXPECT_EQ(nexilis_server_config_get_mode(config), AUTHENTICATION_MODE_EMPTY);

    nexilis_server_config_set_mode(config, AUTHENTICATION_MODE_ADMIN_ACCESS);
    EXPECT_EQ(nexilis_server_config_get_mode(config), AUTHENTICATION_MODE_ADMIN_ACCESS);

    nexilis_server_config_set_mode(config, AUTHENTICATION_MODE_ROOT_ACCESS);
    EXPECT_EQ(nexilis_server_config_get_mode(config), AUTHENTICATION_MODE_ROOT_ACCESS);

    nexilis_server_config_destroy(config);
}

TEST(ServerConfigTest_c, Tickrate)
{
    nexilis_server_ConfigC* config = nexilis_server_config_create();

    nexilis_server_config_set_tickrate(config, 60.0f);
    EXPECT_FLOAT_EQ(nexilis_server_config_get_tickrate(config), 60.0f);

    nexilis_server_config_set_tickrate(config, 30.5f);
    EXPECT_FLOAT_EQ(nexilis_server_config_get_tickrate(config), 30.5f);

    nexilis_server_config_destroy(config);
}

TEST(ServerConfigTest_c, PassphraseAndRootPasswordIndependent)
{
    nexilis_server_ConfigC* config = nexilis_server_config_create();

    nexilis_server_config_set_passphrase(config, "passphrase_secret");
    nexilis_server_config_set_root_password(config, "root_secret");

    EXPECT_TRUE(nexilis_server_config_is_passphrase(config, "passphrase_secret"));
    EXPECT_FALSE(nexilis_server_config_is_passphrase(config, "root_secret"));

    EXPECT_TRUE(nexilis_server_config_is_root_password(config, "root_secret"));
    EXPECT_FALSE(nexilis_server_config_is_root_password(config, "passphrase_secret"));

    nexilis_server_config_destroy(config);
}
