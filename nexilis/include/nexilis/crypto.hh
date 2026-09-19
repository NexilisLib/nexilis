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

#ifndef NEXILIS_CRYPTO_HH
#define NEXILIS_CRYPTO_HH

#include <cstddef>
#include <cstdint>
#include <string>

namespace nexilis::crypto
{

/// Length of an AES-256-GCM message key in bytes.
constexpr std::size_t kKeyLength = 32;

/// Length of the GCM nonce in bytes.
constexpr std::size_t kNonceLength = 12;

/// Length of the GCM authentication tag in bytes.
constexpr std::size_t kTagLength = 16;

/// Derive a key from a password via PBKDF2-HMAC-SHA256.
///
/// The result is cached per (password, salt) pair. This is the primitive
/// behind deriveMessageKey(); it is public so the TLS layer can derive its
/// pre-shared key from the same password with its own domain-separated salt.
///
/// \param password   The connection password stored in the AuthConfig.
/// \param salt       A domain-separating salt string appended to the
///                   "nexilis-" domain prefix by the callers.
/// \param length     Key length in bytes (defaults to the AES-GCM key length).
/// \param iterations PBKDF2 iteration count (defaults to the message-key count).
/// \return The derived key, or an empty string on failure.
std::string deriveKey(const std::string& password,
                      const std::string& salt,
                      std::size_t length = kKeyLength,
                      int iterations = 100000);

/// Derive a 32-byte message key from a connection password, bound to the
/// given room id, so different rooms use different keys.
///
/// The result is cached per (password, room id) pair.
///
/// \param password The connection password stored in the AuthConfig.
/// \param room_id  The room id the message is broadcast in.
/// \return The derived key, or an empty string on failure.
std::string deriveMessageKey(const std::string& password, uint64_t room_id);

/// Authenticated-encrypt plaintext with AES-256-GCM.
///
/// \param key       32-byte key, for example from deriveMessageKey.
/// \param plaintext The data to encrypt.
/// \param out       Receives "nonce || ciphertext || tag".
/// \return True on success.
bool encrypt(const std::string& key, const std::string& plaintext, std::string& out);

/// Authenticated-decrypt a message produced by encrypt().
///
/// \param key    32-byte key, for example from deriveMessageKey.
/// \param sealed "nonce || ciphertext || tag" as produced by encrypt().
/// \param out    Receives the plaintext on success.
/// \return True on success (including a valid authentication tag), false if
///         the message was tampered with or the key does not match.
bool decrypt(const std::string& key, const std::string& sealed, std::string& out);

/// RFC 4648 base64 encoding (with padding).
std::string toBase64(const std::string& data);

/// RFC 4648 base64 decoding (with padding).
/// \return The decoded bytes, or an empty string on invalid input.
std::string fromBase64(const std::string& data);

} // namespace nexilis::crypto

#endif
