#include <nexilis/authentication.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/command.hh>
#include <nexilis/config.hh>
#include <nexilis/json.hh>
#include <nexilis/message_handler.hh>
#include <nexilis/packet.hh>
#include <nexilis/log.hh>

#include <nexilis/common/util.hh>

namespace nexilis
{

uint64_t extractUint64FromVector(const std::vector<uint8_t>& data)
{
    uint64_t result = 0;

    if (Config::getBigEndian())
    {
        for (auto byte : data)
        {
            if (byte == 0xFF)
            {
                break;
            }
            result = (result << 8) | byte;
        }
    }
    else
    {
        for (uint64_t i = 0; i < data.size(); ++i)
        {
            if (data[i] == 0xFF)
            {
                break;
            }
            result |= static_cast<uint64_t>(data[i]) << (i * 8);
        }
    }

    return result;
}

std::vector<uint8_t> removeItemsUntilFF(const std::vector<uint8_t>& data)
{
    auto ffPosition = std::find(data.begin(), data.end(), 0xFF);

    if (ffPosition != data.end())
    {
        return std::vector<uint8_t>(ffPosition + 1, data.end());
    }
    else
    {
        return {};
    }
}

bool containsFF(const std::vector<uint8_t>& data)
{
    for (auto byte : data)
    {
        if (byte == 0xFF)
        {
            return true;
        }
    }
    return false;
}
// NEXILIS_ERROR("myfilename", ErrorType::NOT_IMPLEMENTED);

MessageHandler::Message MessageHandler::readMessage(std::string address, const std::vector<uint8_t>& payload, uint16_t port, Authentication* authentication)
{
    Log::debug("Payload size: ", payload.size());

    // Create a new client.
    User user(address);

    // TODO
    // Error Messages.
    std::vector<uint8_t> errordata = {9, 0, 0};
    Message errorMessage("", errordata, -1, nullptr, 0);

    // If the message contains 0xFF byte we consider this message nexilis message.
    bool normalMessage = containsFF(payload);
    User* realUser = nullptr;
    if (normalMessage)
    {
        uint64_t id = extractUint64FromVector(payload);

        // Id extraction is successfull.
        if (id)
        {
            Log::debug("Message from client: ", id);
            auto existingUser = ClientStorage::getClientById(id);

            if (existingUser)
            {
                Log::debug("Existing client: ", id);

                // Valid state to enter switch (authentication->getMode()).
                realUser = existingUser;
            }
            else
            {
                Log::error("Trying to send messages without id");
                Log::error("TODO send error message");
            }
        }
        else
        {
            Log::error("Trying to send messages without id");
            Log::error("TODO send error message");
        }
    }
    else
    {
        Log::debug("NO ID IN THE MESSAGE");
    }

    switch (authentication->getMode())
    {
        case Authentication::Mode::free:
        {
            Log::error("Not implemented!");
            return errorMessage;
        }
        case Authentication::Mode::whiteListed:
        {
            Log::error("Not implemented!");
            return errorMessage;
        }
        case Authentication::Mode::passwordProtected:
        {
            if (realUser)
            {
                if (realUser->hasCommonAccess())
                {
                    Log::info("Known client sends a message!");

                    auto a = removeItemsUntilFF(payload);
                    uint64_t messageId = extractUint64FromVector(payload);
                    auto b = removeItemsUntilFF(a);

                    return Message(
                            address,
                            b,
                            port,
                            realUser,
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
                    Log::info("Correct password by user ", user.getId());
                    user.setCommonAccess(true);

                    uint64_t newClientId = user.getId();
                    ClientStorage::add(std::move(user));
                    auto realNewClient = ClientStorage::getClientById(newClientId);

                    // Checking successfull client creation.
                    assert(realNewClient);
                    assert(user.getId() == realNewClient->getId());

                    // This message is equal to Packet::getId (without client id).
                    std::vector<uint8_t> message{1, 0};
                    std::vector<uint8_t> idBytes = Util::convertToByteVector(user.getId());
                    for (uint64_t i = 0; i < idBytes.size(); i++)
                    {
                        message.push_back(idBytes[i]);
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
                    Log::error("PRINTING ERROR SEQUENCE as CHARS:");

                    for (auto i : payload)
                    {
                        Log::info(static_cast<char>(i));
                    }

                    Log::error("PRINTING ERROR SEQUENCE as INTEGERS:");
                    for (auto i : payload)
                    {
                        Log::info(static_cast<int>(i));
                    }

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
