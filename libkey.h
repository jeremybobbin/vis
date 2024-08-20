#ifndef LIBKEY_H
#define LIBKEY_H

#include <stddef.h>
#include <stdbool.h>

/* maximum bytes needed for string representation of a (pseudo) key */
#define VIS_KEY_LENGTH_MAX 64

extern const char *vis_keys_symbolic[];

/**
 * @}
 * @defgroup vis_keys
 * @{
 */
/**
 * Advance to the start of the next symbolic key.
 *
 * Given the start of a symbolic key, returns a pointer to the start of the one
 * immediately following it.
 */
const char *vis_keys_next(const char *keys);
/**
 * Count keys
 *
 * printf("abc: %d\n", vis_keys_count("abc"));
 * printf("abc<Home>: %d\n", vis_keys_count("abc<Home>"));
 * printf("abc<Home: %d\n", vis_keys_count("abc<Home"));
 * abc: 3
 * abc<Home>: 4
 * abc<Home: 8
 */
const char *vis_keys_next(const char *keys);

#endif
