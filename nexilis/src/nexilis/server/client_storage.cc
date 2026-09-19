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

#include <nexilis/server/client_storage.hh>

#include <nexilis/logger/log.hh>

namespace nexilis::server
{

std::vector<std::unique_ptr<User>> ClientStorage::m_clients = {};
std::mutex ClientStorage::m_mutex;

void ClientStorage::add(std::unique_ptr<User> client)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_clients.emplace_back(std::move(client));
    Log::info("New client, total amount = ", m_clients.size());
}

bool ClientStorage::contains(uint64_t id)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return std::find_if(m_clients.begin(), m_clients.end(),
                        [id](const std::unique_ptr<User>& client)
                        {
                            return client->getId() == id;
                        }) != m_clients.end();
}

std::vector<std::unique_ptr<User>>& ClientStorage::getAllClients()
{
    return m_clients;
}

User* ClientStorage::getClientById(uint64_t id)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = std::find_if(m_clients.begin(), m_clients.end(),
                           [id](const std::unique_ptr<User>& client)
                           {
                               return client->getId() == id;
                           });

    if (it != m_clients.end())
    {
        return it->get();
    }
    else
    {
        return nullptr;
    }
}

std::vector<User*> ClientStorage::getClientsByIpAddress(const std::string& ip_address)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    std::vector<User*> result;

    for (auto& client : m_clients)
    {
        if (client->getIPAddress() == ip_address)
        {
            result.push_back(client.get());
        }
    }

    return result;
}

void ClientStorage::clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_clients.clear();
}

bool ClientStorage::remove(uint64_t id)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = std::find_if(m_clients.begin(), m_clients.end(),
                           [id](const std::unique_ptr<User>& client)
                           {
                               return client->getId() == id;
                           });

    if (it != m_clients.end())
    {
        m_clients.erase(it);
        Log::info("Removed client from storage, total amount = ", m_clients.size());
        return true;
    }
    return false;
}

} // namespace nexilis::server
