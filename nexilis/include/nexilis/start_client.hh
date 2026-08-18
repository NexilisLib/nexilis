#ifndef NEXILIS_START_CLIENT_HH
#define NEXILIS_START_CLIENT_HH

#include <nexilis/room_info.hh>
#include <nexilis/tcp_client.hh>

#include <thread>

namespace nexilis
{

std::thread startClient(client::ClientAPI& client_api, TCPClient& tcp_client, std::vector<RoomInfo>& rooms, std::atomic<bool>& ready, std::mutex& mtx);

}

#endif
