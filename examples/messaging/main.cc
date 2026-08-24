#include <nexilis/client/client_api.hh>
#include <nexilis/client/packet.hh>
#include <nexilis/client/protocol/nxboost/tcp_client.hh>
#include <nexilis/logger/log.hh>
#include <nexilis/protocol_manager.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace
{

template <typename Predicate>
bool waitFor(Predicate&& predicate,
             std::chrono::milliseconds timeout = std::chrono::seconds(10))
{
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (!predicate() && std::chrono::steady_clock::now() < deadline)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return predicate();
}

/// Print all room messages that have not been shown yet.
void printNewMessages(nexilis::client::ClientAPI& api, uint64_t room_id,
                      const std::string& listener, std::size_t& printed)
{
    auto* room = api.getRoom(room_id);
    if (!room)
    {
        return;
    }

    const auto count = room->getMessages().size();
    for (; printed < count; ++printed)
    {
        const auto& message = room->getMessages()[printed];
        const auto* sender = message.getClient();
        const auto& name = sender ? sender->getUsername() : std::string("unknown");
        std::cout << "[" << listener << " heard " << name << "] " << message.getPayload() << std::endl;
    }
}

} // namespace

int main()
{
    nexilis::Log::startConsoleDebugging();

    nexilis::ProtocolManager protocol_manager;

    // Server.
    nexilis::server::ServerConfig settings;
    settings.setMode(nexilis::server::AuthenticationMode::password_protected);
    settings.setPassphrase("salasana");
    settings.setRootPassword("root");

    auto server = protocol_manager.createProtocol<nexilis::server::nxboost::TCPServer>(settings);
    server.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Alice connects and hosts a chat room.
    nexilis::client::ClientConfig server_data;
    server_data.setPassword("salasana");
    server_data.setBoostTCPAddress("127.0.0.1");
    server_data.setMode(nexilis::server::AuthenticationMode::password_protected);

    nexilis::client::ClientAPI alice_api(server_data);
    auto alice = protocol_manager.createProtocol<nexilis::client::nxboost::TCPClient>(alice_api);
    alice.start();

    alice.sendMessage(nexilis::client::Packet::Set::General::username(alice_api, "Alice"));
    alice.sendMessage(nexilis::client::Packet::Room::Management::create(alice_api, "chat"));

    if (!waitFor([&]()
                 { return !alice_api.getActiveRooms().empty(); }))
    {
        std::cerr << "Alice failed to create the chat room" << std::endl;
        return EXIT_FAILURE;
    }

    const uint64_t room_id = alice_api.getActiveRooms()[0].getId();
    alice.sendMessage(nexilis::client::Packet::Room::Management::join(alice_api, room_id));

    if (!waitFor([&]()
                 { return alice_api.clientInRoom(); }))
    {
        std::cerr << "Alice failed to join the chat room" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Alice is hosting room \"chat\" (" << room_id << ")" << std::endl;

    // Bob connects, finds the chat room and joins it.
    nexilis::client::ClientConfig bob_data;
    bob_data.setPassword("salasana");
    bob_data.setBoostTCPAddress("127.0.0.1");
    bob_data.setMode(nexilis::server::AuthenticationMode::password_protected);

    nexilis::client::ClientAPI bob_api(bob_data);
    auto bob = protocol_manager.createProtocol<nexilis::client::nxboost::TCPClient>(bob_api);
    bob.start();

    bob.sendMessage(nexilis::client::Packet::Set::General::username(bob_api, "Bob"));
    bob.sendMessage(nexilis::client::Packet::Get::Info::rooms(bob_api));

    if (!waitFor([&]()
                 { return !bob_api.getActiveRooms().empty(); }))
    {
        std::cerr << "Bob failed to discover any rooms" << std::endl;
        return EXIT_FAILURE;
    }

    bob.sendMessage(nexilis::client::Packet::Room::Management::join(bob_api, room_id));

    if (!waitFor([&]()
                 { return bob_api.clientInRoom(); }))
    {
        std::cerr << "Bob failed to join the chat room" << std::endl;
        return EXIT_FAILURE;
    }

    if (!waitFor([&]()
                 {
                     auto* room = alice_api.getRoom(room_id);
                     return room && room->getClients().size() == 2; }))
    {
        std::cerr << "Alice never saw Bob join the chat room" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Bob joined the room, both clients are ready" << std::endl;

    // The conversation. Every broadcast is delivered to everyone in the
    // room, including the sender.
    struct Turn
    {
        nexilis::client::nxboost::TCPClient& sender;
        nexilis::client::ClientAPI& api;
        std::string text;
    };

    const std::vector<Turn> conversation = {
            {alice, alice_api, "Hi Bob! Alice here."},
            {bob, bob_api, "Hey Alice! Nice to meet you."},
            {alice, alice_api, "How do you like Nexilis so far?"},
            {bob, bob_api, "It is surprisingly easy to use!"},
            {alice, alice_api, "Agreed. See you around, Bob!"},
            {bob, bob_api, "See you, Alice! Bye!"}};

    for (const auto& turn : conversation)
    {
        turn.sender.sendMessage(
                nexilis::client::Packet::Room::Communicate::broadcast(turn.api, turn.text));
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    // Wait until both clients have received and displayed every message.
    const auto expected = conversation.size();
    std::size_t alice_printed = 0;
    std::size_t bob_printed = 0;

    const bool delivered = waitFor([&]()
                                   {
                                       printNewMessages(alice_api, room_id, "Alice", alice_printed);
                                       printNewMessages(bob_api, room_id, "Bob", bob_printed);
                                       auto* alice_room = alice_api.getRoom(room_id);
                                       auto* bob_room = bob_api.getRoom(room_id);
                                       return alice_room && bob_room &&
                                              alice_room->getMessages().size() >= expected &&
                                              bob_room->getMessages().size() >= expected; },
                                   std::chrono::seconds(15));

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    printNewMessages(alice_api, room_id, "Alice", alice_printed);
    printNewMessages(bob_api, room_id, "Bob", bob_printed);

    bob.stop();
    std::cout << "Bob disconnected" << std::endl;

    alice.stop();
    std::cout << "Alice disconnected" << std::endl;

    server.stop();
    std::cout << "Server stopped" << std::endl;

    return delivered ? EXIT_SUCCESS : EXIT_FAILURE;
}
