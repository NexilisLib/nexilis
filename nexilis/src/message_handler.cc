#include <cstdint>
#include <nexilis/authentication.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/command.hh>
#include <nexilis/config.hh>
#include <nexilis/message_handler.hh>
#include <nexilis/packet.hh>

#include <nexilis/common/util.hh>

#include <sys/types.h>

namespace nexilis
{

size_t extractSizeFromVector(const std::vector<uint8_t>& data)
{
    size_t result = 0;

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
        for (size_t i = 0; i < data.size(); ++i)
        {
            if (data[i] == 0xFF)
            {
                break;
            }
            result |= static_cast<size_t>(data[i]) << (i * 8);
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

MessageHandler::Message MessageHandler::readMessage(std::string address, std::string message, uint16_t port, Authentication* authentication)
{
    Log::info("Received message: ", message, " from ", address, " port ", port);
    Log::info("Message size: ", message.size());

    // Create new client.
    Client client(address);

    // TODO
    // Error Messages.
    std::vector<uint8_t> errordata = {0xa, 0x10, 0x10};
    Message errorMessage("", errordata, -1, nullptr);

    std::vector<uint8_t> convertedMessage = Util::convertToByteVector(message.c_str(), message.size());

    // If the message contains 0xFF byte we consider this message nexilis message.
    bool normalMessage = containsFF(convertedMessage);
    Client* realClient = nullptr;
    if (normalMessage)
    {
        size_t id = extractSizeFromVector(convertedMessage);

        // Id extraction is successfull.
        if (id)
        {
            Log::debug("Attempt to connect client: ", id);
            auto existingClient = ClientStorage::getClientById(id);

            if (existingClient)
            {
                Log::debug("Connecting client: ", id);
                // Valid state to enter switch (authentication->getMode()).
                realClient = existingClient;
            }
            else
            {
                Log::error("Undefined behaviour");
            }
        }
        else
        {
            Log::error("Undefined behaviour");
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
            if (realClient)
            {
                if (realClient->hasCommonAccess())
                {
                    Log::info("Known client sends a message!");

                    return Message(
                        address,
                        removeItemsUntilFF(convertedMessage),
                        port,
                        realClient);
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
                if (authentication->isCommonPassword(std::string(message)))
                {
                    Log::info("Correct password by user ", client.getId());
                    client.setCommonAccess(true);

                    size_t newClientId = client.getId();
                    ClientStorage::add(std::move(client));
                    auto realNewClient = ClientStorage::getClientById(newClientId);

                    assert(realNewClient);
                    assert(client.getId() == realNewClient->getId());

                    std::vector<uint8_t> message{0x20, 0x10};
                    std::vector<uint8_t> idBytes = Util::convertToByteVector(realNewClient->getId());
                    for (size_t i = 0; i < idBytes.size(); i++)
                    {
                        message.push_back(idBytes[i]);
                    }

                    return Message(
                        address,
                        message,
                        port,
                        realNewClient);
                }
                else
                {
                    Log::error("Unidentified client sends message other than password");
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
