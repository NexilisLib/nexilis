#include <nexilis/dispatcher.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/log.hh>
#include <nexilis/af_inet/udp_client_receiver.hh>

uint8_t setupCommand[] = { 0x10, 0x10, '5', '4', '2', '0', '9' };
uint8_t pingCommand[] = { 0x20, 0x10 };
uint8_t passwdCommand[] = { 0x40, 0x20, 's', 'a', 'l', 'a', 's', 'a', 'n', 'a' };

void startServer()
{
    nexilis::ProtocolManager manager;
    auto server = manager.addProtocol<nexilis::UdpClientReceiver>(54209);
    server.start();

    std::cout << "Server started!" << std::endl;
}

void sendMessageAfterDelay(uint8_t message[], size_t dataSize, std::string msgName)
{
    std::cout << "Calling thread: " << msgName << std::endl;

    // Send the message after a delay
    nexilis::UDPSender sender("192.168.1.85", 54200);
    sender.sendMessage(message, dataSize);
}

int main()
{
    nexilis::Log::startConsoleLogging();

    //auto a = nexilis::Thread()

    // Start the server in one thread
    std::thread serverThread(startServer);

    std::thread messageThread(sendMessageAfterDelay, setupCommand, sizeof(setupCommand), "MESSAGE");

    std::thread pingThread(sendMessageAfterDelay, pingCommand, sizeof(pingCommand), "PING");

    std::thread authenticationThread(sendMessageAfterDelay, passwdCommand, sizeof(passwdCommand), "PASSWD");

    messageThread.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    pingThread.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    serverThread.join();

    return 0;
}
