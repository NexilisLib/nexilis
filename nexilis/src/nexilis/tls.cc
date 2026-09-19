/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

#include <nexilis/tls.hh>

#include <nexilis/crypto.hh>

#include <openssl/ssl.h>

#include <cstring>
#include <vector>

namespace nexilis::tls
{
namespace
{

void freePskExData(void* /*parent*/,
                   void* ptr,
                   CRYPTO_EX_DATA* /*ad*/,
                   int /*idx*/,
                   long /*argl*/,
                   void* /*argp*/)
{
    delete static_cast<std::vector<unsigned char>*>(ptr);
}

int pskExDataIndex()
{
    static const int index = SSL_CTX_get_ex_new_index(0, nullptr, nullptr, nullptr, freePskExData);
    return index;
}

const std::vector<unsigned char>* pskFromContext(SSL* ssl)
{
    SSL_CTX* ctx = SSL_get_SSL_CTX(ssl);
    if (!ctx)
    {
        return nullptr;
    }
    return static_cast<const std::vector<unsigned char>*>(SSL_CTX_get_ex_data(ctx, pskExDataIndex()));
}

unsigned int pskClientCallback(SSL* ssl,
                               const char* /*hint*/,
                               char* identity,
                               unsigned int max_identity_len,
                               unsigned char* psk,
                               unsigned int max_psk_len)
{
    const auto* key = pskFromContext(ssl);
    if (!key || key->size() > max_psk_len)
    {
        return 0;
    }

    constexpr std::size_t identity_len = sizeof(kPskIdentity); // includes the NUL terminator.
    if (max_identity_len < identity_len)
    {
        return 0;
    }
    std::memcpy(identity, kPskIdentity, identity_len);
    std::memcpy(psk, key->data(), key->size());
    return static_cast<unsigned int>(key->size());
}

unsigned int pskServerCallback(SSL* ssl,
                               const char* /*identity*/,
                               unsigned char* psk,
                               unsigned int max_psk_len)
{
    const auto* key = pskFromContext(ssl);
    if (!key || key->size() > max_psk_len)
    {
        return 0;
    }
    std::memcpy(psk, key->data(), key->size());
    return static_cast<unsigned int>(key->size());
}

} // namespace

std::string derivePsk(const std::string& password)
{
    // Domain-separated from the room-message keys: the same password produces
    // unrelated keys for message encryption and for the TLS transport.
    return crypto::deriveKey(password, "nexilis-tls-psk-v1:");
}

std::shared_ptr<boost::asio::ssl::context> createPskContext(const std::string& password, bool server)
{
    const std::string psk = derivePsk(password);
    if (psk.size() != kPskLength)
    {
        return nullptr;
    }

    auto ctx = std::make_shared<boost::asio::ssl::context>(
            server ? boost::asio::ssl::context::tls_server
                   : boost::asio::ssl::context::tls_client);

    ctx->set_options(
            boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::no_sslv2 |
            boost::asio::ssl::context::no_sslv3 |
            boost::asio::ssl::context::no_tlsv1 |
            boost::asio::ssl::context::no_tlsv1_1);

    SSL_CTX* native = ctx->native_handle();

    // Restrict the ciphers to PSK key exchange so no certificates are needed,
    // and to SHA-256 based suites so the 32-byte key always matches the PRF.
    if (SSL_CTX_set_cipher_list(native, "PSK-AES128-GCM-SHA256:PSK-CHACHA20-POLY1305") != 1)
    {
        return nullptr;
    }
    if (SSL_CTX_set_ciphersuites(native, "TLS_AES_128_GCM_SHA256:TLS_CHACHA20_POLY1305_SHA256") != 1)
    {
        return nullptr;
    }

    // Store the key against the context so multiple servers/clients with
    // different passwords can coexist; OpenSSL calls the free function when
    // the context is destroyed.
    auto* keyStorage = new std::vector<unsigned char>(psk.begin(), psk.end());
    SSL_CTX_set_ex_data(native, pskExDataIndex(), keyStorage);

    if (server)
    {
        SSL_CTX_set_psk_server_callback(native, pskServerCallback);
    }
    else
    {
        SSL_CTX_set_psk_client_callback(native, pskClientCallback);
    }

    return ctx;
}

} // namespace nexilis::tls
