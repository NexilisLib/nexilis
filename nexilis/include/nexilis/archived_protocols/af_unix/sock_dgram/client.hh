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

#ifndef NEXILIS_UNIX_SOCKET_CLIENT_HH
#define NEXILIS_UNIX_SOCKET_CLIENT_HH

#include <nexilis/client/client_api.hh>
#include <nexilis/client/client_protocol.hh>
#include <nexilis/protocol.hh>

#include <sys/un.h>

namespace nexilis::af_unix::sock_dgram
{

class Client : public Protocol,
               public ClientProtocol
{
public:
    /// Constructor.
    Client(ClientAPI& clientApi);

    /// Destructor.
    ~Client();

    /// Send message to the server.
    void sendMessage(const nx_data& message) override;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override;

    /// Protocol::getType() implementation.
    Protocol::Type getType() override
    {
        return Protocol::Type::AF_UNIX_SOCK_DGRAM_CLIENT;
    }

private:
    void createSocket();
    std::string receiveMessage();

private:
    std::string m_serverSocketPath;
    int m_clientSocket;
    struct sockaddr_un m_serverAddr;
};

} // namespace nexilis::af_unix::sock_dgram

#endif
