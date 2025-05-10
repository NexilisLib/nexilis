#include <nexilisc/client/client_api_c.h>
#include <nexilisc/nexilis_promise.h>
#include <nexilisc/nexilis_waiter.h>

#include <functional>
#include <future>

struct nexilis_WaiterHandle
{
    std::function<void()> callback;
    nexilis_ClientAPI* client_api;
    std::mutex mutex;
    bool waited = false;
    nexilis_WaitType wait_type;
};

namespace
{
std::function<void()> create_wait_callback(nexilis_ClientAPI* client_api,
                                           std::promise<void>& promise,
                                           nexilis_WaitType wait_type)
{
    if (!client_api || !client_api->api)
    {
        return [&promise]()
        {
            promise.set_exception(std::make_exception_ptr(
                    std::runtime_error("Invalid ClientAPI")));
        };
    }

    switch (wait_type)
    {
        case NEXILIS_WAIT_UNTIL_ROOMS_CREATED:
            return [client_api, &promise]()
            {
                client_api->api->waitUntilRoomsCreated(promise);
            };
        default:
            return [&promise]()
            {
                promise.set_exception(std::make_exception_ptr(
                        std::runtime_error("Unknown wait type")));
            };
    }
}
} // namespace

nexilis_WaiterHandle* nexilis_waiter_create(nexilis_ClientAPI* client_api, nexilis_PromiseHandle* promise, nexilis_WaitType wait_type)
{
    if (!client_api || !promise)
        return nullptr;

    try
    {
        auto waiter = new nexilis_WaiterHandle();
        waiter->client_api = client_api;
        waiter->wait_type = wait_type;
        waiter->callback = create_wait_callback(client_api, promise->promise, wait_type);
        return waiter;
    }
    catch (...)
    {
        return nullptr;
    }
}

nexilis_WaiterHandle* nexilis_waiter_create_for_rooms_created(nexilis_ClientAPI* client_api, nexilis_PromiseHandle* promise)
{
    if (client_api && promise)
    {
        return nexilis_waiter_create(client_api, promise, NEXILIS_WAIT_UNTIL_ROOMS_CREATED);
    }
    else
    {
        return nullptr;
    }
}

void nexilis_waiter_wait(nexilis_WaiterHandle* waiter)
{
    if (!waiter)
        return;

    std::lock_guard<std::mutex> lock(waiter->mutex);
    if (waiter->waited)
        return;

    if (waiter->callback)
    {
        try
        {
            waiter->callback();
            waiter->waited = true;
        }
        catch (...)
        {
            // Handle exceptions
        }
    }
}

void nexilis_waiter_destroy(nexilis_WaiterHandle* waiter)
{
    if (waiter)
    {
        std::lock_guard<std::mutex> lock(waiter->mutex);
        delete waiter;
    }
}

bool nexilis_waiter_is_valid(const nexilis_WaiterHandle* waiter)
{
    return waiter != nullptr && waiter->callback != nullptr;
}
