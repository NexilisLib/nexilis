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

#ifndef NEXILIS_UNIX_SOCKET_SERVER_HH
#define NEXILIS_UNIX_SOCKET_SERVER_HH

#include <nexilis/protocol.hh>
#include <nexilis/server/command/command.hh>
#include <nexilis/server/server_protocol.hh>

namespace nexilis::af_unix::sock_dgram
{

class Server : public Protocol,
               public Command,
               public ServerProtocol
{
public:
    /// Constructor.
    Server(const Authentication& authentication, const std::string& socketPath);

    /// Destructor.
    ~Server();

    /// Move constructor.
    Server(Server&& other);

    /// Move assignment operator.
    Server& operator=(Server&& other);

    /// Deleted copy constructor.
    Server(const Server& other) = delete;

    /// Deleted copy assignment operator.
    Server& operator=(const Server& other) = delete;

    void start() override
    {
        while (true)
        {
            receiveMessage();
        }
    }

    void stop() override
    {
    }

    Type getType() override
    {
        return Type::AF_UNIX_SOCK_DGRAM_SERVER;
    }

private:
    void createSocket();
    void bindSocket();
    void receiveMessage();
    static void signalHandler(int signum);

private:
    int m_serverSocket;
    nx_data m_buffer;
};

} // namespace nexilis::af_unix::sock_dgram

#endif
