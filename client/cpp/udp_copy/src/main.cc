#include <nexilis/dispatcher.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/log.hh>
#include <nexilis/af_inet/udp_client_receiver.hh>

uint8_t setupCommand[] = { 0x10, 0x10, '5', '4', '2', '1', '0' };
uint8_t pingCommand[] = { 0x20, 0x10 };
uint8_t rootPasswdCommand[] = { 0x40, 0x20, 's', 'a', 'l', 'a', 's', 'a', 'n', 'a' };
uint8_t commonPasswdCommand[] = { 0x40, 0x30, 'c', 'o', 'm', 'm', 'o', 'n' };
uint8_t chatCommand[] = { 0x70, 0x10, 0x10, 'm', 'o', 'i', 'k', 'a' };

void startServer()
{
    nexilis::ProtocolManager manager;
    auto server = manager.addProtocol<nexilis::UdpClientReceiver>(54210);
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

    std::thread authenticationThread(sendMessageAfterDelay, rootPasswdCommand, sizeof(rootPasswdCommand), "ROOTPASSWD");
    std::thread authenticationThread2(sendMessageAfterDelay, commonPasswdCommand, sizeof(commonPasswdCommand), "COMMONPASSWD");


    std::thread chatThread(sendMessageAfterDelay, chatCommand, sizeof(chatCommand), "CHAT THREAD");

    messageThread.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    pingThread.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    chatThread.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    serverThread.join();

    return 0;
}
