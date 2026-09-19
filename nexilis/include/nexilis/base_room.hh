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

#ifndef NEXILIS_BASE_ROOM_HH
#define NEXILIS_BASE_ROOM_HH

#include <nexilis/object/game_item.hh>
#include <nexilis/object/object_2d.hh>
#include <nexilis/object/object_3d.hh>
#include <nexilis/room_data.hh>

namespace nexilis
{

class BaseRoom
{
public:
    /// Constructor.
    explicit BaseRoom(const RoomData& roomData);

    /// Move constructor.
    BaseRoom(BaseRoom&& other) noexcept;

    /// Move assignment operator.
    BaseRoom& operator=(BaseRoom&& other) noexcept;

    /// Copy constructor.
    BaseRoom(const BaseRoom& other);

    /// Get given name for the room.
    std::string getName() const
    {
        return m_roomData.getName();
    }

    /// Get the maximum amount of players in a room.
    uint32_t getMaxSize() const
    {
        return m_roomData.getMaxSize();
    }

    /// Get the identifier of the room.
    uint64_t getId() const
    {
        return m_roomData.getId();
    }

    /// Get the identifier of the creator that created the room.
    uint64_t getCreatorId() const
    {
        return m_roomData.getCreatorId();
    }

    /// Get the 2D/3D context for the room.
    RoomData::Context getContext() const
    {
        return m_roomData.getContext();
    }

    /// Add an Object2D to the room.
    void addObject(Object2D&& object);

    /// Add an Object3D to the room.
    void addObject(Object3D&& object);

    // Get all 2D objects.
    const std::vector<Object2D>& getObjects2D() const;

    // Get all 3D objects.
    const std::vector<Object3D>& getObjects3D() const;

    Object2D* getObject2DById(uint64_t id);

    void deleteObject2D(uint64_t id);

    Object3D* getObject3DById(uint64_t id);

    void deleteObject3D(uint64_t id);

    void addGameItem(GameItem&& item);
    const std::vector<GameItem>& getGameItems() const;
    GameItem* getGameItemById(uint64_t id);
    void deleteGameItem(uint64_t id);
    void updateGameItemStatus(uint64_t id, const std::string& status);

private:
    RoomData m_roomData;
    std::vector<Object2D> m_objects2D;
    std::vector<Object3D> m_objects3D;
    std::vector<GameItem> m_gameItems;
};

} // namespace nexilis

#endif
