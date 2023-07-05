/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_USER_H
#define GUISE_SERVER_USER_H

typedef struct GuiseUser {
    GuiseSerializeUserName name;
    GuiseSerializeUserId id;
    GuiseSerializeRole roles;
    uint64_t passwordHash;
    uint64_t randomChallenge;
} GuiseUser;

#endif
