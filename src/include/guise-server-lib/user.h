/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_GUISE_H
#define GUISE_SERVER_GUISE_H

typedef struct GuiseUser {
    const char* name;
    const GuiseSerializeUserId id;
} GuiseUser;

#endif
