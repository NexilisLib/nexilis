#include <nexilis/server/command/command.hh>
#include <nexilis/server/movement.hh>
#include <nexilis/server/room_storage.hh>

namespace nexilis::server
{

ServerConfig* Movement::m_settings = nullptr;

bool Movement::isInitialized()
{
    return m_settings;
}

void Movement::_initialize(ServerConfig& settings)
{
    m_settings = &settings;
}

double Movement::easing(double progress, double totalDistance)
{
    double easedValue = progress * progress;
    double messageValue = totalDistance * easedValue;
    return messageValue;
}

double Movement::linear(double progress, double totalDistance)
{
    return totalDistance * progress;
}

std::thread Movement::object2D(std::unique_ptr<Movement2D> movement, User& user, Protocol& protocol)
{
    // clang-format off
    return std::thread([&movement, &user, &protocol]()
    {
        try
        {
            auto* room = RoomStorage::getRoomById(user.getRoomId());
            auto* serverObject = room->getObject2DById(movement->getObjectId());
            // Capture start position so each tick computes startPos + f(progress)
            // instead of accumulating offsets onto an already-moved position.
            Vector2f startPosition = serverObject->getPosition();

            Command::runWithTickrate(m_settings->getTickrate(), movement->getDeltatime(), [&movement, &user, &protocol, startPosition](double progress)
            {
                auto func = movement->getMovementFunc();
                auto amount = movement->getAmount();

                auto* room = RoomStorage::getRoomById(user.getRoomId());
                auto* serverObject = room->getObject2DById(movement->getObjectId());

                Vector2f newPosition = Vector2f(startPosition.x + func(progress, amount.x),
                                               startPosition.y + func(progress, amount.y));

                serverObject->setPosition(newPosition);
                std::map<std::string, boost::json::value> messageParams
                {
                    {"createMovingType", boost::json::value("update")},
                    {"objectId", boost::json::value(serverObject->getId())},
                    {"x", boost::json::value(newPosition.x)},
                    {"y", boost::json::value(newPosition.y)}
                };

                auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, movement->getMessageData(), messageParams, movement->getMessageId());
                Command::sendRoomCommand(roomCommand, user, protocol);
            });
        }
        catch (const std::exception& e)
        {
            Log::error(e.what());
        }
    });
    // clang-format on
}

std::thread Movement::object3D(std::unique_ptr<Movement3D> movement, User& user, Protocol& protocol)
{
    // clang-format off
    return std::thread([&movement, &user, &protocol]()
    {
        try
        {
            auto* room = RoomStorage::getRoomById(user.getRoomId());
            auto* serverObject = room->getObject3DById(movement->getObjectId());
            // Capture start position so each tick computes startPos + f(progress)
            // instead of accumulating offsets onto an already-moved position.
            auto startPosition = serverObject->getPosition();

            Command::runWithTickrate(m_settings->getTickrate(), movement->getDeltatime(), [&movement, &user, &protocol, startPosition](double progress)
            {
                auto func = movement->getMovementFunc();
                auto amount = movement->getAmount();

                auto* room = RoomStorage::getRoomById(user.getRoomId());
                auto* serverObject = room->getObject3DById(movement->getObjectId());

                Vector3f newPosition = Vector3f(startPosition.x + func(progress, amount.x),
                                               startPosition.y + func(progress, amount.y),
                                               startPosition.z + func(progress, amount.z));

                serverObject->setPosition(newPosition);
                std::map<std::string, boost::json::value> messageParams
                {
                    {"createMovingType", boost::json::value("update")},
                    {"objectId", boost::json::value(serverObject->getId())},
                    {"x", boost::json::value(newPosition.x)},
                    {"y", boost::json::value(newPosition.y)},
                    {"z", boost::json::value(newPosition.z)}
                };

                auto roomCommand = Command::createRoomCommand(user.getRoomId(), user, movement->getMessageData(), messageParams, movement->getMessageId());
                Command::sendRoomCommand(roomCommand, user, protocol);
            });
        }
        catch (const std::exception& e)
        {
            Log::error(e.what());
        }
    });
    // clang-format on
}

} // namespace nexilis::server
