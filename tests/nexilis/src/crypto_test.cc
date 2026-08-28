#include <gtest/gtest.h>

#include <nexilis/client/client_config.hh>
#include <nexilis/crypto.hh>

#include <string>

using namespace nexilis;

namespace
{

TEST(MessageEncryption, OffByDefault)
{
    client::ClientConfig config;
    EXPECT_FALSE(config.isMessageEncryptionEnabled());
}

TEST(MessageEncryption, ToggleFlipsState)
{
    client::ClientConfig config;
    config.setMessageEncryption(true);
    EXPECT_TRUE(config.isMessageEncryptionEnabled());
    config.setMessageEncryption(false);
    EXPECT_FALSE(config.isMessageEncryptionEnabled());
}

TEST(MessageEncryption, DeriveKeyIsStableForSameRoom)
{
    const auto first = crypto::deriveMessageKey("salasana", 42);
    const auto second = crypto::deriveMessageKey("salasana", 42);

    ASSERT_FALSE(first.empty());
    EXPECT_EQ(first, second);
}

TEST(MessageEncryption, DeriveKeyIsBoundToRoom)
{
    const auto roomA = crypto::deriveMessageKey("salasana", 1);
    const auto roomB = crypto::deriveMessageKey("salasana", 2);

    ASSERT_FALSE(roomA.empty());
    ASSERT_FALSE(roomB.empty());
    EXPECT_NE(roomA, roomB);
}

TEST(MessageEncryption, DeriveKeyIsBoundToPassword)
{
    const auto aliceKey = crypto::deriveMessageKey("alice", 7);
    const auto bobKey = crypto::deriveMessageKey("bob", 7);

    ASSERT_FALSE(aliceKey.empty());
    ASSERT_FALSE(bobKey.empty());
    EXPECT_NE(aliceKey, bobKey);
}

TEST(MessageEncryption, EncryptionDecryptionRoundTrip)
{
    const auto key = crypto::deriveMessageKey("salasana", 42);
    ASSERT_FALSE(key.empty());

    const std::string plaintext = "Hi Bob! Alice here.";
    std::string sealed;
    std::string restored;

    ASSERT_TRUE(crypto::encrypt(key, plaintext, sealed));
    ASSERT_TRUE(crypto::decrypt(key, sealed, restored));
    EXPECT_EQ(restored, plaintext);
}

TEST(MessageEncryption, EmptyMessageRoundTrip)
{
    const auto key = crypto::deriveMessageKey("salasana", 42);
    ASSERT_FALSE(key.empty());

    std::string sealed;
    std::string restored;

    ASSERT_TRUE(crypto::encrypt(key, "", sealed));
    ASSERT_TRUE(crypto::decrypt(key, sealed, restored));
    EXPECT_TRUE(restored.empty());
}

TEST(MessageEncryption, DecryptionWrongKeyFails)
{
    const auto senderKey = crypto::deriveMessageKey("alice", 42);
    const auto eavesdropperKey = crypto::deriveMessageKey("mallory", 42);
    ASSERT_FALSE(senderKey.empty());
    ASSERT_FALSE(eavesdropperKey.empty());

    std::string sealed;
    std::string restored;

    ASSERT_TRUE(crypto::encrypt(senderKey, "secret", sealed));
    EXPECT_FALSE(crypto::decrypt(eavesdropperKey, sealed, restored));
}

TEST(MessageEncryption, DecryptionRejectsTamperedCiphertext)
{
    const auto key = crypto::deriveMessageKey("salasana", 42);
    ASSERT_FALSE(key.empty());

    std::string sealed;
    ASSERT_TRUE(crypto::encrypt(key, "secret", sealed));

    // Flip a byte in the ciphertext portion (after the nonce).
    const auto flippedByteIndex = crypto::kNonceLength + 1;
    sealed[flippedByteIndex] = static_cast<char>(sealed[flippedByteIndex] ^ 0x01);

    std::string restored;
    EXPECT_FALSE(crypto::decrypt(key, sealed, restored));
}

TEST(MessageEncryption, DecryptionRejectsTruncatedInput)
{
    const auto key = crypto::deriveMessageKey("salasana", 42);
    ASSERT_FALSE(key.empty());

    std::string sealed;
    ASSERT_TRUE(crypto::encrypt(key, "secret", sealed));

    std::string restored;
    EXPECT_FALSE(crypto::decrypt(key, sealed.substr(0, sealed.size() / 2), restored));
}

TEST(Base64, KnownValues)
{
    EXPECT_EQ(crypto::toBase64(""), "");
    EXPECT_EQ(crypto::toBase64("f"), "Zg==");
    EXPECT_EQ(crypto::toBase64("fo"), "Zm8=");
    EXPECT_EQ(crypto::toBase64("foo"), "Zm9v");
    EXPECT_EQ(crypto::toBase64("foob"), "Zm9vYg==");
    EXPECT_EQ(crypto::toBase64("fooba"), "Zm9vYmE=");
    EXPECT_EQ(crypto::toBase64("foobar"), "Zm9vYmFy");
}

TEST(Base64, RoundTrip)
{
    const std::string binary = "\x00\x01Hello, world! \xFF\xFE\xFD";
    EXPECT_EQ(crypto::fromBase64(crypto::toBase64(binary)), binary);
}

TEST(Base64, RejectsInvalidInput)
{
    EXPECT_TRUE(crypto::fromBase64("not base64!").empty());
    EXPECT_TRUE(crypto::fromBase64("$$$$").empty());
}

} // namespace
