/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <guise-serialize/server_in.h>
#include <guise-serialize/server_out.h>
#include <guise-server-lib/req_user_login.h>
#include <guise-server-lib/server.h>
#include <guise-server-lib/user.h>
#include <guise-server-lib/user_session.h>
#include <inttypes.h>

/// Try to login a user and creates a user session on success
/// @param self guise server
/// @param address address that the request came from
/// @param data the data following the login request
/// @param len octet size of data
/// @param outStream response out stream
/// @return negative on error
int guiseReqUserLogin(GuiseServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                      struct FldOutStream* outStream)
{
    if (len == 0) {
        return -2;
    }

    FldInStream inStream;
    fldInStreamInit(&inStream, data, len);

    GuiseSerializeClientNonce clientNonce;
    GuiseSerializeUserId userId;
    GuiseSerializePasswordHashWithChallenge passwordHashedWithChallenge;

    guiseSerializeServerInLogin(&inStream, &clientNonce, &userId, &passwordHashedWithChallenge);

    CLOG_C_DEBUG(&self->log, "challenge was approved from client nonce %" PRIx64, clientNonce)

    GuiseUser* foundUser;
    int errorCode = guiseUsersFind(&self->guiseUsers, userId, passwordHashedWithChallenge, &foundUser);
    if (errorCode < 0) {
        return errorCode;
    }

    GuiseUserSession* foundSession;
    int err = guiseUserSessionsCreate(&self->guiseUserSessions, foundUser, address, &foundSession);
    if (err < 0) {
        return err;
    }

    char tempAddressString[32];
    CLOG_C_DEBUG(&self->log,
                 "logged in user '%s' (userID: %" PRIX64 ") and created user session %" PRIx64 " from IP:%s",
                 foundUser->name.utf8, foundUser->id, foundSession->userSessionId,
                 networkAddressToString(address, tempAddressString, 32))

    GuiseSerializeAddress serializeAddress;
    networkAddressToSerializeAddress(&serializeAddress, foundSession->address);

    guiseSerializeServerOutLogin(outStream, clientNonce, &foundUser->name, foundSession->userSessionId,
                                 &serializeAddress);

    return 0;
}
