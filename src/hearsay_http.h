/*
 * Copyright 2012 Torsti Schulz
 *
 * This file is part of the Hearsay library.
 *
 * Hearsay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hearsay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HEARSAY_HTTP_H
#define HEARSAY_HTTP_H

// Default server port for HTTP-based Hearsay-message exchange.
#define HEARSAY_HTTP_SERVER_PORT 1687


/*** Hearsay HTTP API ***
 *
 ** Resources **
 *
 * Resources that a HTTP-based Hearsay implementation should serve.
 *
 * Supported methods are GET and POST. DELETE should only be available for
 * authenticated or local clients, i.e. the user interface. PUT is not
 * supported as all messages are identified by their hash and changing the
 * message* changes their id. Thus there is no meaningful way to replace a
 * message.
 *
 * *) This is not true for volatile attributes like hops-to-sender, but that
 *    alone does not warrant implementing PUT.
 *
 * Resource          [Methods]  Description
 * ----------------------------------------------------------------------------
 * hearsay             [GET]  - Resource describes the Hearsay implementation.
 *
 * hearsay/messages    [GET]  - Lists available public (shared) messages.
 *                              Request can be qualified by a query string.
 *                              TODO: List minimal supported query options.
 *
 * hearsay/message/new [POST] - Post a new message, the hash/id will be derived
 *                              from the message.
 *
 * hearsay/message/<message:id> [GET, POST]
 *                            - GET: Retrieve the message with given id. Return
 *                                   status should not distinguish between 
 *                                   messages that do not exist and private
 *                                   (not shared) messages.
 *                            - POST: Create the message with given id, if it
 *                                    does not exist. Recipient should verify
 *                                    that the id is the hash of the message.
 *
 ** Content-types **
 *
 * A HTTP-based hearsay implementation should support the following content
 * types:
 *
 * - text/plain
 * - text/hearsay
 * - application/json
 *
 * It may also support the following content types:
 *
 * - application/xml
 * - text/html
 */

#define HEARSAY_HTTP_RESOURCE_HEARSAY        "/hearsay"
#define HEARSAY_HTTP_RESOURCE_MESSAGES       "/hearsay/messages"
#define HEARSAY_HTTP_RESOURCE_MESSAGE_PREFIX "/hearsay/message/"
#define HEARSAY_HTTP_RESOURCE_NEW_MESSAGE    "/hearsay/message/new"

#endif

