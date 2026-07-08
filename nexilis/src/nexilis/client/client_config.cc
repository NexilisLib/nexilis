#include <nexilis/client/client_config.hh>

namespace nexilis::client
{

// ClientConfig doesn't need custom copy/move constructors anymore;
// AuthConfig handles the shared fields and the protocol-specific
// members have trivial copy/move semantics.

} // namespace nexilis::client
