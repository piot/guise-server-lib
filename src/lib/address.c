/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <guise-server-lib/address.h>
#include <stdio.h>
#include <tiny-libc/tiny_libc.h>

#ifdef TORNADO_OS_WINDOWS
#include <Ws2tcpip.h>
#endif

int networkAddressEqual(const NetworkAddress* a, const NetworkAddress* b)
{
    return (a->sin_addr.s_addr == b->sin_addr.s_addr && a->sin_port == b->sin_port);
}

const char* networkAddressToString(const NetworkAddress* self, char* temp, size_t maxCount)
{
    const char* converted = inet_ntop(AF_INET, &self->sin_addr, temp, (socklen_t) maxCount - 6);

    size_t len = tc_strlen(converted);

    sprintf(&temp[len], ":%d", self->sin_port);

    return temp;
}

void networkAddressToSerializeAddress(GuiseSerializeAddress* target, const NetworkAddress source)
{
    target->type = GuiseSerializeAddressTypeV4;
    target->address.ipv4[0] = source.sin_addr.s_addr >> 24;
    target->address.ipv4[1] = (source.sin_addr.s_addr >> 16) & 0xff;
    target->address.ipv4[2] = (source.sin_addr.s_addr >> 8) & 0xff;
    target->address.ipv4[3] = (source.sin_addr.s_addr) & 0xff;
    target->port = source.sin_port;
}

void networkAddressFromSerializeAddress(NetworkAddress* target, const GuiseSerializeAddress* source)
{
    // target->type = GuiseSerializeAddressTypeV4;
    target->sin_addr.s_addr = (in_addr_t) ((source->address.ipv4[0] << 24) | (source->address.ipv4[1] << 16) |
                                           (source->address.ipv4[2] << 8) | source->address.ipv4[3]);
    target->sin_port = source->port;
}
