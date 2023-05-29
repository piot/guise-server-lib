/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef USER_SERVER_LIB_UTILS_H
#define USER_SERVER_LIB_UTILS_H

#include <user-server-lib/transport.h>

struct UserServerConnection;

typedef struct UserServerResponse {
    UserServerSendDatagram sendDatagram;
} UserServerResponse;

#endif
