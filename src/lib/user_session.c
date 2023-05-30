/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <guise-server-lib/user_session.h>

/// Initialize a user session
/// @param self
/// @param id
/// @param address
/// @param owner
/// @param log
void guiseUserSessionInit(GuiseUserSession* self, GuiseSerializeUserSessionId id, const NetworkAddress* address,
                     const struct GuiseUser* owner, Clog log)
{
    self->log = log;
    self->userSessionId = id;
    self->address = *address;
    self->user = owner;
}
