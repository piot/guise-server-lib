/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <imprint/allocator.h>
#include <secure-random/secure_random.h>
#include <user-serialize/commands.h>
#include <user-server-lib/address.h>
#include <user-server-lib/req_challenge.h>
#include <user-server-lib/req_user_login.h>
#include <user-server-lib/server.h>
#include <user-server-lib/user.h>
#include <user-server-lib/user_session.h>
#include <user-server-lib/utils.h>

/// Handle incoming datagrams
/// @param self
/// @param address
/// @param data
/// @param len
/// @param response
/// @return
int userServerFeed(UserServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                   UserServerResponse* response)
{
    // CLOG_C_VERBOSE("userServerFeed: feed: %s octetCount: %zu", userSerializeCmdToString(data[0]), len)
#define UDP_MAX_SIZE (1200)
    static uint8_t buf[UDP_MAX_SIZE];
    FldOutStream outStream;
    fldOutStreamInit(&outStream, buf, UDP_MAX_SIZE);
    int result = -1;
    switch (data[0]) {
        case userSerializeCmdChallenge:
            result = userReqChallenge(self, address, data + 1, len - 1, &outStream);
            break;
        case userSerializeCmdLogin:
            result = userReqUserLogin(self, address, data + 1, len - 1, &outStream);
            break;
        default: {
            FldInStream inStream;
            fldInStreamInit(&inStream, data + 1, len - 1);
            const UserSession* foundUserSession;
            int err = userSessionsReadAndFind(&self->userSessions, address, &inStream, &foundUserSession);
            if (err < 0) {
                return err;
            }
            switch (data[0]) {
                default:
                    CLOG_C_SOFT_ERROR(&self->log, "userServerFeed: unknown command %02X", data[0]);
                    return 0;
            }
            break;
        }
    }

    if (result < 0) {
        return result;
    }

    return response->sendDatagram.send(response->sendDatagram.self, address, outStream.octets, outStream.pos);
}

/// Initialize the user server
/// @param self
/// @param memory
/// @param log
/// @return
int userServerInit(UserServer* self, struct ImprintAllocator* memory, Clog log)
{
    self->log = log;

    self->secretChallengeKey = secureRandomUInt64();

    Clog subLog;
    subLog.config = log.config;

    tc_snprintf(self->userSessions.prefix, 32, "%s/usersessions", log.constantPrefix);
    subLog.constantPrefix = self->userSessions.prefix;
    userSessionsInit(&self->userSessions, subLog);

    tc_snprintf(self->users.prefix, 32, "%s/users", log.constantPrefix);
    subLog.constantPrefix = self->users.prefix;
    usersInit(&self->users, subLog);

    return 0;
}

void userServerReset(UserServer* self)
{
    userSessionsReset(&self->userSessions);
    usersReset(&self->users);
}

void userServerDestroy(UserServer* self)
{
    userSessionsDestroy(&self->userSessions);
    usersDestroy(&self->users);
}
