#ifndef NEXILIS_CONSTANTS_HH
#define NEXILIS_CONSTANTS_HH

#include <cstdint>
#include <limits>

namespace nexilis
{

constexpr inline uint64_t NEXILIS_BUFFER = 1024;
constexpr inline uint64_t NEXILIS_MAX = std::numeric_limits<uint64_t>::max();
constexpr inline uint32_t NEXILIS_DEFAULT_MAX_CLIENTS = 1024;
constexpr inline uint32_t NEXILIS_DEFAULT_ROOM_CLIENT_AMOUNT = 30;
constexpr inline float NEXILIS_MAX_POSITION = 10000.0f;

} // namespace nexilis

#endif
