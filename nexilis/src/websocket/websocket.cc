#include <nexilis/websocket/websocket.hh>
#include <nexilis/command.hh>
#include <nexilis/connection_storage.hh>

namespace nexilis
{

Websocket::Websocket(unsigned port) :
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
		Log::info("Received websocket message: " + messageStr);
#endif
		// Add new unknown connection.
		if (!ConnectionStorage::contains(connection))
		{
			ConnectionStorage::add(std::move(connection));
		}

		// We return false from message that is not understood by nexilis.
		if (!Command::read(nexilisMessage, connection))
		{
			Log::error("Something went wrong with the reading of the command");
		}
	});
}

void Websocket::setOpenHandler(const std::function<void()>& openHandler)
{
	m_websocket.set_open_handler([&openHandler](wpp_connection)
	{
		openHandler();
	});
}

void Websocket::setCloseHandler(const std::function<void()>& closeHandler)
{
	m_websocket.set_close_handler([&closeHandler](wpp_connection)
	{
		closeHandler();
	});
}

void Websocket::start()
{
	m_websocket.set_reuse_addr(true);
	m_websocket.listen(m_port);
	m_websocket.start_accept();

	m_websocket.run();
}

std::vector<unsigned char> Websocket::convertToNexilisCommand(const wpp_message& msg)
{
	std::vector<unsigned char> result;

	const std::string& payload = msg->get_payload();

	for (size_t i = 0; i < payload.size(); i++)
	{
		result.push_back(static_cast<unsigned char>(payload[i]));
	}

	return result;
}

}
