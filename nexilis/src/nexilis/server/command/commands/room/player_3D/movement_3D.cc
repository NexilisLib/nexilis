#include <nexilis/server/command/command.hh>
#include <nexilis/server/command/commands.hh>
#include <nexilis/server/movement.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

CommandResult ServerImpl::room_player3d_movement(const DefaultArgs& args)
{
    auto& user = args.getUser();
    auto& protocol = args.getProtocol();
    auto messageId = args.getMessageId();
    auto data = args.getData();

    auto payload = Util::removeAmountOfBytesFromVector(data, 3);
    auto vec_x = Util::floatFromFront(payload);
    auto vec_y = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 4));
    auto vec_z = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 8));
    auto delta = Util::floatFromFront(Util::removeAmountOfBytesFromVector(payload, 12));
    auto movement_vector = Vector3f(vec_x, vec_y, vec_z);
    auto mtx = std::make_shared<std::mutex>();

    // clang-format off
    std::thread([mtx, movement_vector, &user, data, &protocol, &messageId, delta]()
    {
        try
        {
        Command::runWithTickrate(60.0f, delta,
                [&mtx, movement_vector, &user, data, &protocol, &messageId](double progress)
            {
                auto* room = RoomStorage::getRoomById(user.getRoomId());
                if (!room)
                {
                    Log::warning("room_player3d_movement: room not found for user ", user.getRoomId());
                    return;
                }

                double eased_x = Movement::easing(progress, movement_vector.x);
                double eased_y = Movement::easing(progress, movement_vector.y);
                double eased_z = Movement::easing(progress, movement_vector.z);

                auto current_pos = user.getObject3D().getPosition();
                auto new_pos = Vector3f(eased_x + current_pos.x, eased_y + current_pos.y, eased_z + current_pos.z);

                // TODO validation here.

                {
                    std::lock_guard<std::mutex> lock(*mtx);
                    user.getObject3D().setPosition(new_pos);
                    std::map<std::string, boost::json::value> params
                    {
                        {"x", boost::json::value(new_pos.x)},
                        {"y", boost::json::value(new_pos.y)},
                        {"z", boost::json::value(new_pos.z)},
                    };
                    auto room_command = Command::createRoomCommand(user.getRoomId(), user, data, params, messageId);
                    Command::sendRoomCommand(room_command, user, protocol);
                }
            });
        }
        catch (const std::exception& e)
        {
            Log::error("Error with 3D movement thread: ", e.what());
        }
        catch (...)
        {
            Log::error("Other error with 3D movement thread");
        }
    })
    .detach();
    // clang-format on
    return CommandResult::success;
}

} // namespace nexilis::server
