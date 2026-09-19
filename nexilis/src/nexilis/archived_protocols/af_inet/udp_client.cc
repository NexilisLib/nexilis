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

#include <nexilis/af_inet/udp_client.hh>
#include <nexilis/log.hh>
#include <nexilis/nexilis_constants.hh>

#include <arpa/inet.h>

namespace nexilis::af_inet
{

UDPClient::UDPClient(ClientAPI& api)
    : ClientProtocol(&api)
{
    memset(&m_serverAddr, 0, sizeof(m_serverAddr));
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(api.getInetUDPPortNumber());
    if (inet_pton(AF_INET, api.getInetUDPServerAddress().c_str(), &m_serverAddr.sin_addr) <= 0)
    {
        Log::critical("Invalid server address");
        exit(EXIT_FAILURE);
    }

    m_clientSocket = createSocket();
}

UDPClient::UDPClient(UDPClient&& other)
    : Protocol(std::move(other)),
      ClientProtocol(std::move(other)),
      m_clientSocket(std::move(other.m_clientSocket)),
      m_serverAddr(std::move(other.m_serverAddr)),
      m_receiverThread(std::move(other.m_receiverThread))
{
}

UDPClient& UDPClient::operator=(UDPClient&& other)
{
    if (this != &other)
    {
        Protocol::operator=(std::move(other));
        ClientProtocol::operator=(std::move(other));
        m_clientSocket = std::move(other.m_clientSocket);
        m_serverAddr = std::move(other.m_serverAddr);
        m_receiverThread = std::move(other.m_receiverThread);
    }
    return *this;
}

int UDPClient::createSocket()
{
    int socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFD == -1)
    {
        Log::critical("Failed to create socket.");
        exit(EXIT_FAILURE);
    }
    return socketFD;
}

void UDPClient::sendMessage(const nx_data& message)
{
    /// TODO Remove string conversion.
    const char* data = reinterpret_cast<const char*>(message.data());
    sendData(data, message.size());
}

void UDPClient::sendData(const char* data, size_t dataSize)
{
    auto serverAddr = (const struct sockaddr*)&m_serverAddr;
    if (sendto(m_clientSocket, data, dataSize, 0, serverAddr, sizeof(m_serverAddr)) == -1)
    {
        Log::error("Error sending message");
    }
}

nx_data UDPClient::receiveData(sockaddr* srcAddr, socklen_t* srcAddrLen)
{
    nx_data receivedData(NEXILIS_BUFFER);
    ssize_t bytesRead = recvfrom(m_clientSocket, receivedData.data(), receivedData.size(), 0, srcAddr, srcAddrLen);

    if (bytesRead == -1)
    {
        perror("recvfrom");
        Log::error("UDPClient receiveData");
    }

    receivedData.resize(bytesRead);
    return receivedData;
}

void UDPClient::start()
{
    m_receiverThread = std::thread(&UDPClient::receiveLoop, this);
}

void UDPClient::stop()
{
    // Join the thread when stopping
    if (m_receiverThread.joinable())
    {
        m_receiverThread.join();
    }
}

void UDPClient::receiveLoop()
{
    while (true)
    {
        sockaddr srcAddr;
        socklen_t srcAddrLen;

        auto data = receiveData(&srcAddr, &srcAddrLen);
        ClientProtocol::getClientAPI()->readMessage(data);
    }
}

} // namespace nexilis::af_inet
