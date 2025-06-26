#ifndef NEXILISC_CLIENT_SESSION_C_H
#define NEXILISC_CLIENT_SESSION_C_H

#include <stdbool.h>
#include <stdint.h>

#include <nexilisc/client/client_api_c.h>
#include <nexilisc/types/vector3_c.h>

#include <nexilis/client/client_session.hh>

#ifdef __cplusplus
extern "C" {
#endif

struct nexilis_ClientSession
{
    nexilis::client::ClientSession* client;
};

nexilis_ClientSession* nexilis_client_session_create(uint64_t id, nexilis_ClientAPI* client_api);
void nexilis_client_session_destroy(nexilis_ClientSession* session);

uint64_t nexilis_client_session_get_id(nexilis_ClientSession* session);

void nexilis_client_session_set_position_3D(nexilis_ClientSession* client, float x, float y, float z);
nexilis_Vector3f* nexilis_client_session_get_position_3D(nexilis_ClientSession* client);
bool nexilis_client_session_get_position_3D_values(nexilis_ClientSession* client, float* x, float* y, float* z);

nexilis_ClientSession* nexilis_client_session_move(nexilis_ClientSession* other);
void nexilis_client_session_move_assign(nexilis_ClientSession* dest, nexilis_ClientSession* src);

bool nexilis_client_session_equal(const nexilis_ClientSession* lhs, const nexilis_ClientSession* rhs);
bool nexilis_client_session_not_equal(const nexilis_ClientSession* lhs, const nexilis_ClientSession* rhs);

void nexilis_client_session_set_username(nexilis_ClientSession* session, const char* new_username);

#ifdef __cplusplus
}
#endif

#endif
