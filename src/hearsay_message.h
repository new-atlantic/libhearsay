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

/// @file hearsay_message.h
/// @brief Hearsay message format defintion and function prototypes.
#ifndef HEARSAY_MESSAGE_H
#define HEARSAY_MESSAGE_H

/// @brief Hearsay API version string length. Format M.N or MM.NN.
#define HEARSAY_MESSAGE_VERSION_STRING_LENGTH 5u

/// @brief Message id length: fits a hex-encoded sha256 hash of the message.
#define HEARSAY_MESSAGE_ID_LENGTH 64u

/// @brief Time and date (UTC) when message was created using ISO 8601 notation
///        in the following format: YYYY-MM-DDThh:mm:ssZ.
#define HEARSAY_MESSAGE_TIMESTAMP_LENGTH 20u

/// @brief Message sender name field length. Arbitrary prime number as limit.
#define HEARSAY_MESSAGE_SENDER_NAME_LENGTH 73u

/// @brief Message sender address length. Longest value based on IPv6 as chars.
#define HEARSAY_MESSAGE_SENDER_ADDRESS_LENGTH 71u

/// @brief Address type length. Current options are MAC, IPv4 and IPv6.
#define HEARSAY_MESSAGE_ADDRESS_TYPE_LENGTH 4u

/// @brief Maximum length of a mime-type header (RFC 4288)
#define HEARSAY_MESSAGE_MAX_MIME_TYPE_LENGTH 255u

/// @brief Maximum length for the message content. Arbitrary prime number.
#define HEARSAY_MESSAGE_BODY_MAX_LENGTH 2011u

/**
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
	char id[HEARSAY_MESSAGE_ID_LENGTH + 1];
	char timestamp[HEARSAY_MESSAGE_TIMESTAMP_LENGTH + 1];
	char sender_name[HEARSAY_MESSAGE_SENDER_NAME_LENGTH +1];
	char sender_address[HEARSAY_MESSAGE_SENDER_ADDRESS_LENGTH + 1];
	char sender_address_type[HEARSAY_MESSAGE_ADDRESS_TYPE_LENGTH + 1];
	char message_reference[HEARSAY_MESSAGE_ID_LENGTH + 1];
	char content_type[HEARSAY_MESSAGE_MAX_MIME_TYPE_LENGTH + 1];
	char message_body[HEARSAY_MESSAGE_BODY_MAX_LENGTH + 1];
} hearsay_message;

/// @brief MIME-type for Hearsay messages as plain text.
#define HEARSAY_MESSAGE_TEXT_MIME "text/hearsay"

///@brief Maximum legal length for a "text/hearsay" representation.
// TODO: Count maximum length based on headers (title + content) and body.
#define HEARSAY_MESSAGE_TEXT_MAXIMUM_SIZE 0u

/**
 * @brief Check that the Hearsay message contains valid data.
 *
 * @param[in] message A hearsay_message struct with data.
 *
 * @retval 1 The data are valid.
 * @retval 0 The data in the struct are invalid.
 */

extern int hearsay_message_validate (const hearsay_message *message);

/**
 * @brief Calculate the hash for the given Hearsay message.
 *
 * @param[in, out] message A hearsay_message struct with data.
 *
 * @retval 1 The hash was succefully updated in the struct.
 * @retval 0 The data in the struct are invalid.
 */

extern int hearsay_message_calculate_hash (hearsay_message *message);

/**
 * @brief Check that the hash of the given Hearsay message is valid.
 *
 * @param[in, out] message A hearsay_message struct with data.
 *
 * @retval 1 The hash was valid.
 * @retval 0 The hash does not match the data.
 */

extern int hearsay_message_validate_hash  (hearsay_message *message);

/**
 * @brief Checks that the given message is a proper text/hearsay representation.
 */

extern int hearsay_message_text_validate (char *text, site_t size);

/**
 * @brief
 *
 * @param
 *
 * @return
 *
 * @retval
 */

extern hearsay_message *hearsay_message_text_to_struct (char *text,
                                                        size_t size);

/**
 * @brief Creates a hearsay_message from a text/hearsay representation.
 *
 * @param[in] text Text buffer containting text/hearsay message.
 * @param[in] size Size of the buffer.
 *
 * @return Returns a pointer to hearsay_message struct.
 *
 * @retval NULL A null pointer is returned if there is a memory allocation
 *              error or the message is invalid. If you want to know which is
 *              the case you should validate the message first with
 *              hearsay_message_text_validate().
 */

extern char *hearsay_message_struct_to_text (hearsay_message *message);

/**
 * @brief
 *
 * @param
 *
 * @return
 *
 * @retval
 */

extern int hearsay_message_json_validate (char *json, site_t size);

/**
 * @brief
 *
 * @param
 *
 * @return
 *
 * @retval
 */

extern hearsay_message *hearsay_message_json_to_struct (char *json,
                                                        size_t size);
/**
 * @brief Create a JSON representation of a Hearsay message. 
 *
 * @param[in] message An initialized hearsay_message. 
 *
 * @return Returns a pointer to a char buffer containing the JSON
 *         representation. \c free() has to be explicitly called for the
 *         allocated memory.
 *
 * @retval  NULL A null pointer is returned if there is a memory allocation
 *               error or the message is invalid.
 */

extern char *hearsay_message_struct_to_json (hearsay_message *message);

#endif

