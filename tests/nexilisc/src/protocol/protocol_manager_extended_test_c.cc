#include <gtest/gtest.h>

#include <nexilisc/protocol_manager_c.h>
#include <nexilisc/protocol_type_c.h>

class ProtocolManagerExtendedTest_c : public ::testing::Test
{
protected:
    nexilis_ProtocolManagerC* manager = nullptr;

    void SetUp() override
    {
        manager = nexilis_protocol_manager_create();
    }

    void TearDown() override
    {
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
