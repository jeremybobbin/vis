#ifndef LIBKEY_H
#define LIBKEY_H

#include <signal.h>
#include <stddef.h>
#include <stdbool.h>

/* maximum bytes needed for string representation of a (pseudo) key */
#define VIS_KEY_LENGTH_MAX 64

static const char *vis_keys_symbolic[] = {
	"Backspace",
	"Begin",
	"Cancel",
	"Clear",
	"Close",
	"Command",
	"Copy",
	"Delete",
	"Down",
	"End",
	"Enter",
	"Escape",
	"Exit",
	"Find",
	"Help",
	"Home",
	"Insert",
	"Left",
	"Mark",
	"Message",
	"Move",
	"Open",
	"Options",
	"PageDown",
	"PageUp",
	"Print",
	"Redo",
	"Reference",
	"Refresh",
	"Replace",
	"Restart",
	"Resume",
	"Right",
	"Save",
	"Select",
	"Space",
	"Suspend",
	"Tab",
	"Up",
};

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
const char *vis_keys_next(Vis*, const char *keys);

#endif
