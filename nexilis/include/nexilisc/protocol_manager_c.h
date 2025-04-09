#ifndef NEXILISC_PROTOCOL_MANAGER_C_H
#define NEXILISC_PROTOCOL_MANAGER_C_H

#include <nexilisc/protocol_type_c.h>
#include <nexilisc/protocol_status_c.h>

#include <nexilis/protocol_manager.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_ProtocolManagerC
{
    nexilis::ProtocolManager* manager;
};

typedef struct nexilis_ProtocolDataC nexilis_ProtocolDataC;

// ProtocolManager
nexilis_ProtocolManagerC* nexilis_protocol_manager_create();
void nexilis_protocol_manager_destroy(nexilis_ProtocolManagerC* manager);

// ProtocolData
nexilis_ProtocolDataC* nexilis_protocol_data_create(nexilis_ProtocolTypeC type);
void nexilis_protocol_data_destroy(nexilis_ProtocolDataC* data);
nexilis_ProtocolTypeC nexilis_protocol_data_get_type(const nexilis_ProtocolDataC* data);
nexilis_ProtocolStatusC nexilis_protocol_data_get_status(const nexilis_ProtocolDataC* data);
size_t nexilis_protocol_data_get_id(const nexilis_ProtocolDataC* data);

#ifdef __cplusplus
}
#endif

#endif
