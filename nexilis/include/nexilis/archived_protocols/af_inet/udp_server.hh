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

#ifndef NEXILIS_AF_INET_UDP_SERVER_HH
#define NEXILIS_AF_INET_UDP_SERVER_HH

#include <nexilis/af_inet/base_udp_server.hh>
#include <nexilis/authentication.hh>
#include <nexilis/command.hh>
#include <nexilis/message_handler.hh>
#include <nexilis/server/server_protocol.hh>

namespace nexilis::af_inet
{

class UDPServer : public BaseUDPServer, public Command, public ServerProtocol
{
public:
    /// Constructor.
    /// \param port The port we are assigning the udp server.
    /// This has been initialized the value of Port::UDP.
    UDPServer(const Authentication& authentication, unsigned port = static_cast<unsigned>(Port::UDP));

    /// Destructor.
    ~UDPServer();

    /// Move constructor.
    UDPServer(UDPServer&& other);

    /// Move assignment operator.
    UDPServer& operator=(UDPServer&& other);

    /// Deleted copy constructor.
    UDPServer(const UDPServer& other) = delete;

    /// Deleted copy assignment operator.
    UDPServer& operator=(const UDPServer& other) = delete;

    /// Protocol::start() implementation.
    void start() override;

    /// Protocol::stop() implementation.
    void stop() override
    {
        BaseUDPServer::stop();
    }

    // Get message from server.
    // \return Message from the BaseUdpServer.
    BaseUDPServer::Message getNextMessage()
    {
        BaseUDPServer::Message msg;
        BaseUDPServer::getNextMessage(msg);
        return msg;
    }

    /// Protocol::getType() implementation.
    Type getType() override
    {
        return Type::AF_INET_UDP_SERVER;
    }

    void sendDataToClient(const nx_data& data, const sockaddr* clientAddr, socklen_t clientAddrLen);

private:
    std::thread m_receiveThread;
};

} // namespace nexilis::af_inet

#endif
