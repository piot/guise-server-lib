/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_UNIQUE_ID_H
#define GUISE_SERVER_UNIQUE_ID_H

#include <stddef.h>
#include <stdint.h>

typedef uint64_t GuiseUniqueId;

GuiseUniqueId guiseGenerateUniqueIdFromIndex(size_t index);
size_t guiseUniqueIdGetIndex(GuiseUniqueId uniqueIndex);

#endif
