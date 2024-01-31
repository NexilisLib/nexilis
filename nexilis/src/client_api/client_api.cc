#include <nexilis/client_api/client_api.hh>

namespace nexilis
{

bool ClientAPI::readMessage(std::vector<uint8_t> message)
{
    for (uint8_t commandByte : message)
    {
        std::cout << "Commandbyte hex: " << std::hex << static_cast<int>(commandByte);
        std::cout << std::endl;
        std::cout << "Commandbyte char: " <<  static_cast<char>(commandByte);
    }

    switch (message.front())
    {
        // Set
        case 0x10:
        {
            switch (message[1])
            {
                // Client ID.
                case 0x10:
                {
                    auto sizeVector = Util::removeAmountOfBytesFromVector(message, 2);
                    auto id = Util::convertToType<size_t>(sizeVector);
                    setClientId(&id);
                    return true;
                }

                default: return false;
            }

        }

        // Get.
        case 0x20:
        {
            switch (message[1])
            {
                case 0x10:
                {
                    size_t clientId = Util::convertToType<size_t>(Util::removeAmountOfBytesFromVector(message, 2));
                    std::cout << "client id set to" << clientId << std::endl;
                    setClientId(&clientId);
                    return true;
                }

                default: return false;
            }
        }

        default: return false;
    }

}

}

