#include <nexilis/message_handler.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/command.hh>

namespace nexilis
{

MessageHandler::Message MessageHandler::readMessage(std::string address, std::string message, uint16_t port)
{
    Log::info("Received message: ", message, " from ", address, " port", port);

    Client client(address);
    if (!ClientStorage::contains(address))
    {
        Log::info("The first message of the client: ", address, "! clientID: ", client.getId());
        ClientStorage::add(std::move(client));
    }

    auto command = Command::createVectorFromCommandPtr(message.c_str(), message.size());

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

    return Message
    {
        address,
        readyCommand,
        port,
        realClient,
    };
}

}
