#include <nexilis/authentication.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/command.hh>
#include <nexilis/config.hh>
#include <nexilis/json.hh>
#include <nexilis/log.hh>
#include <nexilis/message_handler.hh>
#include <nexilis/packet.hh>
#include <nexilis/room_storage.hh>

#include <nexilis/common/util.hh>

namespace nexilis
{

// Payload handled by this class:
// Client id 8 bytes
// Message id 8 bytes
// Command bytes (at least 2 bytes), second parameter of MessageHandler::Message.

MessageHandler::Message MessageHandler::readMessage(std::string address, const std::vector<uint8_t>& payload, uint16_t port, Authentication* authentication)
{
    Log::debug("Payload size: ", payload.size());

    // TODO
    // Error Messages.
    std::vector<uint8_t> errordata = {9, 0, 0};
    Message errorMessage("", errordata, -1, nullptr, 0);

    auto clientId = Util::uint64FromFront(payload);
    auto* user = ClientStorage::getClientById(clientId);
    bool userAlreadyExists = true;

    if (!user)
    {
        // Create a new user.
        Log::info("Creating new user");
        uint64_t newId = Util::getRandomUint64();
        User newUser(newId, address);
        auto username = Util::getRandomString(10);
        newUser.setUsername(username);
        user = &newUser;
        userAlreadyExists = false;
    }
    assert(user);

    switch (authentication->getMode())
    {
        case Authentication::AuthenticationMode::free:
        {
            Log::error("Not implemented!");
            return errorMessage;
        }
        case Authentication::AuthenticationMode::whiteListed:
        {
            Log::error("Not implemented!");
            return errorMessage;
        }
        case Authentication::AuthenticationMode::passwordProtected:
        {
            if (userAlreadyExists)
            {
                if (user->hasCommonAccess())
                {
                    Log::info("Known client sends a message!");

                    // Vector without client id (8 bytes).
                    auto vectorWithoutClientId = Util::removeAmountOfBytesFromVector(payload, 8);

                    // Next eight bytes is the message id.
                    uint64_t messageId = Util::uint64FromFront(vectorWithoutClientId);

                    // Vector without message id (8 bytes).
                    auto messageVector = Util::removeAmountOfBytesFromVector(vectorWithoutClientId, 8);

                    return Message(
                            address,
                            messageVector,
                            port,
                            user,
                            messageId);
                }
                // Message from verified client that has no access.
                else
                {
                    Log::error("Message from verified client that has no access");
                    return errorMessage;
                }
            }
            else
            {
                // Only accept the password as a message from unidentied clients.
                // Normally string conversion is avoided throughout nexilis, but this one stays for obvious reasons.
                if (authentication->isPassphrase(Util::convertToString(payload)))
                {
                    Log::info("Correct password by user ", user->getId());
                    user->setCommonAccess(true);

                    uint64_t newClientId = user->getId();
                    ClientStorage::add(std::move(*user));
                    auto realNewClient = ClientStorage::getClientById(newClientId);

                    // Checking successfull client creation.
                    assert(realNewClient);
                    assert(user->getId() == realNewClient->getId());

                    // This message is equal to Packet::getId (without client id).
                    std::vector<uint8_t> message{1, 0};
                    auto idBytes = Util::convertToByteVector(user->getId());
                    for (auto&& byte : idBytes)
                    {
                        message.emplace_back(byte);
                    }
                    return Message(
                            address,
                            message,
                            port,
                            realNewClient,
                            0);
                }
                else
                {
                    Log::error("NEW MESSAGE WHICH IS IS NOT PASSWORD");
                    return errorMessage;
                }
            }
        }
        default:
        {
            Log::error("Missing authentication mode");
            return errorMessage;
        }
    }
}

} // namespace nexilis
