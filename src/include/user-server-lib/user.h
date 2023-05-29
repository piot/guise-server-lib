/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef USER_SERVER_USER_H
#define USER_SERVER_USER_H

typedef struct User {
    const char* name;
    const UserSerializeUserId id;
} User;

#endif
