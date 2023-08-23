#include <iostream>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

static const short PORT = 1999;

struct Client
{
    boost::asio::io_service& m_io_service;
    boost::asio::ip::tcp::socket m_socket;

    Client(boost::asio::io_service& svc, std::string const& host, std::string const& port) 
        : m_io_service(svc), m_socket(m_io_service) 
    {
        boost::asio::ip::tcp::resolver resolver(m_io_service);
        boost::asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(boost::asio::ip::tcp::resolver::query(host, port));
        boost::asio::connect(this->m_socket, endpoint);
    };

    void send(std::string const& message) 
    {
        m_socket.send(boost::asio::buffer(message));
    }
};


void client_thread()
{
    boost::asio::io_service svc;
    Client client(svc, "127.0.0.1", std::to_string(PORT));

    client.send("hello world\n");
    client.send("bye world\n");
}

int main() 
{
    boost::thread_group tg;
    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
    tg.create_thread(client_thread);

    tg.join_all();
}
