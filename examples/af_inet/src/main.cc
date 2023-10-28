#include <chrono>
#include <mutex>
#include <nexilis/protocol_manager.hh>
#include <nexilis/af_inet/udp_sender.hh>

#include <thread>

int main()
{
	nexilis::Log::startConsoleLogging();

	std::mutex mtx;	

	nexilis::ProtocolManager manager;

	// Creating udp server with default port.
	auto udpServer = manager.addProtocol<nexilis::UDPServer>();

	std::thread serverThread([&udpServer, &mtx]()
	{
		while (true)
		{
			std::lock_guard<std::mutex> lock(mtx);;
			udpServer.receiveMessage();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	});

	// Creating a sender instance.
	auto sender = nexilis::UDPSender("0.0.0.0");

	std::thread senderThread([&sender]()
	{
		sender.sendMessage("moika");
	});

	serverThread.join();
	senderThread.join();

	return 0;
}
