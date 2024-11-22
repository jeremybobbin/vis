/* This file is included from ui-terminal.c
 *
 * The goal is *not* to reimplement curses. Instead we aim to provide the
 * simplest possible drawing backend for VT-100 compatible terminals.
 * This is useful for debugging and fuzzing purposes as well as for environments
 * with no curses support.
 *
 * Currently no attempt is made to optimize terminal output. The amount of
 * flickering will depend on the smartness of your terminal emulator.
 *
 * The following terminal escape sequences are used:
 *
 *  - CSI ? 1049 h             Save cursor and use Alternate Screen Buffer (DECSET)
 *  - CSI ? 1049 l             Use Normal Screen Buffer and restore cursor (DECRST)
 *  - CSI ? 25 l               Hide Cursor (DECTCEM)
 *  - CSI ? 25 h               Show Cursor (DECTCEM)
 *  - CSI 2 J                  Erase in Display (ED)
 *  - CSI row ; column H       Cursor Position (CUP)
 *  - CSI ... m                Character Attributes (SGR)
 *    - CSI 0 m                     Normal
 *    - CSI 1 m                     Bold
 *    - CSI 3 m                     Italicized
 *    - CSI 4 m                     Underlined
 *    - CSI 5 m                     Blink
 *    - CSI 7 m                     Inverse
 *    - CSI 22 m                    Normal (not bold)
 *    - CSI 23 m                    Not italicized
 *    - CSI 24 m                    Not underlined
 *    - CSI 25 m                    Not blinking
 *    - CSI 27 m                    Not inverse
 *    - CSI 30-37,39                Set foreground color
 *    - CSI 38 ; 2 ; R ; G ; B m    Set RGB foreground color
 *    - CSI 40-47,49                Set background color
 *    - CSI 48 ; 2 ; R ; G ; B m    Set RGB background color
 *
 * See http://invisible-island.net/xterm/ctlseqs/ctlseqs.txt
 * for further information.
 */
#include <stdio.h>
#include <termios.h>
#include "buffer.h"

#define ui_term_backend_init ui_vt100_init
#define ui_term_backend_blit ui_vt100_blit
#define ui_term_backend_clear ui_vt100_clear
#define ui_term_backend_colors ui_vt100_colors
#define ui_term_backend_resize ui_vt100_resize
#define ui_term_backend_save ui_vt100_save
#define ui_term_backend_restore ui_vt100_restore
#define ui_term_backend_suspend ui_vt100_suspend
#define ui_term_backend_resume ui_vt100_resume
#define ui_term_backend_new ui_vt100_new
#define ui_term_backend_free ui_vt100_free

#define CELL_COLOR_BLACK   { .index = 0 }
#define CELL_COLOR_RED     { .index = 1 }
#define CELL_COLOR_GREEN   { .index = 2 }
#define CELL_COLOR_YELLOW  { .index = 3 }
#define CELL_COLOR_BLUE    { .index = 4 }
#define CELL_COLOR_MAGENTA { .index = 5 }
#define CELL_COLOR_CYAN    { .index = 6 }
#define CELL_COLOR_WHITE   { .index = 7 }
#define CELL_COLOR_DEFAULT { .index = 9 }

#define CELL_ATTR_NORMAL    0
#define CELL_ATTR_UNDERLINE (1 << 0)
#define CELL_ATTR_REVERSE   (1 << 1)
#define CELL_ATTR_BLINK     (1 << 2)
#define CELL_ATTR_BOLD      (1 << 3)
#define CELL_ATTR_ITALIC    (1 << 4)
#define CELL_ATTR_DIM       (1 << 5)

typedef struct {
	UiTerm uiterm;
	String buf;
	FILE *fp;
	char *term;
	struct termios termios;
} UiVt100;
	
static CellColor color_rgb(UiTerm *ui, uint8_t r, uint8_t g, uint8_t b) {
	return (CellColor){ .r = r, .g = g, .b = b, .index = (uint8_t)-1 };
}

static CellColor color_terminal(UiTerm *ui, uint8_t index) {
	return (CellColor){ .r = 0, .g = 0, .b = 0, .index = index };
}


static void output(FILE *fp, const char *data, size_t len) {
	fwrite(data, len, 1, fp);
	fflush(fp);
}

static void output_literal(FILE *fp, const char *data) {
	output(fp, data, strlen(data));
}

static void screen_alternate(FILE *fp, bool alternate) {
	output_literal(fp, alternate ? "\x1b[?1049h" : "\x1b[0m" "\x1b[?1049l" "\x1b[0m" );
}

static void cursor_visible(FILE *fp, bool visible) {
	output_literal(fp, visible ? "\x1b[?25h" : "\x1b[?25l");
}

