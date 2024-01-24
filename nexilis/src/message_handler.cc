#include "nexilis/authentication.hh"
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

MessageHandler::Message MessageHandler::readMessage(std::string address, std::string message, uint16_t port, Authentication* authentication)
{
    Log::info("Received message: ", message, " from ", address, " port ", port);

    // Create new client.
    Client client(address);

    // We do the authentication here.
    switch (authentication->getMode())
    {
        case Authentication::Mode::free:
        {
            Log::error("Not implemented!");
            return {};
        }
        case Authentication::Mode::whiteListed:
        {
            Log::error("Not implemented!");
            return {};
        }
        case Authentication::Mode::passwordProtected:
        {
            if (client.hasCommonAccess())
            {
                std::cout << "Client has common access!" << std::endl;
                break;
            }
            else
            {
                // Only accept the password as a message from unidentied clients.
                if (authentication->isCommonPassword(std::string(message)))
                {
                    Log::info("Correct password by user ", client.getId());
                    client.setCommonAccess(true);

                    size_t clientId = client.getId();
                    ClientStorage::add(std::move(client));
                    auto realClient = ClientStorage::getClientById(clientId);
                    assert(realClient);

                    std::vector<uint8_t> message { 0x20, 0x10 };

                    std::vector<uint8_t> idBytes = Util::convertToByteVector(realClient->getId());
                    for (size_t i = 0; i < idBytes.size(); i++)
                    {
                        message.push_back(idBytes[i]);
                    }

                    return Message
                    {
                        address,
                        std::move(message),
                        port,
                        realClient
                    };
                }
                else
                {
                    std::cout << "PASSWORD WAS NOT CORRECT" << std::endl;
                }
            }
        }
        default:
        {
            Log::error("Missing authentication mode");
            return {};
        }
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
