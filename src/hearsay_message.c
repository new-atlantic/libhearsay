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

/**
 * @internal
 * @brief Definition of a Hearsay message represented as a struct.
 *
 * The initial representation is only based on chars. Though wasteful in terms
 * of space use it requires no conversion for outputting "text/hearsay".
 * 
 * @todo Future versions should probably be more sophisticated, using unions
 *       and structs for version and sender address. Endianness is not an issue
 *       as the message is transformed into UTF-8 encoded text before
 *       transmission.
 */

typedef struct {
	char version[HEARSAY_MESSAGE_VERSION_STRING_LENGTH + 1];
	// Hearsay API version string.
	char id[HEARSAY_MESSAGE_ID_LENGTH + 1];
	// Hash of the (non-volatile) message contents. 
	// Serves as a UUID.
	char timestamp[HEARSAY_MESSAGE_TIMESTAMP_LENGTH + 1];
	// Time when the message was created.
	char sender_name[HEARSAY_MESSAGE_SENDER_NAME_LENGTH +1];
	// Name of the sender (optional).
	char sender_address[HEARSAY_MESSAGE_SENDER_ADDRESS_LENGTH + 1];
	// Network address of the sender (optional)
	char sender_address_type[HEARSAY_MESSAGE_ADDRESS_TYPE_LENGTH + 1];
	// Type of network address (optional).
	char message_reference[HEARSAY_MESSAGE_ID_LENGTH + 1];
	// Shortest path to message sender (volatile).
	char n_hops[HEARSAY_MESSAGE_NUMBER_OF_HOPS_LENGTH + 1];
	// Id of message being referred to. 
	// Useful for replies and continued messages.
	char content_type[HEARSAY_MESSAGE_MAX_MIME_TYPE_LENGTH + 1];
	// MIME type of the message contents. 
	// Only sensible at the moment is text/plain.
	char message_body[HEARSAY_MESSAGE_BODY_MAX_LENGTH + 1];
	// The actual message.
} hearsay_message;

int hearsay_message_calculate_hash (hearsay_message *message){

	char *hash;
	char *hash_string;
	char *hearsay_message_content;
	char *hex_pair;

	size_t hearsay_message_max_content_length = \
		HEARSAY_MESSAGE_VERSION_STRING_LENGTH
		+ HEARSAY_MESSAGE_TIMESTAMP_LENGTH
		+ HEARSAY_MESSAGE_SENDER_NAME_LENGTH
		+ HEARSAY_MESSAGE_SENDER_ADDRESS_LENGTH
		+ HEARSAY_MESSAGE_ADDRESS_TYPE_LENGTH
		+ HEARSAY_MESSAGE_ID_LENGTH
		+ HEARSAY_MESSAGE_MAX_MIME_TYPE_LENGTH
		+ HEARSAY_MESSAGE_BODY_MAX_LENGTH
		+ 1;
	size_t binary_hash_length = gnutls_hash_get_len (GNUTLS_DIG_SHA256) + 1;
	
	hash = malloc (binary_hash_length);
	if (! hash) return 0;
	
	hash_string = calloc ((HEARSAY_MESSAGE_ID_LENGTH + 1), 1);
	if (! hash_string) return 0;
	
	hearsay_message_content = malloc (hearsay_message_max_content_length);
	if (! hearsay_message_content) return 0;

	hex_pair = malloc (3);
	if (! hex_pair) return 0;

	// Calculate the sha256 hash of the message.
	// TODO: Document formally and publicly over what part of the message
	//       the hash is calculated.	
	
	strncat (hearsay_message_content,
	         message->version,
	         HEARSAY_MESSAGE_VERSION_STRING_LENGTH);
	strncat (hearsay_message_content,
	         message->timestamp,
	         HEARSAY_MESSAGE_TIMESTAMP_LENGTH);
	strncat (hearsay_message_content,
	         message->sender_name,
	         HEARSAY_MESSAGE_SENDER_NAME_LENGTH);
	strncat (hearsay_message_content,
	         message->sender_address,
	         HEARSAY_MESSAGE_SENDER_ADDRESS_LENGTH);
	strncat (hearsay_message_content,
	         message->sender_address_type,
	         HEARSAY_MESSAGE_ADDRESS_TYPE_LENGTH);
	strncat (hearsay_message_content,
	         message->message_reference,
	         HEARSAY_MESSAGE_ID_LENGTH);
	strncat (hearsay_message_content,
	         message->content_type,
	         HEARSAY_MESSAGE_MAX_MIME_TYPE_LENGTH);
	strncat (hearsay_message_content,
	         message->message_body,
	         HEARSAY_MESSAGE_BODY_MAX_LENGTH);
	
	gnutls_hash_fast (GNUTLS_DIG_SHA256,
	                  hearsay_message_content,
	                  strnlen (hearsay_message_content,
	                           hearsay_message_max_content_length),
	                  hash);

	// Translate binary hash to base 16.
	// TODO: Probably not the most efficient way to do this.
	for (int i = 0;i < binary_hash_length; i++) {
		snprintf (hex_pair, 3, "%02hhx", hash[i]);
		strncat (hash_string, hex_pair, 2);
	}

	// Copy calculated hash to message id.
	memcpy (message->id, hash_string, sizeof(hash_string));
	
	// Force null byte at end of array.
	message->id[HEARSAY_MESSAGE_ID_LENGTH] = '\0';
	
	// TODO: Replace malloc/free with simple stack based allocation or
	//       alloca (portability issues with alloca...).
	free (hash);
	free (hash_string);
	free (hearsay_message_content);
	free (hex_pair);

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

int hearsay_message_text_validate (const char *text, site_t size)
{
	return 0;
}

hearsay_message *hearsay_message_text_to_struct (const char *text,
                                                 size_t size)
{
	return NULL;
}

char *hearsay_message_struct_to_text (hearsay_message *message)
{
	return NULL;
}
