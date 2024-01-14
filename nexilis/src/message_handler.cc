#include <cstdint>
#include <nexilis/message_handler.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/command.hh>

#include <nexilis/common/util.hh>
#include <sys/types.h>

namespace nexilis
{

MessageHandler::Message MessageHandler::readMessage(std::string address, std::string message, uint16_t port)
{
    Log::info("Received message: ", message, " from ", address, " port", port);

    // Create new client.
    Client client(address);

    // Yeah this needs to be modifier alot.
    // However as long as we get stuff to happen it's fine.
    if (!ClientStorage::contains(address))
    {
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
        else
        {
            Log::info("Message from unregistered user: ", message, " address ", address, "! clientID: ", client.getId());
        }
    }

    Log::info("Message from registered client ", address, " message: ", message);

    std::vector<uint8_t> msg;

    auto command = Command::createVectorFromCommandPtr(message.c_str(), message.size());
    for (uint8_t i = 0; i < command.size(); i++)
    {
        msg.push_back(command[i]);
    }

    size_t index = 0;
    size_t playerId = 0;

    while (index < command.size() && command[index] != 0xFF)
    {
        char digitChar = command[index];
        if (isdigit(digitChar))
        {
            playerId = playerId * 10 + (digitChar - '0');
        }
        index++;
    }

    auto fullCommandInBytes = Command::createVectorFromCommandPtr(message.c_str(), message.size());
    auto readyCommand = Command::removeAmountOfBytesFromVector(fullCommandInBytes, index + 1);

    auto realClient = ClientStorage::getClientById(playerId);
    assert(realClient);

    return Message
    {
        address,
        readyCommand,
        port,
        realClient,
    };
}

}
