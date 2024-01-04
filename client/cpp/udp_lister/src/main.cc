#include <algorithm>
#include <nexilis/dispatcher.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/log.hh>
#include <nexilis/af_inet/udp_client_receiver.hh>

#include <mutex>
#include <condition_variable>

unsigned char setupCommand[] = { 0x10, 0x10, '5', '4', '2', '0', '9' };
unsigned char pingCommand[] = { 0x20, 0x10 };

std::mutex mtx;

void startServer()
{
    nexilis::ProtocolManager manager;
    auto server = manager.addProtocol<nexilis::UdpClientReceiver>(54209);
    server.start();
}

void sendMessageAfterDelay(unsigned char message[], size_t dataSize, std::string msgName)
{
    std::cout << "Calling thread: " << msgName << std::endl;

    // Send the message after a delay
    nexilis::UDPSender sender("192.168.1.85", 54200);
    sender.sendMessage(message, dataSize);
}

int main()
{
    nexilis::Log::startConsoleLogging();

    // Start the server in one thread
    std::thread serverThread(startServer);

    std::thread messageThread(sendMessageAfterDelay, setupCommand, sizeof(setupCommand), "MESSAGE");

    std::thread pingThread(sendMessageAfterDelay, pingCommand, sizeof(pingCommand), "PING");

    messageThread.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    pingThread.join();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    serverThread.join();

    return 0;
}
