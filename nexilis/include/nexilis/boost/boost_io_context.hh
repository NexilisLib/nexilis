#ifndef NEXILIS_UDP_BOOST_IO_CONTEXT_HH
#define NEXILIS_UDP_BOOST_IO_CONTEXT_HH

#include <boost/asio.hpp>

namespace nexilis
{

namespace boost_io_context
{

boost::asio::io_context& getIOContext();

void start();

void stop();

} // namespace boost_io_context

} // namespace nexilis

#endif
