/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef USER_SERVER_USERS_H
#define USER_SERVER_USERS_H

#include <stdlib.h>

#include <user-server-lib/address.h>

struct FldInStream;
struct User;

typedef struct Users {
    struct User* users;
    size_t capacity;
    size_t count;
    char prefix[32];
    Clog log;
} Users;

void usersInit(Users* self, Clog log);
void usersReset(Users* self);
void usersDestroy(Users* self);
int usersCreate(Users* sessions, struct User* user, const NetworkAddress* address, struct User** outUser);
int usersReadAndFind(const Users* self, const NetworkAddress* address, struct FldInStream* stream,
                     struct User** outUser);
int usersReadLogin(const Users* self, const char* name, struct User** outUser);

#endif
