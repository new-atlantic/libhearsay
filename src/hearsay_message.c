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

/// @file hearsay_message.c
/// @brief Function definitions for handling hearsay messages.
//  TODO: Split validation and transformation to separate .c files?

#include <ctype.h>
#include <string.h>

#include "hearsay_message.h"

int hearsay_message_calculate_hash (hearsay_message *message){

	// TODO: Proper initialization via a cryptographic hash function.
	char hash[HEARSAY_MESSAGE_ID_LENGTH + 1]
	 = "1234567890123456789012345678901234567890123456789012345678901234";

	// TODO: Use gnutls_hash_fast to calculate the hash. First concatenate
	//       the contents of the struct.

	// Copy calculated hash to message id.
	memcpy (message->id, hash, sizeof(hash));
	
	// Enforce null byte at end of array.
	message->id[HEARSAY_MESSAGE_ID_LENGTH] = '\0';
	return 1;
}

int hearsay_message_validate_hash (hearsay_message *message)
{
	char hash[HEARSAY_MESSAGE_ID_LENGTH + 1];

	// Test that the id consists of valid hexadecimal digits.
	for (unsigned int i = 0; i < HEARSAY_MESSAGE_ID_LENGTH; i++) {
		if (! isxdigit (message->id[i])) {
			return 0;
		}
	}

	// Create copy of original hash.
	memcpy (hash, message->id, sizeof(hash));

	// (Re-)calculate hash based on message.
	if (! hearsay_message_calculate_hash (message)) {
		return 0;
	}

	// Compare original and new hash.
	if (! strcmp (hash, message->id)) {
		return 1;
	} else {
		return 0;
	}
}

