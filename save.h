#ifndef SAVE_H
#define SAVE_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "text.h"

typedef struct TextSave TextSave;

/**
 * @}
 * @defgroup save
 * @{
 */
/**
 * Method used to save the text.
 */
enum TextSaveMethod {
	/** Automatically chose best option. */
	TEXT_SAVE_AUTO,
	/**
	 * Save file atomically using `rename(2)`.
	 *
	 * Creates a temporary file, restores all important meta data,
	 * before moving it atomically to its final (possibly already
	 * existing) destination using `rename(2)`. For new files,
	 * permissions are set to `0666 & ~umask`.
	 *
	 * @rst
	 * .. warning:: This approach does not work if:
	 *
	 *   - The file is a symbolic link.
	 *   - The file is a hard link.
	 *   - File ownership can not be preserved.
	 *   - File group can not be preserved.
	 *   - Directory permissions do not allow creation of a new file.
	 *   - POSXI ACL can not be preserved (if enabled).
	 *   - SELinux security context can not be preserved (if enabled).
	 * @endrst
	 */
	TEXT_SAVE_ATOMIC,
	/**
	 * Overwrite file in place.
	 * @rst
	 * .. warning:: I/O failure might cause data loss.
	 * @endrst
	 */
	TEXT_SAVE_INPLACE,
};

/**
 * Save the whole text to the given file name.
 */
bool text_save(Text*, const char *filename);
/**
 * Save the whole text to the given file name, using the specified method.
 *
 * @rst
 * .. note:: Equivalent to ``text_save_method(filename, TEXT_SAVE_AUTO)``.
 * @endrst
 */
bool text_save_method(Text*, const char *filename, enum TextSaveMethod);

/**
 * Setup a sequence of write operations.
 *
 * The returned `TextSave` pointer can be used to write multiple, possibly
 * non-contigious, file ranges.
 * @rst
 * .. warning:: For every call to `text_save_begin` there must be exactly
 *              one matching call to either `text_save_commit` or
 *              `text_save_cancel` to release the underlying resources.
 * @endrst
 */
TextSave *text_save_begin(Text*, const char *filename, enum TextSaveMethod);
/**
 * Write file range.
 * @return The number of bytes written or ``-1`` in case of an error.
 */
ssize_t text_save_write_range(TextSave*, Filerange*);
/**
 * Commit changes to disk.
 * @return Whether changes have been saved.
 * @rst
 * .. note:: Releases the underlying resources and `free(3)`'s the given `TextSave`
 *           pointer which must no longer be used.
 * @endrst
 */
bool text_save_commit(TextSave*);
/**
 * Abort a save operation.
 * @rst
 * .. note:: Does not guarantee to undo the previous writes (they might have been
 *           performed in-place). However, it releases the underlying resources and
 *           `free(3)`'s the given `TextSave` pointer which must no longer be used.
 * @endrst
 */
void text_save_cancel(TextSave*);
/** @} */

#endif
