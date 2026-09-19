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

#ifndef NEXILIS_AF_INET_BASE_UDP_SERVER_HH
#define NEXILIS_AF_INET_BASE_UDP_SERVER_HH

#include <nexilis/ports.hh>
#include <nexilis/protocol.hh>

#include <sys/socket.h>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

namespace nexilis::af_inet
{

class BaseUDPServer : public Protocol
{
public:
    struct Message
    {
        const char* address;
        std::string message;
        uint16_t port;
        const struct sockaddr* clientAddr;
        socklen_t clientAddrLen;
    };

    /// Constructor.
    BaseUDPServer(unsigned port = static_cast<unsigned>(Port::UDP));

    /// Move constructor.
    BaseUDPServer(BaseUDPServer&& other);

    /// Move assignment operator.
    BaseUDPServer& operator=(BaseUDPServer&& other);

    /// Deleted copy constructor..
    BaseUDPServer(const BaseUDPServer& other) = delete;

    /// Deleted copy assignment operator.
    BaseUDPServer& operator=(const BaseUDPServer& other) = delete;

    /// Virtual destructor
    virtual ~BaseUDPServer();

    /// Start listening to incoming messages.
    void start() override;

    /// Stop the server.
    void stop() override;

    /// Retrieve message from the queue (if available).
    bool getNextMessage(Message& msg);

protected:
    int m_serverSocket;

    std::unique_ptr<std::atomic<bool>> m_running;
    std::thread m_recvThread;
    std::unique_ptr<std::mutex> m_mtx;
    std::queue<Message> m_messageQueue;
    std::unique_ptr<std::condition_variable> m_condition;

private:
    // Thread function to handle incoming messages.
    void receiverThread();
};

} // namespace nexilis::af_inet

#endif
