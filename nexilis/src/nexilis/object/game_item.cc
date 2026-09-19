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

#include <nexilis/object/game_item.hh>

namespace nexilis
{

GameItem::GameItem(uint64_t id, const std::string& type, const Vector3f& position,
                   const Vector3f& dimensions, const std::string& status,
                   const std::string& filepath)
    : m_id(id),
      m_type(type),
      m_position(position),
      m_dimensions(dimensions),
      m_status(status),
      m_filepath(filepath),
      m_mutex(std::make_unique<std::mutex>())
{
}

GameItem::GameItem(GameItem&& other) noexcept
    : m_id(std::move(other.m_id)),
      m_type(std::move(other.m_type)),
      m_position(std::move(other.m_position)),
      m_dimensions(std::move(other.m_dimensions)),
      m_status(std::move(other.m_status)),
      m_filepath(std::move(other.m_filepath)),
      m_mutex(std::move(other.m_mutex))
{
}

GameItem& GameItem::operator=(GameItem&& other) noexcept
{
    if (this != &other)
    {
        m_id = std::move(other.m_id);
        m_type = std::move(other.m_type);
        m_position = std::move(other.m_position);
        m_dimensions = std::move(other.m_dimensions);
        m_status = std::move(other.m_status);
        m_filepath = std::move(other.m_filepath);
        m_mutex = std::move(other.m_mutex);
    }
    return *this;
}

uint64_t GameItem::getId() const
{
    if (m_mutex)
    {
        std::lock_guard lock(*m_mutex);
        return m_id;
    }
    return m_id;
}

std::string GameItem::getType() const
{
    if (m_mutex)
    {
        std::lock_guard lock(*m_mutex);
        return m_type;
    }
    return m_type;
}

Vector3f GameItem::getPosition() const
{
    if (m_mutex)
    {
        std::lock_guard lock(*m_mutex);
        return m_position;
    }
    return m_position;
}

Vector3f GameItem::getDimensions() const
{
    if (m_mutex)
    {
        std::lock_guard lock(*m_mutex);
        return m_dimensions;
    }
    return m_dimensions;
}

std::string GameItem::getStatus() const
{
    if (m_mutex)
    {
        std::lock_guard lock(*m_mutex);
        return m_status;
    }
    return m_status;
}

std::string GameItem::getFilepath() const
{
    if (m_mutex)
    {
        std::lock_guard lock(*m_mutex);
        return m_filepath;
    }
    return m_filepath;
}

void GameItem::setPosition(const Vector3f& pos)
{
    if (!m_mutex)
    {
        m_mutex = std::make_unique<std::mutex>();
    }
    std::lock_guard lock(*m_mutex);
    m_position = pos;
}

void GameItem::setDimensions(const Vector3f& dim)
{
    if (!m_mutex)
    {
        m_mutex = std::make_unique<std::mutex>();
    }
    std::lock_guard lock(*m_mutex);
    m_dimensions = dim;
}

void GameItem::setStatus(const std::string& status)
{
    if (!m_mutex)
    {
        m_mutex = std::make_unique<std::mutex>();
    }
    std::lock_guard lock(*m_mutex);
    m_status = status;
}

} // namespace nexilis
