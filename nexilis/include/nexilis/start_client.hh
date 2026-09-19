/* Copyright (C) 2026 Valtteri Viirret
   This file is part of the Nexilis Project.

   This file is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This file is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this file.  If not, see <https://gnu.org>. */

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
