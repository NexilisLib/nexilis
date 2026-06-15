#ifndef NEXILIS_ROOM_INFO_HH
#define NEXILIS_ROOM_INFO_HH

#include <cstdint>
#include <string>

namespace nexilis
{

struct RoomInfo
{
    uint64_t id;
    std::string name;
};

} // namespace nexilis

#endif
