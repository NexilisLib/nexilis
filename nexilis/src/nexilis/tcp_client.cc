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

#include <nexilis/nx_data.hh>
#include <nexilis/tcp_client.hh>

namespace nexilis
{

TCPClient::TCPClient(client::ClientAPI& client_api)
    : m_tcpClient(client::nxboost::TCPClient(client_api))
{
}

TCPClient::TCPClient(TCPClient&& other) noexcept
    : m_tcpClient(std::move(other.m_tcpClient))
{
}

TCPClient& TCPClient::operator=(TCPClient&& other) noexcept
{
    if (this != &other)
    {
        m_tcpClient = std::move(other.m_tcpClient);
    }
    return *this;
}

void TCPClient::start()
{
    m_tcpClient.start();
}

void TCPClient::stop()
{
    m_tcpClient.stop();
}

void TCPClient::sendMessage(const nx_data& message)
{
    m_tcpClient.sendMessage(message);
}

void TCPClient::sendMessage(const nx_data& message,
                            const std::function<void()>& callback)
{
    m_tcpClient.sendMessage(message, callback);
}

std::future<void> TCPClient::sendMessageAsync(const nx_data& message)
{
    return m_tcpClient.sendMessageAsync(message);
}

nexilis::client::ProtocolStatus TCPClient::getProtocolStatus()
{
    return m_tcpClient.getProtocolStatus();
}

} // namespace nexilis
