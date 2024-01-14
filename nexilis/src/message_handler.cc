#include <cstdint>
#include <nexilis/message_handler.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/command.hh>

#include <nexilis/common/util.hh>
#include <sys/types.h>

namespace nexilis
{

size_t extractSizeFromVector(const std::vector<uint8_t>& data)
{
    size_t result = 0;

    for (auto byte : data)
    {
        if (byte == 0xFF)
        {
            break;
        }
        result = (result << 8) | byte;
    }

    return result;
}

std::vector<uint8_t> removeItemsUntilFF(std::vector<uint8_t>& data)
{
    auto ffPosition = std::find(data.begin(), data.end(), 0xFF);

    if (ffPosition != data.end())
    {
        // Erase items including 0xFF
        return std::vector<uint8_t>(ffPosition + 1, data.end());
    }
    else
    {
        return {};
    }
}

MessageHandler::Message MessageHandler::readMessage(std::string address, std::string message, uint16_t port)
{
    Log::info("Received message: ", message, " from ", address, " port", port);

    // Create new client.
    Client client(address);

    // Magic bytes 0x20, 0x10
    if (message[0] == 0x20 && message[1] == 0x10)
    {
        Log::info("Registering user ", client.getIPAddress());

        std::vector<uint8_t> readyMessage;

        auto idVector = Util::convertToByteVector(client.getId());

        // clientside command.
        readyMessage.push_back(0x10);
        readyMessage.push_back(0x10);

        for (uint8_t i = 0; i < idVector.size(); i++)
        {
            readyMessage.push_back(idVector[i]);
        }

        // Authentication.
        ClientStorage::add(std::move(client));
        auto realClient = ClientStorage::getClientById(client.getId());

        assert(realClient);

        return Message
        {
            address,
            readyMessage,
            port,
            realClient
        };
    }

    std::vector<uint8_t> msg = Util::convertToByteVector(message.c_str(), message.size());
    size_t id = extractSizeFromVector(msg);
    auto realClient = ClientStorage::getClientById(id);

    if (realClient)
    {
        auto readyCommand = removeItemsUntilFF(msg);

        return Message
        {
            address,
            readyCommand,
            port,
            realClient,
        };
    }
    else
    {
        Log::info("Message from unidentified user!");
        return {};
    }
}

}
