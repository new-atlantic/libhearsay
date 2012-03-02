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

#ifndef HEARSAY_MESSAGE_H
#define HEARSAY_MESSAGE_H

// Message id length. The id is a hex-encoded sha256 hash of the message.
#define HEARSAY_MESSAGE_ID_LENGTH 64u

// Time and date (UTC) when message was created using ISO 8601 notation using
// the following format: YYYY-MM-DDThh:mm:ssZ.
#define HEARSAY_MESSAGE_TIMESTAMP_LENGTH 20u

typedef struct {
	// TODO: char version[];
	char id[HEARSAY_MESSAGE_ID_LENGTH + 1];
	char timestamp[HEARSAY_MESSAGE_TIMESTAMP_LENGTH + 1];
	// TODO: char sender_name[];
	// TODO: char sender_address[];
	// TODO: char sender_address_type[];
	char message_reference[HEARSAY_MESSAGE_ID_LENGTH + 1];
	// TODO: char content_type[];
	// TODO: char message_body[];
} hearsay_message;

extern int hearsay_message_calculate_hash (hearsay_message *message);

extern int hearsay_message_validate_hash  (hearsay_message *message);

#endif

