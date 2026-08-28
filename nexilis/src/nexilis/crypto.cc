#include <nexilis/crypto.hh>

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <cstring>
#include <functional>
#include <mutex>
#include <unordered_map>
#include <utility>

namespace nexilis::crypto
{
namespace
{

/// Salt prefix that separates Nexilis message keys from any other use of the
/// same password. The room id is appended to keep room keys distinct.
constexpr char kSaltPrefix[] = "nexilis-message-key-v1:";

/// Hash functor so (password, room id) pairs can be used as map keys.
struct KeyHash
{
    std::size_t operator()(const std::pair<std::string, uint64_t>& key) const
    {
        std::size_t hash = std::hash<std::string>{}(key.first);
        hash ^= std::hash<uint64_t>{}(key.second) + 0x9e3779b97f4a7c15ULL + (hash << 6) + (hash >> 2);
        return hash;
    }
};

std::mutex g_keyCacheMutex;
std::unordered_map<std::pair<std::string, uint64_t>, std::string, KeyHash> g_keyCache;

} // namespace

std::string deriveKey(const std::string& password,
                      const std::string& salt,
                      std::size_t length,
                      int iterations)
{
    std::string key(length, '\0');
    if (length == 0)
    {
        return key;
    }
    if (password.empty() || salt.empty())
    {
        return {};
    }

    const int result = PKCS5_PBKDF2_HMAC(
            password.c_str(), static_cast<int>(password.size()),
            reinterpret_cast<const unsigned char*>(salt.c_str()), static_cast<int>(salt.size()),
            iterations, EVP_sha256(),
            static_cast<int>(key.size()), reinterpret_cast<unsigned char*>(key.data()));
    if (result != 1)
    {
        return {};
    }
    return key;
}

std::string deriveMessageKey(const std::string& password, uint64_t room_id)
{
    const auto cacheKey = std::make_pair(password, room_id);
    {
        std::lock_guard<std::mutex> lock(g_keyCacheMutex);
        const auto it = g_keyCache.find(cacheKey);
        if (it != g_keyCache.end())
        {
            return it->second;
        }
    }

    const std::string key = deriveKey(password, std::string(kSaltPrefix) + std::to_string(room_id));

    if (key.empty())
    {
        return {};
    }

    {
        std::lock_guard<std::mutex> lock(g_keyCacheMutex);
        g_keyCache[cacheKey] = key;
    }
    return key;
}

bool encrypt(const std::string& key, const std::string& plaintext, std::string& out)
{
    if (key.size() != kKeyLength)
    {
        return false;
    }

    std::string nonce(kNonceLength, '\0');
    if (RAND_bytes(reinterpret_cast<unsigned char*>(nonce.data()), static_cast<int>(nonce.size())) != 1)
    {
        return false;
    }

    // GCM does not expand the payload: ciphertext has the same length as the
    // plaintext, followed by the fixed-size authentication tag.
    std::string sealed;
    sealed.resize(kNonceLength + plaintext.size() + kTagLength);
    std::memcpy(sealed.data(), nonce.data(), kNonceLength);

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        return false;
    }

    const unsigned char* keyPtr = reinterpret_cast<const unsigned char*>(key.data());
    const unsigned char* noncePtr = reinterpret_cast<const unsigned char*>(nonce.data());
    const unsigned char* plainPtr = reinterpret_cast<const unsigned char*>(plaintext.data());
    unsigned char* sealedPtr = reinterpret_cast<unsigned char*>(sealed.data()) + kNonceLength;

    int len = 0;
    bool ok = false;
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) == 1 &&
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(kNonceLength), nullptr) == 1 &&
        EVP_EncryptInit_ex(ctx, nullptr, nullptr, keyPtr, noncePtr) == 1 &&
        EVP_EncryptUpdate(ctx, sealedPtr, &len, plainPtr, static_cast<int>(plaintext.size())) == 1 &&
        EVP_EncryptFinal_ex(ctx, sealedPtr + len, &len) == 1)
    {
        unsigned char tag[kTagLength];
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, static_cast<int>(kTagLength), tag) == 1)
        {
            std::memcpy(sealedPtr + plaintext.size(), tag, kTagLength);
            out = std::move(sealed);
            ok = true;
        }
    }

    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

