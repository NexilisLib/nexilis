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
