#ifndef NEXILIS_WEBSOCKET_WEBSOCKETPP_HH
#define NEXILIS_WEBSOCKET_WEBSOCKETPP_HH

#include "websocket_macros.hh"
#include "../command.hh"
#include "../connection_storage.hh"

#include <boost/asio/ip/tcp.hpp>

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

        m_websocket.set_message_handler([this](wpp_connection cnn, wpp_message msg)
        {
            auto con = m_websocket.get_con_from_hdl(cnn);
            auto& socket = con->get_raw_socket();
            auto& tcp_socket = dynamic_cast<boost::asio::ip::tcp::socket&>(socket);
            auto remote_endpoint = tcp_socket.remote_endpoint();

            std::string ip_address = remote_endpoint.address().to_string();
            unsigned short port_number = remote_endpoint.port();

            for (auto& connection : connections)
            {
                // Message from previously known client.
                if (connection.getIPAddress() == ip_address && connection.getPortNumber() == port_number)
                {
                    Command::read(convertToNexilisCommand(msg), connection);
                    return;
                }
            }

            // This is the very first message from the client, we add the client to connections.
            connections.emplace_back(m_websocket, cnn, ip_address, port_number);
            Command::read(convertToNexilisCommand(msg), connections.back());
        });

   }

    void setOpenHandler(const std::function<void(wpp_connection)>& openHandler)
    {
        m_websocket.set_open_handler(openHandler);
    }

    void setCloseHandler(const std::function<void(wpp_connection)>& closeHandler)
    {
        m_websocket.set_close_handler(closeHandler);
    }

    // Start the Websocket server.
    void start()
    {
        m_websocket.set_reuse_addr(true);
        m_websocket.listen(m_port);
        m_websocket.start_accept();

        m_websocket.run();
    }

private:

    std::vector<unsigned char> convertToNexilisCommand(const wpp_message& msg)
    {
        std::vector<unsigned char> result;

        const std::string& payload = msg->get_payload();

        for (size_t i = 0; i < payload.size(); i++)
        {
            result.push_back(static_cast<unsigned char>(payload[i]));
        }

        return result;
    }

    short m_port;

    wpp_websocket m_websocket;
};

}

#endif
