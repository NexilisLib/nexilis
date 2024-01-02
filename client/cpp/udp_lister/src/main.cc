#include <nexilis/dispatcher.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/log.hh>

void startServer()
{
    nexilis::ProtocolManager manager;
    auto server = manager.addProtocol<nexilis::AfInetUdpServer>(54209);
    server.start();
}

void sendMessageAfterDelay()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));

    unsigned char bytesToSend[] = { 0x10, 0x10 };
    size_t dataSize = sizeof(bytesToSend);

    // Send the message after a delay
    nexilis::UDPSender sender("192.168.1.85", 54200);
    sender.sendMessage(bytesToSend, dataSize);
}

int main()
{
    nexilis::Log::startConsoleLogging();

    // Start the server in one thread
    std::thread serverThread(startServer);

    // Wait for one second before sending the message in another thread
    std::thread messageThread(sendMessageAfterDelay);

    // Join the threads to ensure they complete before exiting
    serverThread.join();
    messageThread.join();

    return 0;
}
