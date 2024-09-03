
A message in the nexilis API consist of few parts

# Identification. (size_t)
First 8 bytes is the id of the client.

# The first NULL byte (0xFF)
Null byte separating client id and message id.

# Message id (size_t)
The unique identifier for the message (8 bytes).

# The NULL byte (0xFF)
The NUll byte comes always before the command byte.
Should be used with extra features as well if needed.

# The command byte. (std::vector<uint8_t>[i])
The command byte comes after the identification.
At least 2 bytes in size.

Currently command bytes are readable from /include/nexilis/command_type.hh.
