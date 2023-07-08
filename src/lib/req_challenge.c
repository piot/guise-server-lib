/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <guise-serialize/server_in.h>
#include <guise-serialize/server_out.h>
#include <guise-server-lib/req_challenge.h>
#include <guise-server-lib/server.h>
#include <guise-server-lib/user.h>
#include <guise-server-lib/user_session.h>
#include <inttypes.h>
#include <secure-random/secure_random.h>

int guiseReqChallenge(GuiseServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                      struct FldOutStream* outStream)
{
    (void) address;
    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    GuiseSerializeClientNonce clientNonce;
    GuiseSerializeUserId userId;
    guiseSerializeServerInChallenge(&inStream, &userId, &clientNonce);

    GuiseUser* user = guiseUsersFindFromUserId(&self->guiseUsers, userId);
    if (user == 0) {
        return -1;
    }

    user->randomChallenge = secureRandomUInt64();

    CLOG_C_VERBOSE(&self->log,
                   "received challenge request from client nonce %" PRIx64 " and constructed challenge %" PRIx64,
                   clientNonce, user->randomChallenge)

    return guiseSerializeServerOutChallenge(outStream, clientNonce, user->randomChallenge);
}
