#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command.hh>
#include <nexilis/server/config.hh>
#include <nexilis/server/message/message_handler.hh>
#include <nexilis/server/room_storage.hh>
#include <nexilis/server/server_json.hh>
#include <nexilis/server/settings.hh>

#include <nexilis/logger/log.hh>

#include <nexilis/util.hh>

namespace nexilis::server
{

MessageHandler::MessageHandler()
    : NxClass("server::MessageHandler")
{
}

// Payload handled by this class:
// Client id 8 bytes
// Message id 8 bytes
// Command bytes (at least 2 bytes), second parameter of MessageHandler::Message.

Message MessageHandler::readMessage(std::string address, const nx_data& payload, uint16_t port, Settings* authentication)
{
    Log::debug(header(), "Payload size: ", payload.size());
    Util::debugUint8Vector(payload);

    // TODO
    // Error Messages.
    nx_data errordata = {9, 0, 0};
    BaseMessage base_error_message(0, "", -1, nullptr);
    Message errorMessage(std::move(base_error_message), errordata);

    auto clientId = Util::uint64FromFront(payload);
    if (clientId == 0)
    {
        Log::error(header(), "Client id is zero");
        return errorMessage;
    }

    // FIXME
    // First message is ussumed different in AuthenticationMode::passwordProtected.
    bool userAlreadyExists = true;

    // Does the user actually exist?
    auto* user = ClientStorage::getClientById(clientId);
    if (!user)
    {
        // Create a new user.
        uint64_t newId = Util::getRandomUint64();
        User newUser(newId, address);
        Log::info(header(), "Created new user: ", newId);
        auto username = Util::getRandomString(10);
        newUser.setUsername(username);
        user = &newUser;
        userAlreadyExists = false;
    }
    assert(user);

    switch (authentication->getMode())
    {
        case AuthenticationMode::empty:
        {
            return errorMessage;
        }
        case AuthenticationMode::skip:
        {
            return handlePayload(payload, user, address, port);
        }
        case AuthenticationMode::admin_access:
        case AuthenticationMode::root_access:
        {
            Log::error(header(), "Not implemented!");
            return errorMessage;
        }
        case AuthenticationMode::password_protected:
        {
            if (userAlreadyExists)
            {
                if (user->hasCommonAccess())
                {
                    Log::info(header(), "Access successfull");

                    return handlePayload(payload, user, address, port);
                }
                // Message from verified client that has no access.
                else
                {
                    Log::error(header(), "Message from verified client that has no access");
                    return errorMessage;
                }
            }
            else
            {
                // Only accept the password as a message from unidentied clients.
                // Normally string conversion is avoided throughout nexilis, but this one stays for obvious reasons.
                if (authentication->isPassphrase(Util::convertToString(payload)))
                {
                    Log::info(header(), "Correct password by user ", user->getId());
                    user->setCommonAccess(true);

                    uint64_t newClientId = user->getId();
                    ClientStorage::add(std::move(*user));
                    auto realNewClient = ClientStorage::getClientById(newClientId);

                    // Checking successfull client creation.
                    assert(realNewClient);
                    assert(user->getId() == realNewClient->getId());

                    // This message is equal to Packet::getId (without client id).
                    // TODO add other data such as port number.
                    nx_data message{1, 0};
                    auto idBytes = Util::convertToByteVector(user->getId());
                    for (auto&& byte : idBytes)
                    {
                        message.emplace_back(byte);
                    }

                    auto new_message_id = Util::getRandomUint64();

                    BaseMessage base_message(new_message_id, address, port, realNewClient);

                    return Message(std::move(base_message), message);
                }
                else
                {
                    Log::error(header(), "Authentication error");
                    return errorMessage;
                }
            }
        }
        default:
        {
            Log::error(header(), "Missing authentication mode");
            return errorMessage;
        }
    }
}

Message MessageHandler::handlePayload(const nx_data& payload, User* user, const std::string& address, uint16_t port)
{
    // Vector without client id (8 bytes).
    auto vectorWithoutClientId = Util::removeAmountOfBytesFromVector(payload, 8);

    // Next eight bytes is the message id.
    uint64_t messageId = Util::uint64FromFront(vectorWithoutClientId);

    // Vector without message id (8 bytes).
    auto messageVector = Util::removeAmountOfBytesFromVector(vectorWithoutClientId, 8);

    BaseMessage base_message(messageId, address, port, user);

    return Message(std::move(base_message), messageVector);
}

} // namespace nexilis::server
