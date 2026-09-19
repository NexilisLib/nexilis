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

#ifndef NEXILIS_CLIENT_COMMAND_GET_ROOM_ID_HH
#define NEXILIS_CLIENT_COMMAND_GET_ROOM_ID_HH

#include <nexilis/client/base_api_command.hh>

#include <mutex>

namespace nexilis::client
{

class GetRoomIdCommand : public BaseAPICommand
{
public:
    explicit GetRoomIdCommand(uint64_t room_id)
        : m_room_id(room_id)
    {
    }

    ReadResult execute(ClientAPI& api, ClientAPI::ClientAPIData& data) override
    {
        auto& mtx = data.getRoomsMutex();
        std::lock_guard<std::mutex> lock(*mtx);

        api.setRoomId(m_room_id);
        return ReadResult::success;
    }

private:
    uint64_t m_room_id;
};

} // namespace nexilis::client

#endif
