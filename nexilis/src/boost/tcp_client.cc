#include <nexilis/boost/tcp_client.hh>
#include <nexilis/log.hh>

#include <iostream>
#include <thread>

namespace nexilis::boost
{

TCPClient::TCPClient(const std::string& serverIP, const std::string& serverPort) :
    m_socket(m_ioService),
    m_resolver(m_ioService),
    m_iterator(m_resolver.resolve({serverIP, serverPort}))
{
}

TCPClient::~TCPClient()
{
    m_socket.close();
}

bool TCPClient::connectToServer()
{
    ::boost::asio::connect(m_socket, m_iterator);
    return m_socket.is_open();
}

bool TCPClient::send(const std::string& data)
{
    ::boost::asio::write(m_socket, ::boost::asio::buffer(data));
    return true;
}

bool TCPClient::receive(std::string& buffer)
{
    ::boost::asio::streambuf receiveBuffer;
    ::boost::system::error_code error;

    ::boost::asio::read(m_socket, receiveBuffer, ::boost::asio::transfer_at_least(1), error);

    if (error == ::boost::asio::error::eof)
    {
        // Server closed the connection.
        Log::error("Server closed the connection");
    }
    else if (error)
    {
        // Handle other errors.
        Log::error("Error reading from server: ", error.message());
    }

    buffer = ::boost::asio::buffer_cast<const char*>(receiveBuffer.data());
    return true;
}

void TCPClient::start()
{
    if (connectToServer())
    {
        Log::info("Connected to server!");

        // Start a separate thread to continuously receive messages.
        std::thread receiveThread(&TCPClient::receiveLoop, this);
        receiveThread.join();
    }
    else
    {
        Log::error("Failed to connect to the server");
    }
}

void TCPClient::receiveLoop()
{
    while (true)
    {
        std::string buffer;

        if (receive(buffer))
        {
            if (!buffer.empty())
            {
                std::cout << "Received from server: " << buffer << std::endl;
                send("clientreply");
            }
            else
            {
                std::cout << "Received empty message from server" << std::endl;
                break;
            }
        }
        else
        {
            std::cout << "Error receiving from server" << std::endl;
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

}
