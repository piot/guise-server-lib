/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "cipher.h"
#include <clog/clog.h>
#include <guise-serialize/server_in.h>
#include <guise-serialize/server_out.h>
#include <guise-server-lib/req_challenge.h>
#include <guise-server-lib/server.h>
#include <guise-server-lib/user.h>
#include <guise-server-lib/user_session.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>

int guiseReqChallenge(GuiseServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                    struct FldOutStream* outStream)
{
    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    GuiseSerializeClientNonce clientNonce;
    guiseSerializeServerInChallenge(&inStream, &clientNonce);

    // Challenge is done to avoid at least the simplest forms of IP spoofing
    uint64_t constructedChallenge = extremelyUnsecureCipher(clientNonce, self->secretChallengeKey);

    CLOG_C_VERBOSE(&self->log, "received challenge request from client nonce %016lX and constructed challenge %016lX",
                   clientNonce, constructedChallenge)

    return guiseSerializeServerOutChallenge(outStream, clientNonce, constructedChallenge);
}
