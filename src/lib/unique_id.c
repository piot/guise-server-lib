/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <guise-server-lib/unique_id.h>
#include <secure-random/secure_random.h>
#include <stdlib.h>

GuiseUniqueId guiseGenerateUniqueIdFromIndex(size_t index)
{
    uint32_t upperPart = secureRandomUInt64() % 0xffffffff;

    GuiseUniqueId uniqueIndex = ((uint64_t) upperPart << 32) | index;

    return uniqueIndex;
}

size_t guiseUniqueIdGetIndex(GuiseUniqueId uniqueIndex)
{
    return uniqueIndex & 0xffffffff;
}
