#ifndef IMPL_WEBSOCKET_SERVER_HH
#define IMPL_WEBSOCKET_SERVER_HH

#include <nexilis/websocket/websocketpp.hh>

class Server
{
public:
    Server() :
        m_websocket(8000)
    {
        m_websocket.setOpenHandler([](nexilis::connection)
        {
            std::cout << "open handler" << std::endl;
        });

        m_websocket.setCloseHandler([](nexilis::connection)
        {
            std::cout << "close handler" << std::endl;
        });

        m_websocket.start();
    }

private:
    nexilis::Websocketpp m_websocket;
};

#endif
