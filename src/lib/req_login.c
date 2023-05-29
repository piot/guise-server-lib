/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "cipher.h"
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <user-serialize/server_in.h>
#include <user-serialize/server_out.h>
#include <user-server-lib/req_user_login.h>
#include <user-server-lib/server.h>
#include <user-server-lib/user.h>
#include <user-server-lib/user_session.h>

/// Try to login a user and creates a user session on success
/// @param self
/// @param address
/// @param data
/// @param len
/// @param outStream
/// @return
int userReqUserLogin(UserServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                     struct FldOutStream* outStream)
{
    if (len == 0) {
        return -2;
    }

    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    UserSerializeClientNonce clientNonce;
    UserSerializeServerChallenge serverChallengeFromClient;
    char username[32];

    userSerializeServerInLogin(&inStream, &clientNonce, &serverChallengeFromClient, username, 32);

    // Challenge is done to avoid at least the simplest forms of IP spoofing
    uint64_t calculatedClientNonce = extremelyUnsecureCipher(serverChallengeFromClient, self->secretChallengeKey);
    if (calculatedClientNonce != clientNonce) {
        CLOG_C_SOFT_ERROR(&self->log, "challenge didnt work out")
        return -2;
    }

    CLOG_C_DEBUG(&self->log, "challenge was approved from client nonce %016lX", clientNonce);

    User* foundUser;
    int errorCode = usersReadLogin(&self->users, username, &foundUser);
    if (errorCode < 0) {
        return errorCode;
    }

    UserSession* foundSession;
    int err = userSessionsCreate(&self->userSessions, foundUser, address, &foundSession);
    if (err < 0) {
        return err;
    }

    CLOG_C_DEBUG(&self->log, "logged in user '%s' and created user session %016lX", foundUser->name,
                 foundSession->userSessionId);

    userSerializeServerOutLogin(outStream, clientNonce, foundSession->userSessionId);

    return 0;
}
