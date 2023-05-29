/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <user-serialize/serialize.h>
#include <user-server-lib/address.h>
#include <user-server-lib/unique_id.h>
#include <user-server-lib/user_session.h>
#include <user-server-lib/user_sessions.h>

/// Initialize the user session collection
/// @param self
/// @param log
void userSessionsInit(UserSessions* self, Clog log)
{
    self->log = log;
    self->userSessionCapacity = 1024;
    self->userSessions = tc_malloc_type_count(UserSession, self->userSessionCapacity);
    tc_mem_clear_type_n(self->userSessions, self->userSessionCapacity);
}

void userSessionsReset(UserSessions* self)
{
    for (size_t i = 0; i < self->userSessionCapacity; ++i) {
        UserSession* session = &self->userSessions[i];
        session->user = 0;
    }
}

void userSessionsDestroy(UserSessions* self)
{
    self->userSessionCapacity = 0;
    tc_free(self->userSessions);
}

int userSessionsCreate(UserSessions* sessions, struct User* user, const NetworkAddress* address,
                       UserSession** outSession)
{
    for (size_t i = 0; i < sessions->userSessionCapacity; ++i) {
        UserSession* session = &sessions->userSessions[i];
        if (session->user == 0) {
            Clog userSessionLog;
            userSessionLog.config = sessions->log.config;
            tc_snprintf(session->prefix, 32, "%s/%zu", sessions->log.constantPrefix, i);
            userSessionLog.constantPrefix = session->prefix;
            UserSerializeUserSessionId uniqueSessionId = userGenerateUniqueIdFromIndex(i);
            userSessionInit(session, uniqueSessionId, address, user, userSessionLog);
            *outSession = session;
            return 0;
        }
    }
    *outSession = 0;
    return -1;
}

static int userSessionsFind(const UserSessions* self, UserSerializeUserSessionId uniqueId, const NetworkAddress* addr,
                            const UserSession** outSession)
{
    size_t index = userUniqueIdGetIndex(uniqueId);
    if (index >= self->userSessionCapacity) {
        return -2;
    }

    UserSession* foundSession = &self->userSessions[index];
    if (foundSession->userSessionId != uniqueId) {
        CLOG_C_SOFT_ERROR(&self->log, "wrong user session id, got %016X but wanted %016X", uniqueId,
                          foundSession->userSessionId);
    }
    if (!networkAddressEqual(addr, &foundSession->address)) {
        char addrTemp[64];
        CLOG_C_SOFT_ERROR(&self->log, "wrong address %s vs %s", userAddressToString(addr, addrTemp, 64),
                          userAddressToString(&foundSession->address, addrTemp, 64));
        *outSession = 0;
        return -3;
    }

    *outSession = foundSession;

    return 0;
}

int userSessionsReadAndFind(const UserSessions* self, const NetworkAddress* address, FldInStream* stream,
                            const UserSession** outSession)
{

    UserSerializeUserSessionId userSessionId;
    userSerializeReadUserSessionId(stream, &userSessionId);

    int errorCode = userSessionsFind(self, userSessionId, address, outSession);
    if (errorCode < 0) {
        CLOG_C_WARN(&self->log, "couldn't find user session %d", userSessionId);
        return errorCode;
    }

    return 0;
}
