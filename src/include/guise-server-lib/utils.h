/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_LIB_UTILS_H
#define GUISE_SERVER_LIB_UTILS_H

#include <guise-server-lib/transport.h>

struct GuiseServerConnection;

typedef struct GuiseServerResponse {
    GuiseServerSendDatagram sendDatagram;
} GuiseServerResponse;

#endif
