/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef USER_SERVER_TRANSPORT_H
#define USER_SERVER_TRANSPORT_H

#include <stddef.h>
#include <stdint.h>

#include <user-server-lib/address.h>

typedef int (*userServerSendFn)(void* self, const NetworkAddress* address, const uint8_t* data, size_t size);
typedef struct UserServerSendDatagram {
    userServerSendFn send;
    void* self;
} UserServerSendDatagram;

#endif
