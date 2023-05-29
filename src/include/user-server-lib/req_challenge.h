/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef USER_SERVER_REQ_CHALLENGE_H
#define USER_SERVER_REQ_CHALLENGE_H

#include <user-server-lib/address.h>

struct NetworkAddress;
struct UserServer;
struct FldOutStream;

int userReqChallenge(struct UserServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                     struct FldOutStream* outStream);

#endif
