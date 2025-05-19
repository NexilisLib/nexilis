#ifndef NEXILISC_CLIENT_ROOM_H_C
#define NEXILISC_CLIENT_ROOM_H_C

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nexilis_Communication nexilis_Communication;
typedef struct nexilis_Room nexilis_Room;
typedef struct nexilis_ClientSession nexilis_ClientSession;


//nexilis_Room* nexilis_room_create(const char* room_data, nexilis_ClientSession** clients, size_t num_clients);
void nexilis_room_destroy(nexilis_Room* room);
void nexilis_room_add_client(nexilis_Room* room, nexilis_ClientSession* client);
void nexilis_room_remove_client(nexilis_Room* room, uint64_t client_id);
nexilis_ClientSession** nexilis_room_get_clients(const nexilis_Room* room, size_t* num_clients);
void nexilis_room_add_message(nexilis_Room* room, nexilis_Communication* communication);
nexilis_Communication** nexilis_room_get_messages(const nexilis_Room* room, size_t* num_messages);
bool nexilis_room_contains_communication(const nexilis_Room* room, const nexilis_Communication* communication);
bool nexilis_room_contains_communication_by_id(const nexilis_Room* room, uint64_t communication_id);

nexilis_Communication* nexilis_communication_create(const char* payload, nexilis_ClientSession* sender);
void nexilis_communication_destroy(nexilis_Communication* communication);
const char* nexilis_communication_get_payload(const nexilis_Communication* communication);
const nexilis_ClientSession* nexilis_communication_get_client(const nexilis_Communication* communication);
uint64_t nexilis_communication_get_id(const nexilis_Communication* communication);

#ifdef __cplusplus
}
#endif

#endif
