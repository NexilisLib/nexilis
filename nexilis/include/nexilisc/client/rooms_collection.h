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

#ifndef NEXILIS_CLIENT_ROOM_COLLECTION_H
#define NEXILIS_CLIENT_ROOM_COLLECTION_H

#include <nexilisc/client/room_c.h>

#include <nexilis/client/room.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_RoomsCollection
{
    std::vector<nexilis::client::Room>* rooms;
};

struct nexilis_RoomsCollectionClients
{
    const std::vector<nexilis::client::ClientSession>* clients;
};

size_t nexilis_rooms_collection_rooms_count(const nexilis_RoomsCollection* collection);
nexilis_Room* nexilis_rooms_collection_room_get(nexilis_RoomsCollection* collection, size_t index);
void nexilis_rooms_collection_free(nexilis_RoomsCollection* collection);

#ifdef __cplusplus
}
#endif

#endif
