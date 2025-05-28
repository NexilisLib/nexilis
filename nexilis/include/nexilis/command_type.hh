#ifndef NEXILIX_COMMAND_TYPE_HH
#define NEXILIX_COMMAND_TYPE_HH

#include <cstdint>
#include <string>

namespace nexilis
{

enum class CommandType : uint8_t
{
    setting = 0,
    getting = 1,

    room = 2,
    /**
     *  2:0     Management
     *  2:0:0   Join room; uint64_t roomId
     *  2:0:1   Leave room; void
     *  2:0:2   Create room; string roomName
     *
     *  2:1     Communication.
     *  2:1:0   broadcast; string
     *  2:1:1   othercast; string
     *  2:1:2   unicast; uint64_t, string
     *
     *  2:2      Player2D
     *  2:2:0    Set position; Vec2f position
     *  2:2:1    Set dimensions; Vec2f dimensions
     *  2:2:2    2D movement vector; Vec2f movement
     *
     *  2:3      Object2D
     *  2:3:0    Create; Vec2f position, Vec2f dimensions, string path
     *  2:3:1    Destroy; uint64 id
     *  2:3:2    Move; uint64 id, Vec2f newPos
     *  2:3:3    Create moving;
     *              Vec2f startingPosition,
     *              Vec2f dimensions,
     *              Vec2f movement,
     *              float deltaTime,
     *              MovementType movementType,
     *              std::string path
     *
     *  2:4      Player3D
     *  2:4:0    Set position; Vector3f position
     *  2:4:1    Set dimensions; Vector3f dimensions
     *  2:4:2    3D movement vector; Vector3f movement
     *
     *  2:5      Object3D
     *
     */

    authentication = 3,
    server_management = 4,
    player_management = 5,
    error = 7,
    info = 8,
    undefined = 9
};

std::string commandTypeAsString(CommandType command_type);
CommandType stringAsCommandType(const std::string& str);

} // namespace nexilis

#endif
