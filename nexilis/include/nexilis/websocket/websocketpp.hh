#ifndef NEXILIS_WEBSOCKET_WEBSOCKETPP
#define NEXILIS_WEBSOCKET_WEBSOCKETPP

#include "websocket_macros.hh"

#include <functional>

namespace nexilis
{

class Websocketpp
{
public:
    /// Constructor.
    /// \param port Port for the websocket connection.
    Websocketpp(short port) :
        m_port(port)
    {
        try
        {
            // Initialize websocketpp.
            m_websocket.set_access_channels(websocketpp::log::alevel::all);
            m_websocket.clear_access_channels(websocketpp::log::alevel::frame_payload);
            m_websocket.init_asio();
        }
        catch (const std::exception& e)
        {
            std::cout << "Couldn't start connection because: " << e.what() << std::endl;
        }
    }

    void setMessageHandler(const std::function<void(connection, message)>& messageHandler)
    {
        m_websocket.set_message_handler(messageHandler);
    }

    void setOpenHandler(const std::function<void(connection)>& openHandler)
    {
        m_websocket.set_open_handler(openHandler);
    }

    void setCloseHandler(const std::function<void(connection)>& closeHandler)
    {
        m_websocket.set_close_handler(closeHandler);
    }

    void start()
    {
        m_websocket.set_reuse_addr(true);
        m_websocket.listen(m_port);
        m_websocket.start_accept();

        m_websocket.run();
    }

private:
    short m_port;

    websocket m_websocket;
};

}

#endif
