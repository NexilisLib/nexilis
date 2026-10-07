/* Copyright (C) 2026 Valtteri Viirret
   SPDX-License-Identifier: LGPL-3.0-or-later */

#include <chrono>
#include <iostream>
#include <thread>

#include <nexilis/protocol_manager.hh>
#include <nexilis/server/client_storage.hh>
#include <nexilis/server/protocol/nxboost/tcp_server.hh>

int main()
{
    nexilis::ProtocolManager manager;
    nexilis::server::ServerConfig config;
    config.setMode(nexilis::server::AuthenticationMode::password_protected);
    config.setPassphrase("lua-test-password");
    config.setRootPassword("lua-test-root");
    auto server = manager.createProtocol<nexilis::server::nxboost::TCPServer>(config);
    server.start();
    std::cout << "ready" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(60));
    server.stop();
    nexilis::server::ClientStorage::clear();
}
