#ifndef NEXILISC_PROMISE_C_H
#define NEXILISC_PROMISE_C_H

#include <future>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct nexilis_PromiseHandle
    {
        std::promise<void> promise;
    };

    nexilis_PromiseHandle* nexilis_promise_create();
    void nexilis_promise_destroy(nexilis_PromiseHandle* promise);

#ifdef __cplusplus
}
#endif

#endif // NEXILISC_PROMISE_C_H
