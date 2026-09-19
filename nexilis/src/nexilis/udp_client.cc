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

#include <nexilis/udp_client.hh>

namespace nexilis
{

UDPClient::UDPClient(client::ClientAPI& api)
    : m_udpClient(api)
{
}

UDPClient::UDPClient(UDPClient&& other) noexcept
    : m_udpClient(std::move(other.m_udpClient))
{
}

UDPClient& UDPClient::operator=(UDPClient&& other) noexcept
{
    if (this != &other)
    {
        m_udpClient = std::move(other.m_udpClient);
    }
    return *this;
}

void UDPClient::start()
{
    m_udpClient.start();
}

void UDPClient::stop()
{
    m_udpClient.stop();
}

void UDPClient::sendMessage(const nx_data& message)
{
    m_udpClient.sendMessage(message);
}

void UDPClient::sendMessage(const nx_data& message, const std::function<void()>& callback)
{
    m_udpClient.sendMessage(message, callback);
}

std::future<void> UDPClient::sendMessageAsync(const nx_data& message)
{
    return m_udpClient.sendMessageAsync(message);
}

} // namespace nexilis
