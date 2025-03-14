#include <nexilis/protocol_manager.hh>
#include <nexilisc/protocol_manager_c.h>

struct nexilis_ProtocolDataC
{
    nexilis::ProtocolManager::ProtocolData* data;
};

nexilis_ProtocolManagerC* nexilis_ProtocolManager_create()
{
    auto protocol_manager = new nexilis_ProtocolManagerC();
    protocol_manager->manager = new nexilis::ProtocolManager();
    return protocol_manager;
}

void nexilis_ProtocolManager_destroy(nexilis_ProtocolManagerC* manager)
{
    if (manager)
    {
        if (manager->manager)
        {
            delete manager->manager;
            manager->manager = nullptr;
        }
        delete manager;
    }
}

nexilis_ProtocolDataC* nexilis_ProtocolData_create(nexilis_ProtocolTypeC type)
{
    auto data = new nexilis_ProtocolDataC();

    auto protocol_data = nexilis::ProtocolManager::ProtocolData(static_cast<nexilis::Protocol::Type>(type));
    data->data = &protocol_data;

    return data;
}

void nexilis_ProtocolData_destroy(nexilis_ProtocolDataC* data)
{
    if (data)
    {
        if (data->data)
        {
            delete data->data;
            data->data = nullptr;
        }
        delete data;
    }
}

nexilis_ProtocolTypeC nexilis_ProtocolData_get_type(const nexilis_ProtocolDataC* data)
{
    if (data && data->data)
    {
        return static_cast<nexilis_ProtocolTypeC>(data->data->getType());
    }
    return PROTOCOL_TYPE_UNKNOWN;
}

nexilis_ProtocolStatusC nexilis_ProtocolData_get_status(const nexilis_ProtocolDataC* data)
{
    if (data && data->data)
    {
        return static_cast<nexilis_ProtocolStatusC>(data->data->getStatus());
    }
    return PROTOCOL_STATUS_UNDEFINED;
}

size_t nexilis_ProtocolData_get_id(const nexilis_ProtocolDataC* data)
{
    if (data && data->data)
    {
        return data->data->getId();
    }
    return 0;
}
