#ifndef NEXILIS_MESSAGE_HANDLER_HH
#define NEXILIS_MESSAGE_HANDLER_HH

#include <nexilis/client.hh>

#include <vector>

namespace nexilis
{

class MessageHandler
{
public:
    struct Message
    {
        std::string address;
        std::vector<uint8_t> message;
        uint16_t port = 0;
        Client* client = nullptr;
    };

    Message readMessage(std::string address, std::string message, uint16_t port);
};

}

#endif
