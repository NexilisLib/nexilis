#ifndef NEXILIS_MACROS_HH
#define NEXILIS_MACROS_HH

#include <vector>
#include <cstdint>

#define NEXILIS_BUFFER 1024
#define NEXILIS_MAX std::numeric_limits<uint64_t>::max()
#define NEXILIS_DEFAULT_MAX_CLIENTS 1024
#define NEXILIS_DEFAULT_ROOM_CLIENT_AMOUNT 30

using nx_data = std::vector<uint8_t>;

#endif
