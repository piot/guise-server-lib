/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef USER_SERVER_SERVER_H
#define USER_SERVER_SERVER_H

#include <clog/clog.h>
#include <stdarg.h>
#include <user-serialize/types.h>
#include <user-server-lib/address.h>
#include <user-server-lib/transport.h>
#include <user-server-lib/user_sessions.h>
#include <user-server-lib/users.h>

struct ImprintAllocator;
struct UserServerResponse;

typedef struct UserServer {
    Users users;
    UserSessions userSessions;
    Clog log;
    UserSerializeServerChallenge secretChallengeKey;
} UserServer;

int userServerInit(UserServer* self, struct ImprintAllocator* memory, Clog log);
void userServerDestroy(UserServer* self);
void userServerReset(UserServer* self);
int userServerFeed(UserServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                   struct UserServerResponse* response);

#endif
