/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <flood/in_stream.h>
#include <guise-serialize/query_server_in.h>
#include <guise-serialize/query_server_out.h>
#include <guise-server-lib/req_session_query.h>
#include <guise-server-lib/server.h>
#include <guise-server-lib/user.h>
#include <guise-server-lib/user_session.h>
#include <inttypes.h>

int guiseReqSessionQuery(GuiseServer* self, const GuiseUserSession* userSession, FldInStream* inStream,
                         struct FldOutStream* outStream)
{
    GuiseSerializeUserSessionId sessionIdToLookup;
    GuiseSerializeAddress addressToLookup;

    int readErr = guiseSerializeServerInInfoRequest(inStream, &sessionIdToLookup, &addressToLookup);
    if (readErr < 0) {
        return readErr;
    }

    CLOG_C_INFO(&self->log, "checking sessionId %" PRIx64, sessionIdToLookup)

    if ((userSession->user->roles & GuiseSerializeRoleQuery) == 0) {
        CLOG_C_SOFT_ERROR(&self->log, "user has no authorization to query about sessions %" PRIx64,
                          userSession->user->id)
        return -4;
    }

    // int guiseUserSessionsFind(const GuiseUserSessions* self, GuiseSerializeUserSessionId uniqueId, const
    // NetworkAddress* addr,
    //                             const GuiseUserSession** outSession);

    NetworkAddress convertedNetworkAddressToUseForLookup;
    networkAddressFromSerializeAddress(&convertedNetworkAddressToUseForLookup, &addressToLookup);
    const GuiseUserSession* foundSession;
    int findErr = guiseUserSessionsFind(&self->guiseUserSessions, sessionIdToLookup,
                                        &convertedNetworkAddressToUseForLookup, &foundSession);
    if (findErr < 0) {
        return findErr;
    }

    GuiseSerializeConfirmResponse response;
    response.userId = foundSession->user->id;
    response.userSessionId = foundSession->userSessionId;
    response.userName = foundSession->user->name;
    response.address = addressToLookup;

    guiseSerializeServerOutInfoResponse(outStream, &response);

    return 0;
}
