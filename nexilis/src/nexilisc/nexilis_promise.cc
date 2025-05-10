#include <nexilisc/nexilis_promise.h>

nexilis_PromiseHandle* nexilis_promise_create()
{
    return new nexilis_PromiseHandle();
}

void nexilis_promise_destroy(nexilis_PromiseHandle* promise)
{
    if (promise)
    {
        delete promise;
    }
}