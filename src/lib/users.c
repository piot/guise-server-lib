/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "cipher.h"
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <guise-serialize/serialize.h>
#include <guise-server-lib/address.h>
#include <guise-server-lib/user.h>
#include <guise-server-lib/users.h>

int guiseUsersCreate(GuiseUsers* self, GuiseSerializeUserId userId, GuiseUser** outUser)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        GuiseUser* user = &self->guiseUsers[i];
        if (user->id == 0) {
            user->id = userId;
            *outUser = user;
            return i;
        }
    }
    *outUser = 0;
    return -1;
}

/*


static int guiseUsersFind(const GuiseUsers* self, uint32_t id, const NetworkAddress* addr, GuiseUser** outSession)
{
    if (id >= self->capacity) {
        return -2;
    }

    GuiseUser* foundSession = &self->guiseUsers[id];
    if (!nimbleAddressEqual(addr, &foundSession->address)) {
        char addrTemp[64];
        CLOG_C_SOFT_ERROR("wrong address %s vs %s", nimbleAddressToString(addr, addrTemp, 64),
            nimbleAddressToString(&foundSession->address, addrTemp, 64));
        *outSession = 0;
        return -3;
    }

    *outSession = foundSession;

    return 0;
}

 int guiseUsersReadAndFind(const GuiseUsers* self, const NetworkAddress* address, FldInStream* stream,
    GuiseUser** outSession)
{
    uint32_t userId;
    fldInStreamReadUInt32(stream, &userId);

    int errorCode = guiseUsersFind(self, userId, address, outSession);
    if (errorCode < 0) {
        CLOG_C_WARN("couldn't find session %d", sessionId);
        return errorCode;
    }

    CLOG_C_DEBUG("found user session %d", sessionId);

    return 0;
}

*/

GuiseUser* guiseUsersFindFromUserId(const GuiseUsers* self, const GuiseSerializeUserId userId)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        GuiseUser* user = &self->guiseUsers[i];
        if (user->id == userId) {
            return user;
        }
    }
    return 0;
}

int guiseUsersFind(const GuiseUsers* self, const GuiseSerializeUserId userId,
                   const GuiseSerializePasswordHashWithChallenge passwordHashWithChallenge, GuiseUser** outUser)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        GuiseUser* user = &self->guiseUsers[i];
        if (user->id == userId) {
            uint64_t correctHash = extremelyUnsecureCipher(user->randomChallenge, user->passwordHash);
            if (correctHash == passwordHashWithChallenge) {
                *outUser = user;
                return i;
            } else {
                CLOG_C_NOTICE(&self->log, "hash mismatch, can not be logged in correct:%016llx provided:%016llx", correctHash, passwordHashWithChallenge)
                return -4;
            }
        }
    }

    CLOG_C_NOTICE(&self->log, "unknown userId %016llx, can not be logged in", userId)
    *outUser = 0;

    return -1;
}

void guiseUsersInit(GuiseUsers* self, Clog log)
{
    self->log = log;
    self->capacity = 1024;
    self->guiseUsers = tc_malloc_type_count(GuiseUser, self->capacity);
    tc_mem_clear_type_n(self->guiseUsers, self->capacity);
}

void guiseUsersReset(GuiseUsers* self)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        GuiseUser* user = &self->guiseUsers[i];
        user->id = 0;
    }
}

void guiseUsersDestroy(GuiseUsers* self)
{
    tc_free(self->guiseUsers);
}
