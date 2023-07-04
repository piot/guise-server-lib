/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <guise-server-lib/user_session.h>

/// Initialize a user session
/// @param self user session
/// @param id user session id
/// @param address network address for the session
/// @param owner owner of the session
/// @param log target log
void guiseUserSessionInit(GuiseUserSession* self, GuiseSerializeUserSessionId id, const NetworkAddress* address,
                     const struct GuiseUser* owner, Clog log)
{
    self->log = log;
    self->userSessionId = id;
    self->address = *address;
    self->user = owner;
}
