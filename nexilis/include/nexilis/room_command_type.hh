#ifndef NEXILIS_ROOM_COMMAND_TYPE_HH
#define NEXILIS_ROOM_COMMAND_TYPE_HH

#include <nexilis/types/vector2.hh>
#include <nexilis/types/vector3.hh>

#include <cstdint>
#include <string>

namespace nexilis
{

class RoomCommandType
{
public:
    enum class Root : uint8_t
    {
        management = 0,
        communication = 1,
        player2D = 2,
        object2D = 3,
        player3D = 4,
        object3D = 5,
        undefined = 6
    };

    enum class Management : uint8_t
    {
        join = 0,
        leave = 1,
        create = 2
    };

    enum class Communication : uint8_t
    {
        broadcast,
        othercast,
        unicast
    };

    enum class Player2D : uint8_t
    {
        position = 0,
        dimensions = 1,
        movement = 2
    };

    enum class Object2D : uint8_t
    {
        create = 0,
        destroy = 1,
        move = 2,
        createMoving = 3,
        createMovingTest = 4
    };

    enum class Player3D : uint8_t
    {
        position = 0,
        dimensions = 1,
        movement = 2
    };

    enum class Object3D : uint8_t
    {
        create = 0,
        destroy = 1,
        move = 2
    };

    static std::string RoomTypeToString(Root type);
    static std::string ManagementTypeToString(Management management);
    static std::string CommunicationTypeToString(Communication communication);

    static std::string PlayerTypeToString(Player2D player2D);
    static std::string PlayerTypeToString(Player3D player2D);

    static std::string ObjectTypeToString(Object2D object2D);
    static std::string ObjectTypeToString(Object3D object2D);

    template <typename VectorType>
    static Root getRootPlayerType(const VectorType& v)
    {
        if constexpr (std::is_same_v<VectorType, Vector2<decltype(v)>>)
        {
            return Root::player2D;
        }
        else if constexpr (std::is_same_v<VectorType, Vector3<decltype(v)>>)
        {
            return Root::player3D;
        }
        return Root::undefined;
    }

    template <typename VectorType>
    static Root getRootObjectType(const VectorType& v)
    {
        if constexpr (std::is_same_v<VectorType, Vector2<decltype(v)>>)
        {
            return Root::object2D;
        }
        else if constexpr (std::is_same_v<VectorType, Vector3<decltype(v)>>)
        {
            return Root::object3D;
        }
        return Root::undefined;
    }
};

} // namespace nexilis

#endif
