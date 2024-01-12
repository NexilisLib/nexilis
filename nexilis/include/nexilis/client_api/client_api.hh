#include <nexilis/client.hh>

namespace nexilis
{

class ClientAPI
{
public:
    ClientAPI(size_t client_id) : m_client_id(client_id)
    {
    }

private:
    size_t m_client_id;
    // Ideally this would be really cool.
    //Client& client;

    // Or even.
    //Client client;
};

}
