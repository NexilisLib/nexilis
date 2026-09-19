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

#include <nexilis/af_inet/udp_server.hh>
#include <nexilis/client_storage.hh>
#include <nexilis/command.hh>
#include <nexilis/message_handler.hh>

namespace nexilis::af_inet
{

UDPServer::UDPServer(const Authentication& authentication, unsigned port)
    : BaseUDPServer(port),
      Command(authentication)
{
}

UDPServer::UDPServer(UDPServer&& other)
    : BaseUDPServer(std::move(other)),
      Command(std::move(other)),
      m_receiveThread(std::move(other.m_receiveThread))
{
}

UDPServer& UDPServer::operator=(UDPServer&& other)
{
    if (this != &other)
    {
        BaseUDPServer::operator=(std::move(other));
        Command::operator=(std::move(other));
        m_receiveThread = std::move(other.m_receiveThread);
    }
    return *this;
}

UDPServer::~UDPServer()
{
    if (m_receiveThread.joinable())
    {
        m_receiveThread.join();
    }
}

void UDPServer::start()
{
    m_receiveThread = std::thread([this]()
                                  {
        BaseUDPServer::start();

        while (true)
        {
            BaseUDPServer::Message msg;

            // TODO fix
            nx_data data;

            if (BaseUDPServer::getNextMessage(msg))
            {
                auto message = getMessageHandler().readMessage(msg.address, data, msg.port, &Command::getAuthentication());

                auto sendMsg = [this, &msg](const nx_data& data)
                {
                    sendDataToClient(data, msg.clientAddr, msg.clientAddrLen);
                };

                (void)sendMsg;

                if (message.getClient())
                {
                    if (Command::read(message.getData(), *message.getClient(), *this, message.getMessageId()) != Command::Result::success)
                    {
                        Log::error("UDP server message reading error, message: ", msg.message);
                    }
                }
                else
                {
                    Log::info("Message from unauthorized client!");
                }
            }
        } });
}

void UDPServer::sendDataToClient(const nx_data& data, const sockaddr* clientAddr, socklen_t clientAddrLen)
{
    sendto(BaseUDPServer::m_serverSocket, data.data(), data.size(), 0, clientAddr, clientAddrLen);
}

} // namespace nexilis::af_inet
