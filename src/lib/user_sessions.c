/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <guise-serialize/serialize.h>
#include <guise-server-lib/address.h>
#include <guise-server-lib/unique_id.h>
#include <guise-server-lib/user_session.h>
#include <guise-server-lib/user_sessions.h>

/// Initialize the user session collection
/// @param self
/// @param log
void guiseUserSessionsInit(GuiseUserSessions* self, Clog log)
{
    self->log = log;
    self->userSessionCapacity = 1024;
    self->guiseUserSessions = tc_malloc_type_count(GuiseUserSession, self->userSessionCapacity);
    tc_mem_clear_type_n(self->guiseUserSessions, self->userSessionCapacity);
}

void guiseUserSessionsReset(GuiseUserSessions* self)
{
    for (size_t i = 0; i < self->userSessionCapacity; ++i) {
        GuiseUserSession* session = &self->guiseUserSessions[i];
        session->user = 0;
    }
}

void guiseUserSessionsDestroy(GuiseUserSessions* self)
{
    self->userSessionCapacity = 0;
    tc_free(self->guiseUserSessions);
}

int guiseUserSessionsCreate(GuiseUserSessions* sessions, struct GuiseUser* user, const NetworkAddress* address,
                       GuiseUserSession** outSession)
{
    for (size_t i = 0; i < sessions->userSessionCapacity; ++i) {
        GuiseUserSession* session = &sessions->guiseUserSessions[i];
        if (session->user == 0) {
            Clog userSessionLog;
            userSessionLog.config = sessions->log.config;
            tc_snprintf(session->prefix, 32, "%s/%zu", sessions->log.constantPrefix, i);
            userSessionLog.constantPrefix = session->prefix;
            GuiseSerializeUserSessionId uniqueSessionId = guiseGenerateUniqueIdFromIndex(i);
            guiseUserSessionInit(session, uniqueSessionId, address, user, userSessionLog);
            *outSession = session;
            return 0;
        }
    }
    *outSession = 0;
    return -1;
}

static int guiseUserSessionsFind(const GuiseUserSessions* self, GuiseSerializeUserSessionId uniqueId, const NetworkAddress* addr,
                            const GuiseUserSession** outSession)
{
    size_t index = guiseUniqueIdGetIndex(uniqueId);
    if (index >= self->userSessionCapacity) {
        return -2;
    }

    GuiseUserSession* foundSession = &self->guiseUserSessions[index];
    if (foundSession->userSessionId != uniqueId) {
        CLOG_C_SOFT_ERROR(&self->log, "wrong user session id, got %016X but wanted %016X", uniqueId,
                          foundSession->userSessionId);
    }
    if (!networkAddressEqual(addr, &foundSession->address)) {
        char addrTemp[64];
        CLOG_C_SOFT_ERROR(&self->log, "wrong address %s vs %s", networkAddressToString(addr, addrTemp, 64),
                          networkAddressToString(&foundSession->address, addrTemp, 64));
        *outSession = 0;
        return -3;
    }

    *outSession = foundSession;

    return 0;
}

int guiseUserSessionsReadAndFind(const GuiseUserSessions* self, const NetworkAddress* address, FldInStream* stream,
                            const GuiseUserSession** outSession)
{

    GuiseSerializeUserSessionId userSessionId;
    guiseSerializeReadUserSessionId(stream, &userSessionId);

    int errorCode = guiseUserSessionsFind(self, userSessionId, address, outSession);
    if (errorCode < 0) {
        CLOG_C_WARN(&self->log, "couldn't find user session %d", userSessionId);
        return errorCode;
    }

    return 0;
}
