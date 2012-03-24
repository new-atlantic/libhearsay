/*
 * Copyright 2012 Torsti Schulz
 *
 * This file is part of the Hearsay library.
 *
 * Hearsay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hearsay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/// @file  hearsay_http.h
/// @brief Definitions relevant for exchanging Hearsay messages over HTTP.

#ifndef HEARSAY_HTTP_H
#define HEARSAY_HTTP_H

/// @brief Default server port for HTTP-based Hearsay-message exchange.
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

/// @brief [GET] Resource describing the Hearsay implementation.
#define HEARSAY_HTTP_RESOURCE_HEARSAY        "/hearsay"
/// @brief [GET] Resource listing available public (shared) messages. Request
///              can be qualified by a query string.
///              TODO: List minimal supported query options.
#define HEARSAY_HTTP_RESOURCE_MESSAGES       "/hearsay/messages"
/// @brief Prefix for the message resource.

/**
 * hearsay/message/<message:id> [GET, POST]
 *                            - GET: Retrieve the message with given id. Return
 *                                   status should not distinguish between
 *                                   messages that do not exist and private
 *                                   (not shared) messages.
 *                            - POST: Create the message with given id, if it
 *                                    does not exist. Recipient should verify
 *                                    that the id is the hash of the message.
 */

#define HEARSAY_HTTP_RESOURCE_MESSAGE_PREFIX "/hearsay/message/"
/// @brief [POST] Resource for posting a new message, the hash/id will be
///               derived from the message.
#define HEARSAY_HTTP_RESOURCE_NEW_MESSAGE    "/hearsay/message/new"

#endif
