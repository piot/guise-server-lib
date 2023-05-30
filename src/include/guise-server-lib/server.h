/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_SERVER_H
#define GUISE_SERVER_SERVER_H

#include <clog/clog.h>
#include <stdarg.h>
#include <guise-serialize/types.h>
#include <guise-server-lib/address.h>
#include <guise-server-lib/transport.h>
#include <guise-server-lib/user_sessions.h>
#include <guise-server-lib/users.h>

struct ImprintAllocator;
struct GuiseServerResponse;

typedef struct GuiseServer {
    GuiseUsers guiseUsers;
    GuiseUserSessions guiseUserSessions;
    Clog log;
} GuiseServer;

int guiseServerInit(GuiseServer* self, struct ImprintAllocator* memory, Clog log);
void guiseServerDestroy(GuiseServer* self);
void guiseServerReset(GuiseServer* self);
int guiseServerFeed(GuiseServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                   struct GuiseServerResponse* response);

#endif
