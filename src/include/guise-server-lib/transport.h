/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_TRANSPORT_H
#define GUISE_SERVER_TRANSPORT_H

#include <stddef.h>
#include <stdint.h>

#include <guise-server-lib/address.h>

typedef int (*guiseServerSendFn)(void* self, const NetworkAddress* address, const uint8_t* data, size_t size);
typedef struct GuiseServerSendDatagram {
    guiseServerSendFn send;
    void* self;
} GuiseServerSendDatagram;

#endif
