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
	///< @brief Hearsay API version string.
	char id[HEARSAY_MESSAGE_ID_LENGTH + 1];
	///< @brief Hash of the (non-optional) message contents. 
	///         Serves as a UUID.
	char timestamp[HEARSAY_MESSAGE_TIMESTAMP_LENGTH + 1];
	///< @brief Time when the message was created.
	char sender_name[HEARSAY_MESSAGE_SENDER_NAME_LENGTH +1];
	///< @brief Name of the sender (optional).
	char sender_address[HEARSAY_MESSAGE_SENDER_ADDRESS_LENGTH + 1];
	///< @brief Network address of the sender (optional)
	char sender_address_type[HEARSAY_MESSAGE_ADDRESS_TYPE_LENGTH + 1];
	///< @brief Type of network address (optional).
	char message_reference[HEARSAY_MESSAGE_ID_LENGTH + 1];
	///< @brief Id of message being referred to. 
	///         Useful for replies and continued messages.
	char content_type[HEARSAY_MESSAGE_MAX_MIME_TYPE_LENGTH + 1];
	///< @brief MIME type of the message contents. 
	///         Only sensible at the moment is text/plain.
	char message_body[HEARSAY_MESSAGE_BODY_MAX_LENGTH + 1];
	///< @brief The actual message.
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

extern int hearsay_message_text_validate (const char *text, site_t size);

/**
 * @brief Creates a hearsay_message from a text/hearsay representation.
 *
 * @param[in] text Character buffer containting a text/hearsay message.
 * @param[in] size Size of the buffer.
 *
 * @return Returns a pointer to hearsay_message struct. The memory allocated
 *         must be explicitly \c free()d.
 *
 * @retval NULL A null pointer is returned if there is a memory allocation
 *              error or the message is invalid. If you want to know which is
 *              the case you should validate the message first with
 *              hearsay_message_text_validate().
 */

extern hearsay_message *hearsay_message_text_to_struct (const char *text,
                                                        size_t size);

/**
 * @brief Creates a text/hearsay message from a hearsay_message struct.
 *
 * @param[in,out] message Pointer to initialized hearsay_message.
 *
 * @return Returns a pointer to a character buffer containing the text/hearsay
 *         representation of the message. The memory allocated must be
 *         explicitly \c free()d.
 *
 * @retval NULL A null pointer is returned if there is a memory allocation
 *              error or the message is invalid. If you want to know which is
 *              the case you should validate the message first with
 *              hearsay_message_text_validate().
 */

extern char *hearsay_message_struct_to_text (hearsay_message *message);

/**
 * @brief Validate a JSON representation of a Hearsay message.
 *
 * @param[in] json Pointer to a character buffer containing the JSON message.
 * @param[in] size Size of the char buffer.
 *
 * @retval 1 The JSON message is a valid Hearsay message.
 * @retval 0 The message is invalid.
 */

extern int hearsay_message_json_validate (const char *json, site_t size);

/**
 * @brief Create a hearsay_message struct from a JSON representation.
 *
 * @param[in] json Character buffer containing a JSON hearsay message.
 * @param[in] size Size of the char buffer.
 *
 * @return Returns a pointer to hearsay_message struct. The memory allocated
 *         must be explicitly \c free()d.
 *
 * @retval NULL A null pointer is returned if there is a memory allocation
 *              error or the message is invalid. If you want to know which is
 *              the case you should validate the message first with
 *              hearsay_message_text_validate().
 */

extern hearsay_message *hearsay_message_json_to_struct (const char *json,
                                                        size_t size);
/**
 * @brief Create a JSON representation of a Hearsay message. 
 *
 * @param[in,out] message Pointer to initialized hearsay_message. 
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

