#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <stdbool.h>
#include "text.h"

/**
 * @file
 * A dynamically growing buffer storing arbitrary data.
 * @rst
 * .. note:: Used for Register, *not* Text content.
 * @endrst
 */

/** A dynamically growing buffer storing arbitrary data. */
typedef struct {
	char *data;    /**< Data pointer, ``NULL`` if empty. */
	size_t len;    /**< Current length of data. */
	size_t size;   /**< Maximal capacity of the buffer. */
} String;

/** Initialize a String object. */
void string_init(String*);
/** Release all resources, reinitialize buffer. */
void string_release(String*);
/** Set buffer length to zero, keep allocated memory. */
void string_clear(String*);
/** Reserve space to store at least ``size`` bytes.*/
bool string_reserve(String*, size_t size);
/** Reserve space for at least ``len`` *more* bytes. */
bool string_grow(String*, size_t len);
/** If buffer is non-empty, make sure it is ``NUL`` terminated. */
bool string_terminate(String*);
/** Set buffer content, growing the buffer as needed. */
bool string_put(String*, const void *data, size_t len);
/** Set buffer content to ``NUL`` terminated data. */
bool string_put0(String*, const char *data);
/** Remove ``len`` bytes starting at ``pos``. */
bool string_remove(String*, size_t pos, size_t len);
/** Insert ``len`` bytes of ``data`` at ``pos``. */
bool string_insert(String*, size_t pos, const void *data, size_t len);
/** Insert NUL-terminated data at pos. */
bool string_insert0(String*, size_t pos, const char *data);
/** Append further content to the end. */
bool string_append(String*, const void *data, size_t len);
/** Append NUL-terminated data. */
bool string_append0(String*, const char *data);
/** Insert ``len`` bytes of ``data`` at the start. */
bool string_prepend(String*, const void *data, size_t len);
/** Insert NUL-terminated data at the start. */
bool string_prepend0(String*, const char *data);
/** Set formatted buffer content, ensures NUL termination on success. */
bool string_printf(String*, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
/** Append formatted buffer content, ensures NUL termination on success. */
bool string_appendf(String*, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
/** Return length of a buffer without trailing NUL byte. */
size_t string_length0(String*);
/** Return length of a buffer including possible NUL byte. */
size_t string_length(String*);
/** Return current maximal capacity in bytes of this buffer. */
size_t string_capacity(String*);
/**
 * Get pointer to buffer data.
 * Guaranteed to return a NUL terminated string even if buffer is empty.
 */
const char *string_content0(String*);
/**
 * Get pointer to buffer data.
 * @rst
 * .. warning:: Might be NULL, if empty. Might not be NUL terminated.
 * @endrst
 */
const char *string_content(String*);
/**
 * Borrow underlying buffer data.
 * @rst
 * .. warning:: The caller is responsible to ``free(3)`` it.
 * @endrst
 */
char *string_move(String*);

#endif
