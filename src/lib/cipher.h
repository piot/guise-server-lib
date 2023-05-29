/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef USER_SERVER_LIB_CIPHER_H
#define USER_SERVER_LIB_CIPHER_H

#include <stdint.h>

uint64_t extremelyUnsecureCipher(uint64_t publicKey, uint64_t secretKey);

#endif
