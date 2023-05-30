/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <guise-server-lib/unique_id.h>

GuiseUniqueId guiseGenerateUniqueIdFromIndex(size_t index)
{
    uint32_t upperPart = rand() % 0xffffffff;

    GuiseUniqueId uniqueIndex = ((uint64_t) upperPart << 32) | index;

    return uniqueIndex;
}

size_t guiseUniqueIdGetIndex(GuiseUniqueId uniqueIndex)
{
    return uniqueIndex & 0xffffffff;
}
