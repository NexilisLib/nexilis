#ifndef NEXILIS_WEBSOCKET_WEBSOCKET_HH
#define NEXILIS_WEBSOCKET_WEBSOCKET_HH

#include <nexilis/ports.hh>
#include <nexilis/command.hh>
#include <nexilis/connection_storage.hh>
#include <nexilis/log.hh>

#include <nexilis/websocket/websocket_macros.hh>

#include <boost/asio/ip/tcp.hpp>

#include <cctype>
#include <functional>
#include <algorithm>

/// At some we need global debug.
#define WEBSOCKET_DEBUG

namespace nexilis
{

class Websocket
{
public:
    /// Constructor.
    Websocket()
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

            // Get ip address.
            std::string ip_address = remote_endpoint.address().to_string();

            // Convert the address to lowercase to for case-insensitive comparison.
            std::transform(ip_address.begin(), ip_address.end(), ip_address.begin(), ::tolower);

            // Check if the address is an IPv6-mapped Ipv4 address.
            if (ip_address.compare(0, 7, "::ffff:") == 0)
            {
                // Remove previous prefix.
                ip_address = ip_address.substr(7);
            }

            Connection connection(ip_address);
            auto nexilisMessage = convertToNexilisCommand(msg);

#ifdef WEBSOCKET_DEBUG
            std::string messageStr;
            for (int i = 0; i < nexilisMessage.size(); i++)
            {
                messageStr += nexilisMessage[i];
            }
            Log::info("Received message: " + messageStr);
#endif
            // Add new unknown connection.
            if (!ConnectionStorage::contains(connection))
            {
                ConnectionStorage::add(std::move(connection));
            }

            if (!Command::read(nexilisMessage, connection))
            {
                Log::error("Something went wrong with the reading of the command");
            }
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
        m_websocket.listen(static_cast<short>(Port::Websocket));
        m_websocket.start_accept();

        m_websocket.run();
    }

private:
    /// Get command as vectors of bytes.
    /// \param msg Message gotten from websocketpp.
    /// \return The command of nexilis.
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

    wpp_websocket m_websocket;
};

}

#endif
