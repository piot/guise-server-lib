/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_USERS_H
#define GUISE_SERVER_USERS_H

#include <stdlib.h>

#include <guise-server-lib/address.h>

struct FldInStream;
struct GuiseUser;

typedef struct GuiseUsers {
    struct GuiseUser* guiseUsers;
    size_t capacity;
    size_t count;
    char prefix[32];
    Clog log;
} GuiseUsers;

void guiseUsersInit(GuiseUsers* self, Clog log);
void guiseUsersReset(GuiseUsers* self);
void guiseUsersDestroy(GuiseUsers* self);
int guiseUsersCreate(GuiseUsers* self, GuiseSerializeUserId userId, struct GuiseUser** outUser);
struct GuiseUser* guiseUsersFindFromUserId(const GuiseUsers* self, GuiseSerializeUserId userId);

int guiseUsersFind(const GuiseUsers* self, GuiseSerializeUserId userId,
                   const GuiseSerializePasswordHashWithChallenge password, struct GuiseUser** outUser);
int guiseUsersReadLogin(const GuiseUsers* self, const char* name, struct GuiseUser** outUser);

#endif
