/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <user-serialize/serialize.h>
#include <user-server-lib/address.h>
#include <user-server-lib/user.h>
#include <user-server-lib/users.h>

/*
int usersCreate(Users* sessions, User* user, const NetworkAddress* address, User** outSession)
{
    for (size_t i = 0; i < sessions->userSessionCapacity; ++i) {
        User* user = &sessions->users[i];
        if (user->name == 0) {
            user->name = tc_str_dup()
            user->user = user;
            *outSession = user;
            return i;
        }
    }
    *outSession = 0;
    return -1;
}

static int usersFind(const Users* self, uint32_t id, const NetworkAddress* addr, User** outSession)
{
    if (id >= self->capacity) {
        return -2;
    }

    User* foundSession = &self->users[id];
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

 int usersReadAndFind(const Users* self, const NetworkAddress* address, FldInStream* stream,
    User** outSession)
{
    uint32_t userId;
    fldInStreamReadUInt32(stream, &userId);

    int errorCode = usersFind(self, userId, address, outSession);
    if (errorCode < 0) {
        CLOG_C_WARN("couldn't find session %d", sessionId);
        return errorCode;
    }

    CLOG_C_DEBUG("found user session %d", sessionId);

    return 0;
}

*/

int usersReadLogin(const Users* self, const char* nameBuf, User** outUser)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        User* user = &self->users[i];
        if (user->name == 0) {
            user->name = tc_str_dup(nameBuf);
            *outUser = user;
            return i;
        }
    }

    *outUser = 0;

    return 0;
}

void usersInit(Users* self, Clog log)
{
    self->log = log;
    self->capacity = 1024;
    self->users = tc_malloc_type_count(User, self->capacity);
    tc_mem_clear_type_n(self->users, self->capacity);
}

void usersReset(Users* self)
{
    for (size_t i = 0; i < self->capacity; ++i) {
        User* user = &self->users[i];
        if (user->name) {
            free((void*) user->name);
        }
        user->name = 0;
    }
}

void usersDestroy(Users* self)
{
    tc_free(self->users);
}
