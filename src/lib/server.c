/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <flood/out_stream.h>
#include <guise-serialize/commands.h>
#include <guise-server-lib/address.h>
#include <guise-server-lib/req_challenge.h>
#include <guise-server-lib/req_session_query.h>
#include <guise-server-lib/req_user_login.h>
#include <guise-server-lib/server.h>
#include <guise-server-lib/user.h>
#include <guise-server-lib/user_session.h>
#include <guise-server-lib/utils.h>
#include <imprint/allocator.h>

/// Handle incoming datagrams
/// @param self guise server
/// @param address address that the datagram came from
/// @param data the datagram payload
/// @param len the octet size of data
/// @param response response information
/// @return negative on error
int guiseServerFeed(GuiseServer* self, const NetworkAddress* address, const uint8_t* data, size_t len,
                    GuiseServerResponse* response)
{
    // CLOG_C_VERBOSE("guiseServerFeed: feed: %s octetCount: %zu", guiseSerializeCmdToString(data[0]), len)
#define UDP_MAX_SIZE (1200)
    static uint8_t buf[UDP_MAX_SIZE];
    FldOutStream outStream;
    fldOutStreamInit(&outStream, buf, UDP_MAX_SIZE);
    int result = -1;
    switch (data[0]) {
        case guiseSerializeCmdChallenge:
            result = guiseReqChallenge(self, address, data + 1, len - 1, &outStream);
            break;
        case guiseSerializeCmdLogin:
            result = guiseReqUserLogin(self, address, data + 1, len - 1, &outStream);
            break;
        default: {
            FldInStream inStream;
            fldInStreamInit(&inStream, data + 1, len - 1);
            const GuiseUserSession* foundUserSession;
            int err = guiseUserSessionsReadAndFind(&self->guiseUserSessions, address, &inStream, &foundUserSession);
            if (err < 0) {
                return err;
            }
            switch (data[0]) {
                case guiseSerializeCmdConfirmInfoRequest:
                    result = guiseReqSessionQuery(self, foundUserSession, &inStream, &outStream);
                    break;
                default:
                    CLOG_C_SOFT_ERROR(&self->log, "guiseServerFeed: unknown command %02X", data[0])
                    return 0;
            }
        }
    }

    if (result < 0) {
        return result;
    }

    return response->sendDatagram.send(response->sendDatagram.self, address, outStream.octets, outStream.pos);
}

/// Initialize the user server
/// @param self guise server
/// @param memory memory
/// @param log target log
/// @return zero
int guiseServerInit(GuiseServer* self, struct ImprintAllocator* memory, Clog log)
{
    (void) memory;
    self->log = log;

    Clog subLog;
    subLog.config = log.config;

    tc_snprintf(self->guiseUserSessions.prefix, 32, "%s/usersessions", log.constantPrefix);
    subLog.constantPrefix = self->guiseUserSessions.prefix;
    guiseUserSessionsInit(&self->guiseUserSessions, subLog);

    tc_snprintf(self->guiseUsers.prefix, 32, "%s/users", log.constantPrefix);
    subLog.constantPrefix = self->guiseUsers.prefix;
    guiseUsersInit(&self->guiseUsers, subLog);

    return 0;
}

void guiseServerReset(GuiseServer* self)
{
    guiseUserSessionsReset(&self->guiseUserSessions);
    guiseUsersReset(&self->guiseUsers);
}

void guiseServerDestroy(GuiseServer* self)
{
    guiseUserSessionsDestroy(&self->guiseUserSessions);
    guiseUsersDestroy(&self->guiseUsers);
}
