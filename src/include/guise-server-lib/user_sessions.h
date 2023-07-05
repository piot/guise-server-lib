/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_GUISE_SESSIONS_H
#define GUISE_SERVER_GUISE_SESSIONS_H

#include <stdlib.h>
#include <guise-server-lib/address.h>

struct GuiseUserSession;
struct sockaddr_in;
struct FldInStream;
struct GuiseUser;

typedef struct GuiseUserSessions {
    struct GuiseUserSession* guiseUserSessions;
    size_t userSessionCapacity;
    size_t userSessionCount;
    Clog log;
    char prefix[32];
} GuiseUserSessions;

void guiseUserSessionsInit(GuiseUserSessions* self, Clog log);
void guiseUserSessionsDestroy(GuiseUserSessions* self);
void guiseUserSessionsReset(GuiseUserSessions* self);
int guiseUserSessionsCreate(GuiseUserSessions* sessions, struct GuiseUser* user, const NetworkAddress* address,
                       struct GuiseUserSession** outSession);
int guiseUserSessionsReadAndFind(const GuiseUserSessions* self, const NetworkAddress* address, struct FldInStream* stream,
                            const struct GuiseUserSession** outSession);
int guiseUserSessionsFind(const GuiseUserSessions* self, GuiseSerializeUserSessionId uniqueId, const NetworkAddress* addr,
                            const struct GuiseUserSession** outSession);
#endif
