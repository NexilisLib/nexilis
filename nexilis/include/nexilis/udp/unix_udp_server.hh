#ifndef NEXILIS_UNIX_UDP_SERVER_HH
#define NEXILIS_UNIX_UDP_SERVER_HH

#include "../ports.hh"

#include <arpa/inet.h>
#include <sys/un.h>
#include <unistd.h>

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <csignal>

namespace nexilis
{

class UnixUDPServer
{
public:
    /// Constructor.
    UnixUDPServer();

    /// Destructor.
    ~UnixUDPServer();

    // Receive messages from /tmp/nexilis.
    void receiveMessage();

private:
    int m_serverSocket;
    int m_bufferSize;
    char* m_buffer;

    void createSocket();

    void bindSocket();

    static void signalHandler(int signum);

};

}

#endif
