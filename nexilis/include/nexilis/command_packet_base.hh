#ifndef NEXILIS_COMMAND_PACKET_BASE_HH
#define NEXILIS_COMMAND_PACKET_BASE_HH

#include <utility>

template <typename Impl>
class CommandPacketBase
{
public:
    struct Set
    {
        struct General
        {
            template <typename... Args>
            static auto clientId(Args&&... args)
            {
                return Impl::set_general_clientId(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto username(Args&&... args)
            {
                return Impl::set_general_username(std::forward<Args>(args)...);
            }
        };
        struct Protocol
        {
            struct BoostTCP
            {
                template <typename... Args>
                static auto port(Args&&... args)
                {
                    return Impl::set_protocol_boosttcp_port(std::forward<Args>(args)...);
                }
            };
        };
    };

    struct Get
    {
        struct General
        {
            template <typename... Args>
            static auto clientId(Args&&... args)
            {
                return Impl::get_general_clientId(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto roomId(Args&&... args)
            {
                return Impl::get_general_roomId(std::forward<Args>(args)...);
            }
        };
        struct Info
        {
            template <typename... Args>
            static auto general(Args&&... args)
            {
                return Impl::get_info_general(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto clients(Args&&... args)
            {
                return Impl::get_info_clients(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto rooms(Args&&... args)
            {
                return Impl::get_info_rooms(std::forward<Args>(args)...);
            }
        };
    };

    struct Room
    {
        struct Management
        {
            template <typename... Args>
            static auto join(Args&&... args)
            {
                return Impl::room_management_join(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto leave(Args&&... args)
            {
                return Impl::room_management_leave(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto create(Args&&... args)
            {
                return Impl::room_management_create(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto remove(Args&&... args)
            {
                return Impl::room_management_remove(std::forward<Args>(args)...);
            }
        };
        struct Communicate
        {
            template <typename... Args>
            static auto broadcast(Args&&... args)
            {
                return Impl::room_communicate_broadcast(std::forward<Args>(args)...);
            }

            template <typename... Args>
            static auto othercast(Args&&... args)
            {
                return Impl::room_communicate_othercast(std::forward<Args>(args)...);
            }

            template <typename... Args>
            static auto unicast(Args&&... args)
            {
                return Impl::room_communicate_unicast(std::forward<Args>(args)...);
            }
        };

        struct Player2D
        {
            template <typename... Args>
            static auto position(Args&&... args)
            {
                return Impl::room_player2d_position(std::forward<Args>(args)...);
            }

            template <typename... Args>
            static auto dimension(Args&&... args)
            {
                return Impl::room_player2d_dimension(std::forward<Args>(args)...);
            }

            template <typename... Args>
            static auto movement(Args&&... args)
            {
                return Impl::room_player2d_movement(std::forward<Args>(args)...);
            }
        };

        struct Object2D
        {
            template <typename... Args>
            static auto create(Args&&... args)
            {
                return Impl::room_object2d_create(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto destroy(Args&&... args)
            {
                return Impl::room_object2d_destroy(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto move(Args&&... args)
            {
                return Impl::room_object2d_move(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto createMoving(Args&&... args)
            {
                return Impl::room_object2d_createMoving(std::forward<Args>(args)...);
            }
        };

        struct Player3D
        {
            template <typename... Args>
            static auto position(Args&&... args)
            {
                return Impl::room_player3d_position(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto dimension(Args&&... args)
            {
                return Impl::room_player3d_dimension(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto movement(Args&&... args)
            {
                return Impl::room_player3d_movement(std::forward<Args>(args)...);
            }
        };

        struct Object3D
        {
            template <typename... Args>
            static auto create(Args&&... args)
            {
                return Impl::room_object3d_create(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto destroy(Args&&... args)
            {
                return Impl::room_object3d_destroy(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto move(Args&&... args)
            {
                return Impl::room_object3d_move(std::forward<Args>(args)...);
            }
            template <typename... Args>
            static auto createMoving(Args&&... args)
            {
                return Impl::room_object3d_createmoving(std::forward<Args>(args)...);
            }
        };
    };
};

#endif
