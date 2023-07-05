/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef GUISE_SERVER_REQ_SESSION_QUERY_H
#define GUISE_SERVER_REQ_SESSION_QUERY_H

struct GuiseServer;
struct GuiseUserSession;
struct FldOutStream;
struct FldInStream;


int guiseReqSessionQuery(struct GuiseServer* self, const struct GuiseUserSession* userSession, struct FldInStream* inStream,
                         struct FldOutStream* outStream);

#endif
