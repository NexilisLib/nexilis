#include <nexilis/boost/boost_io_context.hh>

namespace nexilis
{

namespace boost_io_context
{

static boost::asio::io_context io_context;

bool isRunning = false;

boost::asio::io_context& getIOContext()
{
    return io_context;
}

void start()
{
    if (!isRunning)
    {
        io_context.reset();
        isRunning = true;
    }
}

void stop()
{
    if (isRunning)
    {
        io_context.stop();
        isRunning = false;
    }
}

}

}
