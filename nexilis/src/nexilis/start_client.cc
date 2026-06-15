#include <nexilis/start_client.hh>

#include <algorithm>
#include <iterator>

#include <nexilis/client/packet.hh>

namespace nexilis
{

std::thread startClient(TCPClient& tcp_client, std::vector<RoomInfo>& rooms, std::atomic<bool>& ready, std::mutex& mtx)
{
    // clang-format off
    return std::thread ([&tcp_client, &rooms, &ready, &mtx]()
    {
        tcp_client.start();

        tcp_client.sendMessage(client::Packet::Get::General::clientId());
        while (!tcp_client.getClientAPI().isInitialized())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        tcp_client.sendMessage(client::Packet::Get::Info::rooms());

        std::promise<void> roomsPromise;
        auto roomsFuture = roomsPromise.get_future();
        auto waitFn = tcp_client.getClientAPI().waitUntilRoomsCreated(roomsPromise);
        waitFn();
        roomsFuture.wait();

        {
            std::lock_guard<std::mutex> lock(mtx);
            const auto& active_rooms = tcp_client.getClientAPI().getActiveRooms();
            rooms.reserve(rooms.size() + active_rooms.size());
            std::transform(active_rooms.begin(), active_rooms.end(), std::back_inserter(rooms),
                           [](const auto& room)
                           { return RoomInfo{room.getId(), room.getName()}; });
        }
        ready = true;
    });
    // clang-format on
}

} // namespace nexilis