bool decrypt(const std::string& key, const std::string& sealed, std::string& out)
{
    if (key.size() != kKeyLength || sealed.size() < kNonceLength + kTagLength)
    {
        return false;
    }

    const std::size_t ciphertextLength = sealed.size() - kNonceLength - kTagLength;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx)
    {
        return false;
    }

    const unsigned char* keyPtr = reinterpret_cast<const unsigned char*>(key.data());
    const unsigned char* noncePtr = reinterpret_cast<const unsigned char*>(sealed.data());
    const unsigned char* sealedPtr = noncePtr + kNonceLength;
    const unsigned char* tagPtr = sealedPtr + ciphertextLength;

    std::string plaintext;
    plaintext.resize(ciphertextLength);

    int len = 0;
    bool ok = false;
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) == 1 &&
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, static_cast<int>(kNonceLength), nullptr) == 1 &&
        EVP_DecryptInit_ex(ctx, nullptr, nullptr, keyPtr, noncePtr) == 1 &&
        EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(plaintext.data()), &len,
                          sealedPtr, static_cast<int>(ciphertextLength)) == 1)
    {
        unsigned char tag[kTagLength];
        std::memcpy(tag, tagPtr, kTagLength);
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, static_cast<int>(kTagLength), tag) == 1 &&
            EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(plaintext.data()) + len, &len) == 1)
        {
            out = std::move(plaintext);
            ok = true;
        }
    }

    EVP_CIPHER_CTX_free(ctx);
    return ok;
}

std::string toBase64(const std::string& data)
{
    static const char kAlphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    // Pad the input to a multiple of three bytes so the loop below can always
    // read a full three-byte triplet.
    std::string padded(data);
    while (padded.size() % 3 != 0)
    {
        padded.push_back('\0');
    }

    std::string out;
    out.reserve(((data.size() + 2) / 3) * 4);

    for (std::size_t i = 0; i < padded.size(); i += 3)
    {
        const std::uint32_t triple =
                (static_cast<unsigned char>(padded[i]) << 16) |
                (static_cast<unsigned char>(padded[i + 1]) << 8) |
                static_cast<unsigned char>(padded[i + 2]);

        out.push_back(kAlphabet[(triple >> 18) & 0x3F]);
        out.push_back(kAlphabet[(triple >> 12) & 0x3F]);
        out.push_back(data.size() - i > 1 ? kAlphabet[(triple >> 6) & 0x3F] : '=');
        out.push_back(data.size() - i > 2 ? kAlphabet[triple & 0x3F] : '=');
    }
    return out;
}

std::string fromBase64(const std::string& data)
{
    static const int8_t kReverse[256] = {
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            62,
            -1,
            -1,
            -1,
            63,
            52,
            53,
            54,
            55,
            56,
            57,
            58,
            59,
            60,
            61,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            0,
            1,
            2,
            3,
            4,
            5,
            6,
            7,
            8,
            9,
            10,
            11,
            12,
            13,
            14,
            15,
            16,
            17,
            18,
            19,
            20,
            21,
            22,
            23,
            24,
            25,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            26,
            27,
            28,
            29,
            30,
            31,
            32,
            33,
            34,
            35,
            36,
            37,
            38,
            39,
            40,
            41,
            42,
            43,
            44,
            45,
            46,
            47,
            48,
            49,
            50,
            51,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
            -1,
    };

    std::string out;
    out.reserve((data.size() / 4) * 3);

    std::uint32_t accumulator = 0;
    int bits = 0;
    for (const char c : data)
    {
        if (c == '=')
        {
            break;
        }
        const auto value = kReverse[static_cast<unsigned char>(c)];
        if (value < 0)
        {
            return {};
        }
        accumulator = (accumulator << 6) | static_cast<std::uint32_t>(value);
        bits += 6;
        if (bits >= 8)
        {
            bits -= 8;
            out.push_back(static_cast<char>((accumulator >> bits) & 0xFF));
        }
    }
    return out;
}

} // namespace nexilis::crypto