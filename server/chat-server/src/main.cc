// nexilis libs
#include <nexilis/log.hh>
#include <nexilis/mysql/database.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/room.hh>
#include <nexilis/room_storage.hh>
#include <nexilis/server_manager.hh>

// nexilis protocols
#include <nexilis/boost/tcp_server.hh>
#include <nexilis/boost/udp_server.hh>

int main()
{
    nexilis::Log::startConsoleDebugging();

    nexilis::Authentication auth;
    auth.setMode(nexilis::Authentication::Mode::passwordProtected);
    auth.setCommonPassword("salasana");

    nexilis::ServerManager serverManager;
    serverManager.setAuthentication(auth);

    nexilis::ProtocolManager protocolManager;

    // Boost TCP
    auto boostTCPServer = protocolManager.createProtocol<nexilis::boost::TCPServer>(12348);
    boostTCPServer.start();
    std::cout << "nexilis boost TCP ready" << std::endl;

    /*
    // Boost UDP
    auto boostUDPServer = protocolManager.createProtocol<nexilis::boost::UDPServer>(12345);
    boostUDPServer.start();
    std::cout << "nexilis boost UDP ready" << std::endl;
    */

    // Add some rooms.
    auto room1 = nexilis::Room(nexilis::Room::Settings("first room", 30));
    auto room2 = nexilis::Room(nexilis::Room::Settings("second room", 60));
    auto room3 = nexilis::Room(nexilis::Room::Settings("third room", 60));

    nexilis::RoomStorage::add(std::move(room1));
    nexilis::RoomStorage::add(std::move(room2));
    nexilis::RoomStorage::add(std::move(room3));

    return 0;
}
