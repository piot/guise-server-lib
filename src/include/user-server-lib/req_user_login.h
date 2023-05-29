/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef RELAY_SERVER_REQ_USER_LOGIN_H
#define RELAY_SERVER_REQ_USER_LOGIN_H

#include <user-server-lib/address.h>

struct UserServer;
struct FldOutStream;

int userReqUserLogin(struct UserServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                     struct FldOutStream* response);

#endif
