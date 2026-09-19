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

#ifndef NEXILIS_TLS_HH
#define NEXILIS_TLS_HH

#include <boost/asio/ssl/context.hpp>

#include <cstddef>
#include <memory>
#include <string>

namespace nexilis::tls
{

/// Length in bytes of the TLS pre-shared key. The TLS 1.3 SHA-256 cipher
/// suites require at least 32 bytes, so this matches the AES-GCM key length.
constexpr std::size_t kPskLength = 32;

/// The PSK identity announced by clients during the handshake. It is
/// constant; the secret is the password-derived key, not the identity.
constexpr char kPskIdentity[] = "nexilis-tls-psk-v1";

/// Derive the raw pre-shared key bytes for a given password.
///
/// Deterministic and domain-separated from the room-message keys, so the same
/// password yields unrelated keys for message encryption and TLS.
///
/// \param password The shared passphrase.
/// \return The key bytes, or an empty string on failure.
std::string derivePsk(const std::string& password);

/// Build an SSL context that authenticates with TLS-PSK derived from the
/// auth password. No certificates are involved, so the existing passphrase
/// can drive the TLS layer on both the server and the client.
///
/// All Nexilis connections through the returned context speak TLS-PSK; there
/// is no fallback to plaintext, so a "wrong password" peer fails the
/// handshake instead of silently downgrading.
///
/// \param server   True to set up the server role, false for the client role.
/// \param password The shared passphrase. Empty means no PSK can be derived.
/// \return The configured context, or nullptr when TLS-PSK cannot be set up.
std::shared_ptr<boost::asio::ssl::context> createPskContext(const std::string& password, bool server);

} // namespace nexilis::tls

#endif
