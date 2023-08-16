#ifndef IMPL_WEBSOCKET_SERVER_HH
#define IMPL_WEBSOCKET_SERVER_HH

#include <nexilis/core.hh>
#include <nexilis/websocket/websocketpp.hh>

class Server
{
public:
    Server() :
        m_websocket(),
        m_core("exampleServer")
    {
        m_websocket.setOpenHandler([](nexilis::wpp_connection)
        {
            std::cout << "open handler" << std::endl;
        });

        m_websocket.setCloseHandler([](nexilis::wpp_connection)
        {
            std::cout << "close handler" << std::endl;
        });

        m_websocket.start();
    }

private:
    nexilis::Websocketpp m_websocket;
    nexilis::Core m_core;
};

#endif
