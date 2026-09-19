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

#include <nexilis/af_inet/tcp_client.hh>
#include <nexilis/log.hh>
#include <nexilis/nexilis_constants.hh>

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>

namespace nexilis::af_inet
{

TCPClient::TCPClient(ClientAPI& api)
    : ClientProtocol(&api)
{
    m_clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (m_clientSocket == -1)
    {
        Log::critical("TCPClient: Error creating socket");
    }

    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(ClientProtocol::getClientAPI()->getInetTCPPortNumber());

    if (inet_pton(AF_INET, ClientProtocol::getClientAPI()->getInetTCPServerAddress().c_str(), &m_serverAddr.sin_addr) <= 0)
    {
        Log::critical("TCPClient: Invalid server address!");
    }
}

TCPClient::~TCPClient()
{
    close(m_clientSocket);

    if (m_listenThread.joinable())
    {
        m_listenThread.join();
    }
}

TCPClient::TCPClient(TCPClient&& other)
    : Protocol(std::move(other)),
      ClientProtocol(std::move(other)),
      m_clientSocket(std::move(other.m_clientSocket)),
      m_serverAddr(std::move(other.m_serverAddr)),
      m_listenThread(std::move(other.m_listenThread))
{
}

TCPClient& TCPClient::operator=(TCPClient&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
        m_clientSocket = std::move(other.m_clientSocket);
        m_serverAddr = std::move(other.m_serverAddr);
        m_listenThread = std::move(other.m_listenThread);
    }
    return *this;
}

void TCPClient::start()
{
    bool connectedToServer = connectToServer();

    if (!connectedToServer)
    {
        Log::error("TCPClient: Couldn't connect to server");
        return;
    }

    m_listenThread = std::thread(&TCPClient::receiveLoop, this);
}

void TCPClient::sendMessage(const std::string& message)
{
    bool sentMessage = send(message.c_str(), message.size());

    if (!sentMessage)
    {
        Log::error("TCPClient: Error sending message");
    }
}

void TCPClient::sendMessage(const nx_data& message)
{
    const char* data = reinterpret_cast<const char*>(message.data());
    bool sentMessage = send(data, message.size());

    if (!sentMessage)
    {
        Log::error("TCPClient: Error sending message");
    }
}

bool TCPClient::connectToServer()
{
    return connect(m_clientSocket, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)) == 0;
}

bool TCPClient::send(const char* data, size_t dataSize)
{
    return write(m_clientSocket, data, dataSize) == static_cast<long>(dataSize);
}

bool TCPClient::receive(char* buffer, size_t bufferSize)
{
    ssize_t bytesRead = read(m_clientSocket, buffer, bufferSize);
    return bytesRead > 0;
}

void TCPClient::receiveLoop()
{
    while (true)
    {
        char buffer[NEXILIS_BUFFER];
        bool receivedData = receive(buffer, sizeof(buffer));

        if (receivedData)
        {
            auto dataVector = Util::convertToByteVector(buffer, sizeof(buffer));
            ClientProtocol::getClientAPI()->readMessage(dataVector);
        }
    }
}

} // namespace nexilis::af_inet
