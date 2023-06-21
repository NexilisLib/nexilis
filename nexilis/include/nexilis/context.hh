#ifndef NEXILIS_CONTEXT_HH
#define NEXILIS_CONTEXT_HH

#include <functional>

namespace nexilis
{

class Context
{

public:

    enum class Messages
    {

    };

public:

    Context(std::function<void()> sendMessage);

    std::function<void()> m_sendMessage;

};

}


#endif
