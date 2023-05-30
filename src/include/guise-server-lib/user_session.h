/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_GUISE_SESSION_H
#define GUISE_SERVER_GUISE_SESSION_H

#include <clog/clog.h>
#include <guise-serialize/types.h>
#include <guise-server-lib/address.h>

struct GuiseUser;

typedef struct GuiseUserSession {
    GuiseSerializeUserSessionId userSessionId;
    NetworkAddress address;
    const struct GuiseUser* user;
    Clog log;
    char prefix[32];
} GuiseUserSession;

void guiseUserSessionInit(GuiseUserSession* self, GuiseSerializeUserSessionId userSessionId, const NetworkAddress* address,
                     const struct GuiseUser* owner, Clog log);

#endif
