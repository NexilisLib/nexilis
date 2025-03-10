A messaga from client to the nexilis server consists of few parts.

# Identification. (size_t)
First 8 bytes is the id of the client.

# Message id (size_t)
The unique identifier for the message (8 bytes).

# The command byte. (std::vector<uint8_t>[i])
The command byte comes after the identification.
At least 2 bytes in size.

# The command parameters
Parameters given to "Packet", varies in size.

Currently command bytes are readable from /include/nexilis/command_type.hh.
