/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef USER_SERVER_USER_SESSIONS_H
#define USER_SERVER_USER_SESSIONS_H

#include <stdlib.h>
#include <user-server-lib/address.h>

struct UserSession;
struct sockaddr_in;
struct FldInStream;
struct User;

typedef struct UserSessions {
    struct UserSession* userSessions;
    size_t userSessionCapacity;
    size_t userSessionCount;
    Clog log;
    char prefix[32];
} UserSessions;

void userSessionsInit(UserSessions* self, Clog log);
void userSessionsDestroy(UserSessions* self);
void userSessionsReset(UserSessions* self);
int userSessionsCreate(UserSessions* sessions, struct User* user, const NetworkAddress* address,
                       struct UserSession** outSession);
int userSessionsReadAndFind(const UserSessions* self, const NetworkAddress* address, struct FldInStream* stream,
                            const struct UserSession** outSession);

#endif
