#include <nexilis/boost/tcp_client.hh>

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
    ::boost::asio::read_until(m_socket, receiveBuffer, '\n');
    buffer = ::boost::asio::buffer_cast<const char*>(receiveBuffer.data());
    return true;
}

void TCPClient::start()
{
    if (connectToServer())
    {
        std::cout << "Connected to server!" << std::endl;

        // Send a message to server.
        const std::string message = "moikamoi\n";
        send(message);

        // Start a separate thread to continuously receive messages.
        std::thread receiveThread(&TCPClient::receiveLoop, this);
        receiveThread.join();
    }
    else
    {
        std::cout << "Failed to connect to the server" << std::endl;
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
                send("clientreply\n");
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