#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/command.hh>
#include <nexilis/client_storage.hh>

namespace nexilis
{

AfInetUdpServer::AfInetUdpServer(unsigned port)
    : BaseUdpServer(port)
{
}

AfInetUdpServer::~AfInetUdpServer()
{
}

void AfInetUdpServer::start()
{
    BaseUdpServer::start();

    while (true)
    {
        BaseUdpServer::Message msg;

        if (BaseUdpServer::getNextMessage(msg))
        {
            Log::info("Received message: ", msg.message, " from ", msg.m_address, " port", msg.port);

            Client client(msg.m_address);
            if (!ClientStorage::contains(msg.m_address))
            {
                Log::info("The first message of the client: ", msg.m_address, "! clientID: ", client.getId());
                ClientStorage::add(std::move(client));
            }

            auto command = Command::createVectorFromCommandPtr(msg.message.c_str(), msg.message.size());

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

            auto realClient = ClientStorage::getClientById(playerId);

            if (realClient)
            {
                auto readyCommand = Command::removeAmountOfBytesFromVector(command, index + 1);

                if (!Command::read(readyCommand, *realClient, *this, true))
                {
                    Log::error("UDP server message reading error, message: ", msg.message);
                }
            }
            else
            {
                Log::info("Message from unauthorized client!");
            }
        }
    }
}

} // namespace nexilis
