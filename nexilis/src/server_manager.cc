#include "nexilis/af_unix/sock_stream/server.hh"
#include <nexilis/server_manager.hh>

namespace nexilis
{

/// Default 1000, maybe the amount should be a macro somewhere.
size_t ServerManager::m_maxClients = 1000;

}
