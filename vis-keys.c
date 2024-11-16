#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "map.h"
#include "ui-terminal-keytab.h"
#include "libkey.h"
#include "util.h"

#define RESUME  (ui_terminal_control[0][1])
#define SUSPEND (ui_terminal_control[1][1])

Map *keymap = NULL;

static bool map_put_recursive(Map *m, const char *k, const char *v) {
	char *sp;
	while ((sp = map_get(m, v))) {
		map_delete(m, v);
		v = sp;
	}
	return map_put(m, k, v);
}

static int decode(char *key, char *buf, size_t len) {
	// turns input from stdin into <C-V>gUi<F4> ...
	size_t i, n;
	if (len == 0) {
		key[0] = '\0';
		return 0;
	}

	char *k;
	const Map *tmp, *map;

	for (i = 1, map = keymap, n = 1; n && i < len; i++) {
		tmp = map_prefix_sized(map, &buf[0], i);
		if ((n = map_count(tmp))) {
			map = tmp;
		}
	}

	// if we've seen the whole buffer, then return the closest match
	if (n && (k = map_get_sized(map, &buf[0], i))) {
		sprintf(key, "<%s>", k);
		return i;
	}

	i = 0;

	if (len > 1 && buf[0] == 0x1b) {
		// might be an <M-...> key
		if ((k = map_get_sized(keymap, &buf[1], len-1))) {
			// <M-Home>, <M-End>, <M-Backspace>
			sprintf(&key[0], "<M-%s>", k);
			return len;
		} else if (buf[1] < 0x20 && isprint(buf[1]|0x60)) {
			// <M-C-u>, <M-C-a>, <M-C-^>
			sprintf(&key[0], "<M-C-%c>", (buf[1]|(isalpha(buf[1]|0x40) ? 0x60 : 0x40)));
			return 2;
		} else if (isprint(buf[1])) {
			sprintf(&key[0], "<M-%c>", buf[1]);
			return 2;
		}
	}

	if (buf[0] < 0x20 && isprint(buf[0]|0x60)) {
		sprintf(&key[0], "<C-%c>", (buf[0]|(isalpha(buf[0]|0x40) ? 0x60 : 0x40)));
		return 1;
	}

	if (len > 1 && ISUTF8(buf[0])) {
		i = 1;
		// TODO - could be an incomplete sequence if the buffer was too small
		while (i < len && !ISUTF8(buf[i])) {
			i++;
		}
		memcpy(&key[0], &buf[0], i);
		key[i] = '\0';
		return i;
	}

	key[0] = buf[0];
	key[1] = '\0';
	return 1;
}

int main() {
	char kb[VIS_KEY_LENGTH_MAX*2], buf[BUFSIZ];
	const char *key;
	int i, j, k, n;

	if ((keymap = map_new()) == NULL) {
		return 1;
	}

	for (i = 0; i < LENGTH(ui_terminal_keytab); i++) {
		if (!map_put_recursive(keymap, ui_terminal_keytab[i][0], ui_terminal_keytab[i][2])) {
			return 1;
		}
	}

	for (i = 0; i < LENGTH(ui_terminal_keytab); i++) {
		if (ui_terminal_keytab[i][1][0] == '^' && ui_terminal_keytab[i][1][1]) {
			kb[0] = ui_terminal_keytab[i][1][1]^0x40;
			kb[1] = '\0';
			key = (const char*) kb;
		} else {
			key = ui_terminal_keytab[i][1];
		}
		// this may error if there's a bug in infocmp code - we'll just ignore it.
		map_put_recursive(keymap, key, ui_terminal_keytab[i][0]);
	}

	// same way libtermkey does it
	if (!(
		map_put_recursive(keymap, "\x1b", "Escape") && // ???, 27, 0x1b, 0o33
		map_put_recursive(keymap, "\r",   "Enter")  && // \n,  10, 0xa,  0o12,
		map_put_recursive(keymap, "\t",   "Tab")       // \t,  9,  0x9,  0o11,
	)) {
		return 1;
	}

	struct termios display;
	tcgetattr(2, &display);
	display.c_iflag &= ~(IXON|INLCR|ICRNL);
	display.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(2, TCSANOW, &display);

	if (write(2, RESUME, sizeof(RESUME)-1) != sizeof(RESUME)-1) {
		fprintf(stderr, "Failed to initialize display: '%s'", errno ? strerror(errno) : "");
	}

	for (i = 0, j = 0, k = 0;;) {
		 if (i == 0) {
			if ((n = read(0, &buf[i], sizeof(buf)-i)) == 0) {
				return 0;
			} else if (n == -1) {
				fprintf(stderr, "vis-keys - read failed: %s\n", strerror(errno));
				return 1;
			} else {
				i += n;
			}
		} else if (j < i) {
			j += decode(&kb[k], &buf[j], i-j);
			if ((n = write(1, &kb[k], strlen(kb)-k)) == 0) {
				return 0;
			} else if (n == -1) {
				fprintf(stderr, "vis-keys - write failed: %s\n", strerror(errno));
				return 1;
			} else {
				k += n;
			}
		} else {
			i = 0;
			j = 0;
			k = 0;
		}
	}
	
	return 0;
}
