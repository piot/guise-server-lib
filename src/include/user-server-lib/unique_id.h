/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef USER_SERVER_UNIQUE_ID_H
#define USER_SERVER_UNIQUE_ID_H

#include <stddef.h>
#include <stdint.h>

typedef uint64_t UserUniqueId;

UserUniqueId userGenerateUniqueIdFromIndex(size_t index);
size_t userUniqueIdGetIndex(UserUniqueId uniqueIndex);

#endif
