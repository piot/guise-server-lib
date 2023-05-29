/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef USER_SERVER_USER_SESSION_H
#define USER_SERVER_USER_SESSION_H

#include <clog/clog.h>
#include <user-serialize/types.h>
#include <user-server-lib/address.h>

struct User;

typedef struct UserSession {
    UserSerializeUserSessionId userSessionId;
    NetworkAddress address;
    const struct User* user;
    Clog log;
    char prefix[32];
} UserSession;

void userSessionInit(UserSession* self, UserSerializeUserSessionId userSessionId, const NetworkAddress* address,
                     const struct User* owner, Clog log);

#endif
