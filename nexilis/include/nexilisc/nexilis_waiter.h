#ifndef NEXILISC_NEXILIS_WAITER_H
#define NEXILISC_NEXILIS_WAITER_H

#include <nexilisc/nexilis_promise.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct nexilis_ClientAPI nexilis_ClientAPI;
    typedef struct nexilis_PromiseHandle nexilis_PromiseHandle;
    typedef struct nexilis_WaiterHandle nexilis_WaiterHandle;

    typedef enum
    {
        NEXILIS_WAIT_UNTIL_ROOMS_CREATED
    } nexilis_WaitType;

    nexilis_WaiterHandle* nexilis_waiter_create(nexilis_ClientAPI* client_api, nexilis_PromiseHandle* promise, nexilis_WaitType wait_type);
    nexilis_WaiterHandle* nexilis_waiter_create_for_rooms_created(nexilis_ClientAPI* client_api, nexilis_PromiseHandle* promise);

    void nexilis_waiter_wait(nexilis_WaiterHandle* waiter);
    void nexilis_waiter_destroy(nexilis_WaiterHandle* waiter);
    bool nexilis_waiter_is_valid(const nexilis_WaiterHandle* waiter);

#ifdef __cplusplus
}
#endif

#endif // NEXILISC_NEXILIS_WAITER_H
