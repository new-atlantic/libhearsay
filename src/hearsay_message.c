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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gnutls/gnutls.h>
#include <gnutls/crypto.h>

#include "hearsay_message.h"

int hearsay_message_calculate_hash (hearsay_message *message){

	char *hash;
	char *hash_string;
	char *hex_pair;
	
	int binary_hash_length = gnutls_hash_get_len (GNUTLS_DIG_SHA256) + 1;
	
	hash = malloc (binary_hash_length);
	hash_string = calloc ((HEARSAY_MESSAGE_ID_LENGTH + 1), 1);
	hex_pair = malloc (3);

	// TODO: Replace dummy string with input message contents.	
	gnutls_hash_fast (GNUTLS_DIG_SHA256,
	                  "dummy_string",
	                  strlen("dummy_string"),
	                  hash);

	// Translate binary hash to base 16.
	// TODO: Probably not the most efficient way to do this.
	for (int i = 0;i < binary_hash_length; i++) {
		snprintf (hex_pair, 3, "%02hhx", hash[i]);
		strcat (hash_string, hex_pair);
	}

        // Force null byte at end of array.
	hash_string[HEARSAY_MESSAGE_ID_LENGTH] = '\0';

	// Copy calculated hash to message id.
	memcpy (message->id, hash_string, sizeof(hash_string));
	
	// Force null byte at end of array.
	message->id[HEARSAY_MESSAGE_ID_LENGTH] = '\0';
	
	// TODO: Replace malloc/free with simple stack based allocation or
	//       alloca (portability issues with alloca...).
	free (hash);
	free (hash_string);
	free (hex_pair);
	
	// TODO: Return 0 on error.
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

