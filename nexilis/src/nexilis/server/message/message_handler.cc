#include <nexilis/server/message/auth_message.hh>
#include <nexilis/server/message/error_message.hh>
#include <nexilis/server/message/message_handler.hh>

#include <nexilis/server/client_storage.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/config.hh>
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

std::unique_ptr<BaseMessage> MessageHandler::readMessage(std::string address, const nx_data& payload, Settings* authentication)
{
    Log::debug(header(), "Payload size: ", payload.size());
    Util::debugUint8Vector(payload);

    auto clientId = Util::uint64FromFront(payload);
    if (clientId == 0)
    {
        Log::error(header(), "Client id is zero");
        return std::make_unique<ErrorMessage>(address, ErrorMessage::Type::client_id_failure);
    }

    bool newUser = false;

    // Does the user actually exist?
    auto* user = ClientStorage::getClientById(clientId);
    if (!user)
    {
        // Create a new user.
        uint64_t newId = Util::getRandomUint64();
        User newlyCreatedUser(newId, address);
        Log::info(header(), "Created new user: ", newId);
        auto username = Util::getRandomString(10);
        newlyCreatedUser.setUsername(username);
        user = &newlyCreatedUser;
        newUser = true;
    }
    assert(user);

    switch (authentication->getMode())
    {
        case AuthenticationMode::empty:
        {
            return std::make_unique<ErrorMessage>(address, ErrorMessage::Type::empty_authentication_mode);
        }
        case AuthenticationMode::skip:
        {
            return std::make_unique<Message>(handlePayload(payload, user, address));
        }
        case AuthenticationMode::admin_access:
        case AuthenticationMode::root_access:
        {
            Log::error(header(), "Not implemented!");
            return std::make_unique<ErrorMessage>(address, ErrorMessage::Type::not_implemented);
        }
        case AuthenticationMode::password_protected:
        {
            if (!newUser)
            {
                if (user->hasCommonAccess())
                {
                    Log::info(header(), "Message from known user");
                    return std::make_unique<Message>(handlePayload(payload, user, address));
                }
                // Message from verified client that has no access.
                else
                {
                    Log::error(header(), "Message from verified client that has no access");
                    return std::make_unique<ErrorMessage>(address, ErrorMessage::Type::no_access);
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
                    nx_data message{1, 0, 0};
                    auto idBytes = Util::convertToByteVector(user->getId());
                    for (auto&& byte : idBytes)
                    {
                        message.emplace_back(byte);
                    }
                    std::vector<nx_data> test{message};

                    auto new_message_id = Util::getRandomUint64();
                    BaseMessage::Data base_data(new_message_id, address, realNewClient);
                    return std::make_unique<AuthMessage>(std::move(base_data), test);
                }
                else
                {
                    Log::error(header(), "Authentication error, instead of password we got: \n");
                    Util::debugUint8Vector(payload);
                    return std::make_unique<ErrorMessage>(address, ErrorMessage::Type::authentication_error);
                }
            }
        }
        default:
        {
            Log::error(header(), "Missing authentication mode");
            return std::make_unique<ErrorMessage>(address, ErrorMessage::Type::missing_authentication_mode);
        }
    }
}

Message MessageHandler::handlePayload(const nx_data& payload, User* user, const std::string& address)
{
    // Vector without client id (8 bytes).
    auto vectorWithoutClientId = Util::removeAmountOfBytesFromVector(payload, 8);

    // Next eight bytes is the message id.
    uint64_t messageId = Util::uint64FromFront(vectorWithoutClientId);

    // Vector without message id (8 bytes).
    auto messageVector = Util::removeAmountOfBytesFromVector(vectorWithoutClientId, 8);

    BaseMessage::Data base_data(messageId, address, user);
    return Message(std::move(base_data), messageVector);
}

} // namespace nexilis::server
