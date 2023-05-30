/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "cipher.h"
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <guise-serialize/server_in.h>
#include <guise-serialize/server_out.h>
#include <guise-server-lib/req_user_login.h>
#include <guise-server-lib/server.h>
#include <guise-server-lib/user.h>
#include <guise-server-lib/user_session.h>

/// Try to login a user and creates a user session on success
/// @param self
/// @param address
/// @param data
/// @param len
/// @param outStream
/// @return
int guiseReqUserLogin(GuiseServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                     struct FldOutStream* outStream)
{
    if (len == 0) {
        return -2;
    }

    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    GuiseSerializeClientNonce clientNonce;
    GuiseSerializeServerChallenge serverChallengeFromClient;
    char username[32];

    guiseSerializeServerInLogin(&inStream, &clientNonce, &serverChallengeFromClient, username, 32);

    // Challenge is done to avoid at least the simplest forms of IP spoofing
    uint64_t calculatedClientNonce = extremelyUnsecureCipher(serverChallengeFromClient, self->secretChallengeKey);
    if (calculatedClientNonce != clientNonce) {
        CLOG_C_SOFT_ERROR(&self->log, "challenge didnt work out")
        return -2;
    }

    CLOG_C_DEBUG(&self->log, "challenge was approved from client nonce %016lX", clientNonce);

    GuiseUser* foundUser;
    int errorCode = guiseUsersReadLogin(&self->guiseUsers, username, &foundUser);
    if (errorCode < 0) {
        return errorCode;
    }

    GuiseUserSession* foundSession;
    int err = guiseUserSessionsCreate(&self->guiseUserSessions, foundUser, address, &foundSession);
    if (err < 0) {
        return err;
    }

    CLOG_C_DEBUG(&self->log, "logged in user '%s' and created user session %016lX", foundUser->name,
                 foundSession->userSessionId);

    guiseSerializeServerOutLogin(outStream, clientNonce, foundSession->userSessionId);

    return 0;
}
