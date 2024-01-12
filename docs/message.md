
A message in the nexilis API consist of few parts

# Identification. (size_t)
First bytes are id of the client, it is a static size_t in the server code.

# Extra features here.
What could be needed? It's a little uncomfortable to add stuff later here,
but should be totally possible.

# The NULL byte (0xFF)
The NUll byte comes always before the command byte.
Should be used with extra features as well if needed.

# The command byte. (std::vector<uint8_t>[i])
The command byte comes after the identification.

Currently command bytes are readable from /include/nexilis/command_type.hh.