static void ui_vt100_blit(UiTerm *tui) {
	UiVt100 *vtui = (UiVt100*)tui;
	String *buf = &vtui->buf;
	string_clear(buf);
	CellAttr attr = CELL_ATTR_NORMAL;
	CellColor fg = CELL_COLOR_DEFAULT, bg = CELL_COLOR_DEFAULT;
	int w = tui->width, h = tui->height;
	Cell *cell = tui->cells;
	/* reposition cursor, erase screen, reset attributes */
	string_append0(buf, "\x1b[H" "\x1b[J" "\x1b[0m");
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			CellStyle *style = &cell->style;
			if (style->attr != attr) {

				static const struct {
					CellAttr attr;
					char on[4], off[4];
				} cell_attrs[] = {
					{ CELL_ATTR_BOLD, "1", "22" },
					{ CELL_ATTR_DIM, "2", "22" },
					{ CELL_ATTR_ITALIC, "3", "23" },
					{ CELL_ATTR_UNDERLINE, "4", "24" },
					{ CELL_ATTR_BLINK, "5", "25" },
					{ CELL_ATTR_REVERSE, "7", "27" },
				};

				for (size_t i = 0; i < LENGTH(cell_attrs); i++) {
					CellAttr a = cell_attrs[i].attr;
					if ((style->attr & a) == (attr & a))
						continue;
					string_appendf(buf, "\x1b[%sm",
					               style->attr & a ? 
					               cell_attrs[i].on :
					               cell_attrs[i].off);
				}

				attr = style->attr;
			}

			if (!cell_color_equal(fg, style->fg)) {
				fg = style->fg;
				if (fg.index != (uint8_t)-1) {
					string_appendf(buf, "\x1b[%dm", 30 + fg.index);
				} else {
					string_appendf(buf, "\x1b[38;2;%d;%d;%dm",
					               fg.r, fg.g, fg.b);
				}
			}

			if (!cell_color_equal(bg, style->bg)) {
				bg = style->bg;
				if (bg.index != (uint8_t)-1) {
					string_appendf(buf, "\x1b[%dm", 40 + bg.index);
				} else {
					string_appendf(buf, "\x1b[48;2;%d;%d;%dm",
					               bg.r, bg.g, bg.b);
				}
			}

			string_append0(buf, cell->data);
			cell++;
		}
	}
	output(vtui->fp, string_content(buf), string_length0(buf));
}

static void ui_vt100_clear(UiTerm *tui) { }

static bool ui_vt100_resize(UiTerm *tui, int width, int height) {
	return true;
}

static void ui_vt100_save(UiTerm *tui) {
	UiVt100 *vtui = (UiVt100*)tui;
	cursor_visible(vtui->fp, true);
}

static void ui_vt100_restore(UiTerm *tui) {
	UiVt100 *vtui = (UiVt100*)tui;
	cursor_visible(vtui->fp, false);
}

static int ui_vt100_colors(Ui *ui) {
	UiVt100 *vtui = (UiVt100*)ui;
	return (vtui->term && strstr(vtui->term, "-256color")) ? 256 : 16;
}

static void ui_vt100_suspend(UiTerm *tui) {
	UiVt100 *vtui = (UiVt100*)tui;
	cursor_visible(vtui->fp, true);
	screen_alternate(vtui->fp, false);
	tcsetattr(fileno(vtui->fp), TCSANOW, &vtui->termios);
}

static void ui_vt100_resume(UiTerm *tui) {
	UiVt100 *vtui = (UiVt100*)tui;
	struct termios display = vtui->termios;
	display.c_iflag &= ~(IXON|INLCR|ICRNL);
	display.c_lflag &= ~(ICANON|ECHO);
	tcsetattr(fileno(vtui->fp), TCSANOW, &display);
	screen_alternate(vtui->fp, true);
	cursor_visible(vtui->fp, false);
}

static bool ui_vt100_init(UiTerm *tui, char* term, FILE *fp) {
	UiVt100 *vtui = (UiVt100*)tui;
	if (fp == NULL) {
		return false;
	}
	vtui->fp = fp;
	vtui->term = term;
	if (tcgetattr(fileno(fp), &vtui->termios)) {
		// return false;
		// ^ makes the lua tests fail
		// we probably shouldn't be init'ing the UI
		// unless stdin isatty or argv/^-$/
	}
	ui_vt100_resume(tui);
	return true;
}

static UiTerm *ui_vt100_new(void) {
	UiVt100 *vtui = calloc(1, sizeof *vtui);
	if (!vtui)
		return NULL;
	string_init(&vtui->buf);
	return (UiTerm*)vtui;
}

static void ui_vt100_free(UiTerm *tui) {
	UiVt100 *vtui = (UiVt100*)tui;
	ui_vt100_suspend(tui);
	string_release(&vtui->buf);
}
