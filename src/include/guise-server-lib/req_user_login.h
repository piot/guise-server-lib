/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_REQ_GUISE_LOGIN_H
#define GUISE_SERVER_REQ_GUISE_LOGIN_H

#include <guise-server-lib/address.h>

struct GuiseServer;
struct FldOutStream;

int guiseReqUserLogin(struct GuiseServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                     struct FldOutStream* response);

#endif
