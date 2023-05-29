/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "cipher.h"
#include <clog/clog.h>
#include <user-serialize/server_in.h>
#include <user-serialize/server_out.h>
#include <user-server-lib/req_challenge.h>
#include <user-server-lib/server.h>
#include <user-server-lib/user.h>
#include <user-server-lib/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>

int userReqChallenge(UserServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                    struct FldOutStream* outStream)
{
    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    UserSerializeClientNonce clientNonce;
    userSerializeServerInChallenge(&inStream, &clientNonce);

    // Challenge is done to avoid at least the simplest forms of IP spoofing
    uint64_t constructedChallenge = extremelyUnsecureCipher(clientNonce, self->secretChallengeKey);

    CLOG_C_VERBOSE(&self->log, "received challenge request from client nonce %016lX and constructed challenge %016lX",
                   clientNonce, constructedChallenge)

    return userSerializeServerOutChallenge(outStream, clientNonce, constructedChallenge);
}
