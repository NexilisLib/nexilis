# Server Message Protocol

A message from client to the nexilis server consists of several parts.

## Framing (TCP only)

Each TCP message is prefixed with a **4-byte big-endian uint32_t length prefix** containing the size of the remaining payload. This ensures message boundaries are preserved over the TCP stream.

```
[4 bytes: payload length (big-endian uint32_t)]
[8 bytes: client ID]
[8 bytes: message ID]
[remaining: command bytes + parameters]
```

The receiver reads exactly 4 bytes for the length, then exactly that many bytes for the payload.

UDP messages do not use a length prefix — each datagram is a single message.

## Identification (uint64_t)

The first 8 bytes of the payload identify the client. A value of 0 indicates an unauthenticated client.

## Message ID (uint64_t)

The unique identifier for the message (8 bytes), used for correlating responses.

## Command bytes (std::vector<uint8_t>)

The command bytes come after the identification. At least 2 bytes:

- First byte: main command type (`/include/nexilis/command_type.hh`)
- Subsequent bytes: sub-command type(s) depending on the command

## Command parameters

Parameters given to `Packet`, varies in size depending on the command type. Strings are written as raw bytes with **no length prefix or null terminator** — they always occupy the remaining bytes after all fixed-size fields.

## Response format

Server responses are JSON objects terminated by a newline character (`\n`), making them readable with `async_read_until(socket, buffer, '\n')` on the client side.

## TLS-PSK transport protection (optional)

By default all TCP traffic travels in plaintext. Enabling TLS protects the whole connection (including the auentication exchange) from outsiders. No certificates or PKI are needed: the pre-shared key is derived from the existing authentication passphrase.

Both sides must opt in:

- `nexilis::server::ServerConfig::setTls(true)` on the server.
- `nexilis::client::ClientConfig::setTls(true)` (or `nexilis_client_config_set_tls` / the C# `ClientConfig.SetTls`) on every client.

Configuration details:

- The PSK is derived with PBKDF2-HMAC-SHA256 (100000 iterations) from the passphrase with the domain-separated salt `"nexilis-tls-psk-v1:"`, producing a 32-byte key (`nexilis::tls::derivePsk`).
- Only PSK cipher suites are negotiated (TLS 1.2 `PSK-AES128-GCM-SHA256` / `PSK-CHACHA20-POLY1305`, TLS 1.3 `TLS_AES_128_GCM_SHA256` / `TLS_CHACHA20_POLY1305_SHA256`), so no certificates are required.
- A passphrase must be set on both server and client or the handshake fails. If the server has TLS enabled but cannot build a context (no passphrase), it refuses incoming connections.
- If only one side enables TLS, the handshake fails rather than silently downgrading to plaintext.
- The handshake happens lazily: the server accepts it before the first message is read or written, and the client performs it right after the TCP connect. TLS is applied to both the initial connection and the random "switched" port the server uses for the authenticated session.
