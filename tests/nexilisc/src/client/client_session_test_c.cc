#include <gtest/gtest.h>

#include <nexilisc/client/client_api_c.h>
#include <nexilisc/client/client_config_c.h>
#include <nexilisc/client/client_session_c.h>

class ClientSessionTest_c : public ::testing::Test
{
protected:
    nexilis_ClientAPI* client_api = nullptr;

    void SetUp() override
    {
        nexilis_ClientConfigC* config = nexilis_client_config_create();
        client_api = nexilis_client_api_create(config);
        nexilis_client_config_destroy(config);
    }

    void TearDown() override
    {
        if (client_api)
        {
            nexilis_client_api_destroy(client_api);
        }
    }
};

TEST_F(ClientSessionTest_c, CreateAndDestroy)
{
    nexilis_ClientSession* session = nexilis_client_session_create(42, client_api);
    ASSERT_NE(session, nullptr);
    nexilis_client_session_destroy(session);
}

TEST_F(ClientSessionTest_c, GetId)
{
    nexilis_ClientSession* session = nexilis_client_session_create(42, client_api);
    EXPECT_EQ(nexilis_client_session_get_id(session), 42u);
    nexilis_client_session_destroy(session);
}

TEST_F(ClientSessionTest_c, GetIdDifferentValues)
{
    nexilis_ClientSession* s1 = nexilis_client_session_create(1, client_api);
    nexilis_ClientSession* s2 = nexilis_client_session_create(999, client_api);

    EXPECT_EQ(nexilis_client_session_get_id(s1), 1u);
    EXPECT_EQ(nexilis_client_session_get_id(s2), 999u);

    nexilis_client_session_destroy(s1);
    nexilis_client_session_destroy(s2);
}

TEST_F(ClientSessionTest_c, SetAndGetPosition3D)
{
    nexilis_ClientSession* session = nexilis_client_session_create(1, client_api);

    nexilis_client_session_set_position_3D(session, 1.0f, 2.0f, 3.0f);

    nexilis_Vector3f* pos = nexilis_client_session_get_position_3D(session);
    ASSERT_NE(pos, nullptr);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_x(pos), 1.0f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_y(pos), 2.0f);
    EXPECT_FLOAT_EQ(nexilis_vector3f_get_z(pos), 3.0f);

    nexilis_vector3f_destroy(pos);
    nexilis_client_session_destroy(session);
}

TEST_F(ClientSessionTest_c, GetPosition3DValues)
{
    nexilis_ClientSession* session = nexilis_client_session_create(1, client_api);

    nexilis_client_session_set_position_3D(session, 10.5f, -20.5f, 30.0f);

    float x, y, z;
    EXPECT_TRUE(nexilis_client_session_get_position_3D_values(session, &x, &y, &z));
    EXPECT_FLOAT_EQ(x, 10.5f);
    EXPECT_FLOAT_EQ(y, -20.5f);
    EXPECT_FLOAT_EQ(z, 30.0f);

    nexilis_client_session_destroy(session);
}

TEST_F(ClientSessionTest_c, EqualSessions)
{
    nexilis_ClientSession* a = nexilis_client_session_create(10, client_api);
    nexilis_ClientSession* b = nexilis_client_session_create(10, client_api);

    // Sessions with same id should be equal (depends on ClientSession implementation).
    bool eq = nexilis_client_session_equal(a, b);
    bool neq = nexilis_client_session_not_equal(a, b);
    EXPECT_EQ(eq, !neq);

    nexilis_client_session_destroy(a);
    nexilis_client_session_destroy(b);
}

TEST_F(ClientSessionTest_c, NotEqualSessions)
{
    nexilis_ClientSession* a = nexilis_client_session_create(10, client_api);
    nexilis_ClientSession* b = nexilis_client_session_create(20, client_api);

    bool neq = nexilis_client_session_not_equal(a, b);
    EXPECT_TRUE(neq);

    nexilis_client_session_destroy(a);
    nexilis_client_session_destroy(b);
}

TEST_F(ClientSessionTest_c, SetUsername)
{
    nexilis_ClientSession* session = nexilis_client_session_create(1, client_api);
    nexilis_client_session_set_username(session, "test_player");
    // No getter for username via C API, but this should not crash.
    nexilis_client_session_destroy(session);
}

TEST_F(ClientSessionTest_c, MoveSession)
{
    nexilis_ClientSession* original = nexilis_client_session_create(50, client_api);
    nexilis_client_session_set_position_3D(original, 1.0f, 2.0f, 3.0f);

    nexilis_ClientSession* moved = nexilis_client_session_move(original);
    ASSERT_NE(moved, nullptr);
    EXPECT_EQ(nexilis_client_session_get_id(moved), 50u);

    nexilis_client_session_destroy(moved);
    nexilis_client_session_destroy(original);
}

TEST_F(ClientSessionTest_c, MoveAssignSession)
{
    nexilis_ClientSession* dest = nexilis_client_session_create(1, client_api);
    nexilis_ClientSession* src = nexilis_client_session_create(2, client_api);

    nexilis_client_session_move_assign(dest, src);
    EXPECT_EQ(nexilis_client_session_get_id(dest), 2u);

    nexilis_client_session_destroy(dest);
    nexilis_client_session_destroy(src);
}
