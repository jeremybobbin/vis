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
	NULL
};

// returns length of first key, limited by len
int vis_keys_nlen(const char *keys, int len) {
	int i = 0, j;
	char mod = 0;

	if (len <= 0 || !keys || !*keys)
		return 0;

	if (len == 1) {
		return 1;
	}

	if (keys[i] == '<') {
		i++;
		if (strncmp(&keys[i], "vis-", 4) == 0) {
			for (i += strlen("vis-"); i < len && keys[i]; i++) {
				if (keys[i] == '>') {
					return i+1;
				}
			}
			return 1;
		}
		// <C-a> is minimum valid exapmle
		// i  : C
		// i+1: -
		// i+2: a
		// i+3: >
		for (; i+2 < len; i += 2) {
			if (keys[i] == '\0' || (keys[i] != 'C' && keys[i] != 'M' && keys[i] != 'S' ) || keys[i+1] != '-')  {
				break;
			}

			switch(keys[i]) {
			case 'C':
				if (mod & 1) {
					return 1;
				}
				mod |= 1;
				continue;
			case 'M':
				if (mod & 2) {
					return 1;
				}
				mod |= 2;
				continue;
			case 'S':
				if (mod & 4) {
					return 1;
				}
				mod |= 4;
				continue;
			}
			break;
		}

		// i+2 since this is terminal. We expect F, a number(1), then '>'(2)
		if (keys[i] == 'F' && i+2 < len && keys[i+1] >= '0' && keys[i+1] <= '9') {
			for (j = 2; j < 4 && i+j < len; j++) {
				if (keys[i+j] < '0' || keys[i+j] > '9') {
					break;
				}
			}
			i += j;
		} else if (i+1 < len && keys[i] && ((keys[i] & 0x80) == 0) && keys[i+1] && ((keys[i+1] & 0x80) == 0) && keys[i+1] != '>') {
			// example matches at this point
			// - <Home>
			// - <S-C-Ho
			// - <S-C-??????
			// - <S-C-??
			// non-matches
			// - a
			// - <S-C-S>
			// - <S-C-ㄦ>
			for (j = 0; vis_keys_symbolic[j]; j++) {
				int n = strlen(vis_keys_symbolic[j]);
				if (len-i < n+1) { // n+1 because of '>'
					continue;
				}
				if (strncmp(&keys[i], vis_keys_symbolic[j], n) != 0) {
					//no-op
				} else if (keys[i+n] == '>') {
					return i+n+1;
				}
			}
			// none of the sym keys match(example <C-Banana>)
			return 1;
		} else if (mod) {
			if (i+1 < len && ISUTF8(keys[i]))
				i++;
			while (i+1 < len && !ISUTF8(keys[i]))
				i++;
		} else {
			return 1;
		}

		if (i < len && keys[i] == '>') {
			return i+1;
		} else {
			return 1;
		}
	}

	if (i < len && ISUTF8(keys[i]))
		i++;
	while (i < len && !ISUTF8(keys[i]))
		i++;

	return i;
}

const char *vis_keys_next(const char *keys) {
	if (keys == NULL || *keys == '\0') {
		return NULL;
	}

	return keys+vis_keys_nlen(keys, strlen(keys));
}


int vis_keys_count(char *keys) {
	int n = 0;
	for (; keys && *keys; keys = (char *) vis_keys_next(keys))
		n++;
	return n;
}
