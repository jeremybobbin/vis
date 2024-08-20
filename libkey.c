#include <string.h>

#include "libkey.h"
#include "libutf.h"
#include "util.h"

const char *vis_keys_symbolic[] = {
	"Backspace",
	"Begin",
	"Cancel",
	"Clear",
	"Close",
	"Command",
	"Copy",
	"Delete",
	"DeleteCharacter",
	"Down",
	"End",
	"Enter",
	"Escape",
	"Exit",
	"Find",
	"Help",
	"Home",
	"Insert",
	"KPUpperLeft",
	"KPUpperRight",
	"KPLowerLeft",
	"KPLowerRight",
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
	NULL
};

const char *vis_keys_next(const char *keys) {
	if (!keys || !*keys)
		return NULL;
	int i = 0, j;
	char mod = 0;

	if (keys[i] == '<') {
		i++;
		if (strncmp(&keys[i], "vis-", 4) == 0) {
			const char *start = keys + 1, *end = start;
			while (end - start < VIS_KEY_LENGTH_MAX && *end && *end != '>')
				end++;
			if (end > start && *end == '>') {
				return end + 1;
			}
		}
		for (;;) {
			switch (keys[i]) {
			case 'C':
			case 'M':
			case 'S':
				if (keys[i+1] != '-') {
					break;
				}
				switch(keys[i]) {
				case 'C':
					if (mod & 1) {
						return &keys[1];
					}
					mod |= 1;
					break;
				case 'M':
					if (mod & 2) {
						return &keys[1];
					}
					mod |= 2;
					break;
				case 'S':
					if (mod & 4) {
						return &keys[1];
					}
					mod |= 4;
					break;
				}
				i += 2;
				continue;
			case '\0':
				return &keys[1];
			}
			break;
		}

		if (keys[i] == 'F' && keys[i+1] >= '0' && keys[i+1] <= '9') {
			for (j = 2; j < 4; j++) {
				if (keys[i+j] < '0' || keys[i+j] > '9') {
					break;
				}
			}
			i += j;
		} else if (keys[i] && ((keys[i] & 0x80) == 0) && keys[i+1] && ((keys[i+1] & 0x80) == 0) && keys[i+1] != '>') {
			// start, end
			for (j = 0; vis_keys_symbolic[j]; j++) {
				if (strncmp(&keys[i], vis_keys_symbolic[j], strlen(vis_keys_symbolic[j])) != 0) {
					//no-op
				} else if (keys[i+strlen(vis_keys_symbolic[j])] == '>') {
					return &keys[i+strlen(vis_keys_symbolic[j])+1];
				}
			}
		} else if (mod) {
			if (ISUTF8(keys[i]))
				i++;
			while (!ISUTF8(keys[i]))
				i++;
		} else {
			return &keys[1];
		}

		if (keys[i] == '>') {
			return &keys[i+1];
		} else {
			return &keys[1];
		}
	}


	if (ISUTF8(keys[i])) {
		i++;
		while (!ISUTF8(keys[i]))
			i++;
		// TODO: are "!ISUTF8" chars valid like <C-???>
	} else {
		return &keys[1];
	}

	return &keys[i];
}


int vis_keys_count(char *keys) {
	int n = 0;
	for (; keys && *keys; keys = vis_keys_next(keys))
		n++;
	return n;
}

