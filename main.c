#include <signal.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "ui-terminal.h"
#include "vis-lua.h"
#include "text-util.h"
#include "text-motions.h"
#include "text-objects.h"
#include "util.h"
#include "libutf.h"
#include "libkey.h"
#include "array.h"
#include "string.h"

#define PAGE      INT_MAX
#define PAGE_HALF (INT_MAX-1)

/** functions to be called from keybindings */
/* ignore key, do nothing */
static const char *nop(Vis*, const char *keys, const Arg *arg);
/* record/replay macro indicated by keys */
static const char *macro_record(Vis*, const char *keys, const Arg *arg);
static const char *macro_replay(Vis*, const char *keys, const Arg *arg);
/* temporarily suspend the editor and return to the shell, type 'fg' to get back */
static const char *suspend(Vis*, const char *keys, const Arg *arg);
/* reset count if set, otherwise remove all but the primary selection */
static const char *normalmode_escape(Vis*, const char *keys, const Arg *arg);
/* reset count if set, otherwise switch to normal mode */
static const char *visualmode_escape(Vis*, const char *keys, const Arg *arg);
/* switch to mode indicated by arg->i */
static const char *switchmode(Vis*, const char *keys, const Arg *arg);
/* switch to insert mode after performing movement indicated by arg->i */
static const char *insertmode(Vis*, const char *keys, const Arg *arg);
/* switch to replace mode after performing movement indicated by arg->i */
static const char *replacemode(Vis*, const char *keys, const Arg *arg);
/* add a new line either before or after the one where the cursor currently is */
static const char *openline(Vis*, const char *keys, const Arg *arg);
/* join lines from current cursor position to movement indicated by arg */
static const char *join(Vis*, const char *keys, const Arg *arg);
/* perform last action i.e. action_prev again */
static const char *repeat(Vis*, const char *keys, const Arg *arg);
/* replace character at cursor with one from keys */
static const char *replace(Vis*, const char *keys, const Arg *arg);
/* create a new cursor on the previous (arg->i < 0) or next (arg->i > 0) line */
static const char *selections_new(Vis*, const char *keys, const Arg *arg);
/* try to align all selections on the same column */
static const char *selections_align(Vis*, const char *keys, const Arg *arg);
/* try to align all selections by inserting the correct amount of white spaces */
static const char *selections_align_indent(Vis*, const char *keys, const Arg *arg);
/* remove all but the primary cursor and their selections */
static const char *selections_clear(Vis*, const char *keys, const Arg *arg);
/* remove the least recently added selection */
static const char *selections_remove(Vis*, const char *keys, const Arg *arg);
/* remove count (or arg->i)-th selection column */
static const char *selections_remove_column(Vis*, const char *keys, const Arg *arg);
/* remove all but the count (or arg->i)-th selection column */
static const char *selections_remove_column_except(Vis*, const char *keys, const Arg *arg);
/* move to the previous (arg->i < 0) or next (arg->i > 0) selection */
static const char *selections_navigate(Vis*, const char *keys, const Arg *arg);
/* select the next region matching the current selection */
static const char *selections_match_next(Vis*, const char *keys, const Arg *arg);
/* clear current selection but select next match */
static const char *selections_match_skip(Vis*, const char *keys, const Arg *arg);
/* rotate selection content count times left (arg->i < 0) or right (arg->i > 0) */
static const char *selections_rotate(Vis*, const char *keys, const Arg *arg);
/* remove leading and trailing white spaces from selections */
static const char *selections_trim(Vis*, const char *keys, const Arg *arg);
/* save active selections to mark */
static const char *selections_save(Vis*, const char *keys, const Arg *arg);
/* restore selections from mark */
static const char *selections_restore(Vis*, const char *keys, const Arg *arg);
/* union selections from mark */
static const char *selections_union(Vis*, const char *keys, const Arg *arg);
/* intersect selections from mark */
static const char *selections_intersect(Vis*, const char *keys, const Arg *arg);
/* perform complement of current active selections */
static const char *selections_complement(Vis*, const char *keys, const Arg *arg);
/* subtract selections from mark */
static const char *selections_minus(Vis*, const char *keys, const Arg *arg);
/* adjust current used count according to keys */
static const char *count(Vis*, const char *keys, const Arg *arg);
/* move to the count-th line or if not given either to the first (arg->i < 0)
 *  or last (arg->i > 0) line of file */
static const char *gotoline(Vis*, const char *keys, const Arg *arg);
/* make the current action use the operator indicated by arg->i */
static const char *operator(Vis*, const char *keys, const Arg *arg);
/* blocks to read a key and performs movement indicated by arg->i which
 * should be one of VIS_MOVE_{TO,TILL}_{,LINE}_{RIGHT,LEFT}*/
static const char *movement_key(Vis*, const char *keys, const Arg *arg);
/* perform the movement as indicated by arg->i */
static const char *movement(Vis*, const char *keys, const Arg *arg);
/* let the current operator affect the range indicated by the text object arg->i */
static const char *textobj(Vis*, const char *keys, const Arg *arg);
/* move to the other end of selected text */
static const char *selection_end(Vis*, const char *keys, const Arg *arg);
/* use register indicated by keys for the current operator */
static const char *reg(Vis*, const char *keys, const Arg *arg);
/* use mark indicated by keys for the current action */
static const char *mark(Vis*, const char *keys, const Arg *arg);
/* {un,re}do last action, redraw window */
static const char *undo(Vis*, const char *keys, const Arg *arg);
static const char *redo(Vis*, const char *keys, const Arg *arg);
/* earlier, later action chronologically, redraw window */
static const char *earlier(Vis*, const char *keys, const Arg *arg);
static const char *later(Vis*, const char *keys, const Arg *arg);
/* delete from the current cursor position to the end of
 * movement as indicated by arg->i */
static const char *delete(Vis*, const char *keys, const Arg *arg);
/* insert register content indicated by keys at current cursor position */
static const char *insert_register(Vis*, const char *keys, const Arg *arg);
/* show a user prompt to get input with title arg->s */
static const char *prompt_show(Vis*, const char *keys, const Arg *arg);
/* blocks to read 3 consecutive digits and inserts the corresponding byte value */
static const char *insert_verbatim(Vis*, const char *keys, const Arg *arg);
/* scroll window content according to arg->i which can be either PAGE, PAGE_HALF,
 * or an arbitrary number of lines. a multiplier overrides what is given in arg->i.
 * negative values scroll back, positive forward. */
static const char *wscroll(Vis*, const char *keys, const Arg *arg);
/* similar to scroll, but do only move window content not cursor position */
static const char *wslide(Vis*, const char *keys, const Arg *arg);
/* call editor function as indicated by arg->f */
static const char *call(Vis*, const char *keys, const Arg *arg);
/* call window function as indicated by arg->w */
static const char *window(Vis*, const char *keys, const Arg *arg);
/* show info about Unicode character at cursor position */
static const char *unicode_info(Vis*, const char *keys, const Arg *arg);
/* either go to count % of file or to matching item */
static const char *percent(Vis*, const char *keys, const Arg *arg);
/* navigate jumplist next (arg->i > 0), prev (arg->i < 0), save (arg->i = 0) */
static const char *jumplist(Vis*, const char *keys, const Arg *arg);

#include "config.h"

/** key bindings functions */

static const char *nop(Vis *vis, const char *keys, const Arg *arg) {
	return keys;
}

static const char *macro_record(Vis *vis, const char *keys, const Arg *arg) {
	if (!vis_macro_record_stop(vis)) {
		if (!keys[0])
			return NULL;
		const char *next = vis_keys_next(keys);
		if (next - keys > 1)
			return next;
		enum VisRegister reg = vis_register_from(vis, keys[0]);
		vis_macro_record(vis, reg);
		keys++;
	}
	vis_draw(vis);
	return keys;
}

static const char *macro_replay(Vis *vis, const char *keys, const Arg *arg) {
	if (!keys[0])
		return NULL;
	const char *next = vis_keys_next(keys);
	if (next - keys > 1)
		return next;
	enum VisRegister reg = vis_register_from(vis, keys[0]);
	vis_macro_replay(vis, reg);
	return keys+1;
}

static const char *suspend(Vis *vis, const char *keys, const Arg *arg) {
	vis_suspend(vis);
	return keys;
}

static const char *repeat(Vis *vis, const char *keys, const Arg *arg) {
	vis_repeat(vis);
	return keys;
}

static const char *selections_new(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	bool anchored = view_selections_anchored(view_selections_primary_get(view));
	VisCountIterator it = vis_count_iterator_get(vis, 1);
	while (vis_count_iterator_next(&it)) {
		Selection *sel = NULL;
		switch (arg->i) {
		case -1:
		case +1:
			sel = view_selections_primary_get(view);
			break;
		case INT_MIN:
			sel = view_selections(view);
			break;
		case INT_MAX:
			for (Selection *s = view_selections(view); s; s = view_selections_next(s))
				sel = s;
			break;
		}

		if (!sel)
			return keys;

		size_t oldpos = view_cursors_pos(sel);
		if (arg->i > 0)
			view_line_down(sel);
		else if (arg->i < 0)
			view_line_up(sel);
		size_t newpos = view_cursors_pos(sel);
		view_cursors_to(sel, oldpos);
		Selection *sel_new = view_selections_new(view, newpos);
		if (!sel_new) {
			if (arg->i == -1)
				sel_new = view_selections_prev(sel);
			else if (arg->i == +1)
				sel_new = view_selections_next(sel);
		}
		if (sel_new) {
			view_selections_primary_set(sel_new);
			view_selections_anchor(sel_new, anchored);
		}
	}
	vis_count_set(vis, VIS_COUNT_UNKNOWN);
	return keys;
}

static const char *selections_align(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	Text *txt = vis_text(vis);
	int mincol = INT_MAX;
	for (Selection *s = view_selections(view); s; s = view_selections_next(s)) {
		int col = view_cursors_cell_get(s);
		if (col >= 0 && col < mincol)
			mincol = col;
	}
	for (Selection *s = view_selections(view); s; s = view_selections_next(s)) {
		if (view_cursors_cell_set(s, mincol) == -1) {
			size_t pos = view_cursors_pos(s);
			size_t col = text_line_width_set(txt, pos, mincol);
			view_cursors_to(s, col);
		}
	}
	return keys;
}

static const char *selections_align_indent(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	Text *txt = vis_text(vis);
	bool left_align = arg->i < 0;
	int columns = view_selections_column_count(view);

	for (int i = 0; i < columns; i++) {
		int mincol = INT_MAX, maxcol = 0;
		for (Selection *s = view_selections_column(view, i); s; s = view_selections_column_next(s, i)) {
			Filerange sel = view_selections_get(s);
			size_t pos = left_align ? sel.start : sel.end;
			int col = text_line_width_get(txt, pos);
			if (col < mincol)
				mincol = col;
			if (col > maxcol)
				maxcol = col;
		}

		size_t len = maxcol - mincol;
		char *buf = malloc(len+1);
		if (!buf)
			return keys;
		memset(buf, ' ', len);

		for (Selection *s = view_selections_column(view, i); s; s = view_selections_column_next(s, i)) {
			Filerange sel = view_selections_get(s);
			size_t pos = left_align ? sel.start : sel.end;
			size_t ipos = sel.start;
			int col = text_line_width_get(txt, pos);
			if (col < maxcol) {
				size_t off = maxcol - col;
				if (off <= len)
					text_insert(txt, ipos, buf, off);
			}
		}

		free(buf);
	}

	view_draw(view);
	return keys;
}

static const char *selections_clear(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	if (view_selections_count(view) > 1)
		view_selections_dispose_all(view);
	else
		view_selection_clear(view_selections_primary_get(view));
	return keys;
}

static const Selection *selection_new_primary(View *view, Filerange *r) {
	Text *txt = view_text(view);
	size_t pos = text_char_prev(txt, r->end);
	Selection *s = view_selections_new(view, pos);
	if (!s)
		return NULL;
	view_selections_set(s, r);
	view_selections_anchor(s, true);
	view_selections_primary_set(s);
	return s;
}

static const char *selections_match_next_literal(Vis *vis, const char *keys, const Arg *arg) {
	Text *txt = vis_text(vis);
	View *view = vis_view(vis);
	Selection *s = view_selections_primary_get(view);
	Filerange sel = view_selections_get(s);
	size_t len = text_range_size(&sel);
	if (!len)
		return keys;

	char *buf = text_bytes_alloc0(txt, sel.start, len);
	if (!buf)
		return keys;

	size_t start = text_find_next(txt, sel.end, buf);
	Filerange match = text_range_new(start, start+len);
	if (start != sel.end && selection_new_primary(view, &match))
		goto out;

	sel = view_selections_get(view_selections(view));
	start = text_find_prev(txt, sel.start, buf);
	if (start == sel.start)
		goto out;

	match = text_range_new(start, start+len);
	selection_new_primary(view, &match);

out:
	free(buf);
	return keys;
}

static const char *selections_match_next(Vis *vis, const char *keys, const Arg *arg) {
	Text *txt = vis_text(vis);
	View *view = vis_view(vis);
	Selection *s = view_selections_primary_get(view);
	Filerange sel = view_selections_get(s);
	if (!text_range_valid(&sel))
		return keys;

	static bool match_word;

	if (view_selections_count(view) == 1) {
		Filerange word = text_object_word(txt, view_cursors_pos(s));
		match_word = text_range_equal(&sel, &word);
	}

	if (!match_word)
		return selections_match_next_literal(vis, keys, arg);

	char *buf = text_bytes_alloc0(txt, sel.start, text_range_size(&sel));
	if (!buf)
		return keys;

	Filerange word = text_object_word_find_next(txt, sel.end, buf);
	if (text_range_valid(&word) && selection_new_primary(view, &word))
		goto out;

	sel = view_selections_get(view_selections(view));
	word = text_object_word_find_prev(txt, sel.start, buf);
	if (!text_range_valid(&word))
		goto out;
	selection_new_primary(view, &word);

out:
	free(buf);
	return keys;
}

static const char *selections_match_skip(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	Selection *sel = view_selections_primary_get(view);
	keys = selections_match_next(vis, keys, arg);
	if (sel != view_selections_primary_get(view))
		view_selections_dispose(sel);
	return keys;
}

static const char *selections_remove(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	view_selections_dispose(view_selections_primary_get(view));
	view_cursor_to(view, view_cursor_get(view));
	return keys;
}

static const char *selections_remove_column(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	int max = view_selections_column_count(view);
	int column = vis_count_get_default(vis, arg->i) - 1;
	if (column >= max)
		column = max - 1;
	if (view_selections_count(view) == 1) {
		vis_keys_feed(vis, "<Escape>");
		return keys;
	}

	for (Selection *s = view_selections_column(view, column), *next; s; s = next) {
		next = view_selections_column_next(s, column);
		view_selections_dispose(s);
	}

	vis_count_set(vis, VIS_COUNT_UNKNOWN);
	return keys;
}

static const char *selections_remove_column_except(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	int max = view_selections_column_count(view);
	int column = vis_count_get_default(vis, arg->i) - 1;
	if (column >= max)
		column = max - 1;
	if (view_selections_count(view) == 1) {
		vis_redraw(vis);
		return keys;
	}

	Selection *sel = view_selections(view);
	Selection *col = view_selections_column(view, column);
	for (Selection *next; sel; sel = next) {
		next = view_selections_next(sel);
		if (sel == col)
			col = view_selections_column_next(col, column);
		else
			view_selections_dispose(sel);
	}

	vis_count_set(vis, VIS_COUNT_UNKNOWN);
	return keys;
}

static const char *selections_navigate(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	if (view_selections_count(view) == 1)
		return wscroll(vis, keys, arg);
	Selection *s = view_selections_primary_get(view);
	VisCountIterator it = vis_count_iterator_get(vis, 1);
	while (vis_count_iterator_next(&it)) {
		if (arg->i > 0) {
			s = view_selections_next(s);
			if (!s)
				s = view_selections(view);
		} else {
			s = view_selections_prev(s);
			if (!s) {
				s = view_selections(view);
				for (Selection *n = s; n; n = view_selections_next(n))
					s = n;
			}
		}
	}
	view_selections_primary_set(s);
	vis_count_set(vis, VIS_COUNT_UNKNOWN);
	return keys;
}

static const char *selections_rotate(Vis *vis, const char *keys, const Arg *arg) {

	typedef struct {
		Selection *sel;
		char *data;
		size_t len;
	} Rotate;

	Array arr;
	Text *txt = vis_text(vis);
	View *view = vis_view(vis);
	int columns = view_selections_column_count(view);
	int selections = columns == 1 ? view_selections_count(view) : columns;
	int count = vis_count_get_default(vis, 1);
	array_init_sized(&arr, sizeof(Rotate));
	if (!array_reserve(&arr, selections))
		return keys;
	size_t line = 0;

	for (Selection *s = view_selections(view), *next; s; s = next) {
		next = view_selections_next(s);
		size_t line_next = 0;

		Filerange sel = view_selections_get(s);
		Rotate rot;
		rot.sel = s;
		rot.len = text_range_size(&sel);
		if ((rot.data = malloc(rot.len)))
			rot.len = text_bytes_get(txt, sel.start, rot.len, rot.data);
		else
			rot.len = 0;
		array_add(&arr, &rot);

		if (!line)
			line = text_lineno_by_pos(txt, view_cursors_pos(s));
		if (next)
			line_next = text_lineno_by_pos(txt, view_cursors_pos(next));
		if (!next || (columns > 1 && line != line_next)) {
			size_t len = array_length(&arr);
			size_t off = arg->i > 0 ? count % len : len - (count % len);
			for (size_t i = 0; i < len; i++) {
				size_t j = (i + off) % len;
				Rotate *oldrot = array_get(&arr, i);
				Rotate *newrot = array_get(&arr, j);
				if (!oldrot || !newrot || oldrot == newrot)
					continue;
				Filerange newsel = view_selections_get(newrot->sel);
				if (!text_range_valid(&newsel))
					continue;
				if (!text_delete_range(txt, &newsel))
					continue;
				if (!text_insert(txt, newsel.start, oldrot->data, oldrot->len))
					continue;
				newsel.end = newsel.start + oldrot->len;
				view_selections_set(newrot->sel, &newsel);
				free(oldrot->data);
			}
			array_clear(&arr);
		}
		line = line_next;
	}

	array_release(&arr);
	vis_count_set(vis, VIS_COUNT_UNKNOWN);
	return keys;
}

static const char *selections_trim(Vis *vis, const char *keys, const Arg *arg) {
	Text *txt = vis_text(vis);
	View *view = vis_view(vis);
	for (Selection *s = view_selections(view), *next; s; s = next) {
		next = view_selections_next(s);
		Filerange sel = view_selections_get(s);
		if (!text_range_valid(&sel))
			continue;
		for (char b; sel.start < sel.end && text_byte_get(txt, sel.end-1, &b)
			&& isspace((unsigned char)b); sel.end--);
		for (char b; sel.start <= sel.end && text_byte_get(txt, sel.start, &b)
			&& isspace((unsigned char)b); sel.start++);
		if (sel.start < sel.end) {
			view_selections_set(s, &sel);
		} else if (!view_selections_dispose(s)) {
			vis_mode_switch(vis, VIS_MODE_NORMAL);
		}
	}
	return keys;
}

static void selections_set(Vis *vis, View *view, Array *sel) {
	enum VisMode mode = vis_mode_get(vis);
	bool anchored = mode == VIS_MODE_VISUAL || mode == VIS_MODE_VISUAL_LINE;
	view_selections_set_all(view, sel, anchored);
	if (!anchored)
		view_selections_clear_all(view);
}

static const char *selections_save(Vis *vis, const char *keys, const Arg *arg) {
	Win *win = vis_window(vis);
	View *view = vis_view(vis);
	enum VisMark mark = vis_mark_used(vis);
	Array sel = view_selections_get_all(view);
	vis_mark_set(win, mark, &sel);
	array_release(&sel);
	vis_cancel(vis);
	return keys;
}

static const char *selections_restore(Vis *vis, const char *keys, const Arg *arg) {
	Win *win = vis_window(vis);
	View *view = vis_view(vis);
	enum VisMark mark = vis_mark_used(vis);
	Array sel = vis_mark_get(win, mark);
	selections_set(vis, view, &sel);
	array_release(&sel);
	vis_cancel(vis);
	return keys;
}

static const char *selections_union(Vis *vis, const char *keys, const Arg *arg) {
	Win *win = vis_window(vis);
	View *view = vis_view(vis);
	enum VisMark mark = vis_mark_used(vis);
	Array a = vis_mark_get(win, mark);
	Array b = view_selections_get_all(view);
	Array sel;
	array_init_from(&sel, &a);

	size_t i = 0, j = 0;
	Filerange *r1 = array_get(&a, i), *r2 = array_get(&b, j), cur = text_range_empty();
	while (r1 || r2) {
		if (r1 && text_range_overlap(r1, &cur)) {
			cur = text_range_union(r1, &cur);
			r1 = array_get(&a, ++i);
		} else if (r2 && text_range_overlap(r2, &cur)) {
			cur = text_range_union(r2, &cur);
			r2 = array_get(&b, ++j);
		} else {
			if (text_range_valid(&cur))
				array_add(&sel, &cur);
			if (!r1) {
				cur = *r2;
				r2 = array_get(&b, ++j);
			} else if (!r2) {
				cur = *r1;
				r1 = array_get(&a, ++i);
			} else {
				if (r1->start < r2->start) {
					cur = *r1;
					r1 = array_get(&a, ++i);
				} else {
					cur = *r2;
					r2 = array_get(&b, ++j);
				}
			}
		}
	}

	if (text_range_valid(&cur))
		array_add(&sel, &cur);

	selections_set(vis, view, &sel);
	vis_cancel(vis);

	array_release(&a);
	array_release(&b);
	array_release(&sel);

	return keys;
}

static void intersect(Array *ret, Array *a, Array *b) {
	size_t i = 0, j = 0;
	Filerange *r1 = array_get(a, i), *r2 = array_get(b, j);
	while (r1 && r2) {
		if (text_range_overlap(r1, r2)) {
			Filerange new = text_range_intersect(r1, r2);
			array_add(ret, &new);
		}
		if (r1->end < r2->end)
			r1 = array_get(a, ++i);
		else
			r2 = array_get(b, ++j);
	}
}

static const char *selections_intersect(Vis *vis, const char *keys, const Arg *arg) {
	Win *win = vis_window(vis);
	View *view = vis_view(vis);
	enum VisMark mark = vis_mark_used(vis);
	Array a = vis_mark_get(win, mark);
	Array b = view_selections_get_all(view);
	Array sel;
	array_init_from(&sel, &a);

	intersect(&sel, &a, &b);
	selections_set(vis, view, &sel);
	vis_cancel(vis);

	array_release(&a);
	array_release(&b);
	array_release(&sel);

	return keys;
}

static void complement(Array *ret, Array *a, Filerange *universe) {
	size_t pos = universe->start;
	for (size_t i = 0, len = array_length(a); i < len; i++) {
		Filerange *r = array_get(a, i);
		if (pos < r->start) {
			Filerange new = text_range_new(pos, r->start);
			array_add(ret, &new);
		}
		pos = r->end;
	}
	if (pos < universe->end) {
		Filerange new = text_range_new(pos, universe->end);
		array_add(ret, &new);
	}
}

static const char *selections_complement(Vis *vis, const char *keys, const Arg *arg) {
	Text *txt = vis_text(vis);
	View *view = vis_view(vis);
	Filerange universe = text_object_entire(txt, 0);
	Array a = view_selections_get_all(view);
	Array sel;
	array_init_from(&sel, &a);

	complement(&sel, &a, &universe);

	selections_set(vis, view, &sel);
	array_release(&a);
	array_release(&sel);
	return keys;
}

static const char *selections_minus(Vis *vis, const char *keys, const Arg *arg) {
	Text *txt = vis_text(vis);
	Win *win = vis_window(vis);
	View *view = vis_view(vis);
	enum VisMark mark = vis_mark_used(vis);
	Array a = view_selections_get_all(view);
	Array b = vis_mark_get(win, mark);
	Array sel;
	array_init_from(&sel, &a);
	Array b_complement;
	array_init_from(&b_complement, &b);

	Filerange universe = text_object_entire(txt, 0);
	complement(&b_complement, &b, &universe);
	intersect(&sel, &a, &b_complement);

	selections_set(vis, view, &sel);
	vis_cancel(vis);

	array_release(&a);
	array_release(&b);
	array_release(&b_complement);
	array_release(&sel);

	return keys;
}

static const char *replace(Vis *vis, const char *keys, const Arg *arg) {
	if (!keys[0]) {
		return NULL;
	}

	const char *next = vis_keys_next(keys);
	if (!next)
		return NULL;
	char utf8[UTFmax+1];

	if (keys[0] == '\n' || keys[0] == '\r' || strcmp(keys, "<Enter>") == 0) {
		utf8[0] = '\n';
		utf8[1] = '\0';
	} else if (strcmp(keys, "<Escape>") == 0) {
		return next;
	} else if (vis_keys_utf8(vis, keys, utf8) <= 0) {
		return next;
	}

	vis_operator(vis, VIS_OP_REPLACE, utf8);
	if (vis_mode_get(vis) == VIS_MODE_OPERATOR_PENDING)
		vis_motion(vis, VIS_MOVE_CHAR_NEXT);
	return next;
}

static const char *count(Vis *vis, const char *keys, const Arg *arg) {
	int digit = arg->i;
	int count = vis_count_get_default(vis, 0);
	if (digit == 0 && count == 0)
		vis_motion(vis, VIS_MOVE_LINE_BEGIN);
	else
		vis_count_set(vis, count * 10 + digit);
	return keys;
}

static const char *gotoline(Vis *vis, const char *keys, const Arg *arg) {
	if (vis_count_get(vis) != VIS_COUNT_UNKNOWN)
		vis_motion(vis, VIS_MOVE_LINE);
	else if (arg->i < 0)
		vis_motion(vis, VIS_MOVE_FILE_BEGIN);
	else
		vis_motion(vis, VIS_MOVE_FILE_END);
	return keys;
}

static const char *operator(Vis *vis, const char *keys, const Arg *arg) {
	vis_operator(vis, arg->i);
	return keys;
}

static const char *movement_key(Vis *vis, const char *keys, const Arg *arg) {
	if (!keys[0]) {
		return NULL;
	}

	const char *next = vis_keys_next(keys);
	if (!next)
		return NULL;

	char utf8[UTFmax+1];
	if (strcmp(keys, "<Enter>") == 0) {
		utf8[0] = '\n';
		utf8[1] = '\0';
		vis_motion(vis, arg->i, utf8);
	} else if (vis_keys_utf8(vis, keys, utf8) > 0) {
		vis_motion(vis, arg->i, utf8);
	}


	return next;
}

static const char *movement(Vis *vis, const char *keys, const Arg *arg) {
	vis_motion(vis, arg->i);
	return keys;
}

static const char *textobj(Vis *vis, const char *keys, const Arg *arg) {
	vis_textobject(vis, arg->i);
	return keys;
}

static const char *selection_end(Vis *vis, const char *keys, const Arg *arg) {
	for (Selection *s = view_selections(vis_view(vis)); s; s = view_selections_next(s))
		view_selections_flip(s);
	return keys;
}

static const char *reg(Vis *vis, const char *keys, const Arg *arg) {
	if (!keys[0])
		return NULL;
	const char *next = vis_keys_next(keys);
	if (next - keys > 1)
		return next;
	enum VisRegister reg = vis_register_from(vis, keys[0]);
	vis_register(vis, reg);
	return keys+1;
}

static const char *mark(Vis *vis, const char *keys, const Arg *arg) {
	if (!keys[0])
		return NULL;
	const char *next = vis_keys_next(keys);
	if (next - keys > 1)
		return next;
	enum VisMark mark = vis_mark_from(vis, keys[0]);
	vis_mark(vis, mark);
	return keys+1;
}

static const char *undo(Vis *vis, const char *keys, const Arg *arg) {
	size_t pos = text_undo(vis_text(vis));
	if (pos != EPOS) {
		View *view = vis_view(vis);
		if (view_selections_count(view) == 1)
			view_cursor_to(view, pos);
		/* redraw all windows in case some display the same file */
		vis_draw(vis);
	}
	return keys;
}

static const char *redo(Vis *vis, const char *keys, const Arg *arg) {
	size_t pos = text_redo(vis_text(vis));
	if (pos != EPOS) {
		View *view = vis_view(vis);
		if (view_selections_count(view) == 1)
			view_cursor_to(view, pos);
		/* redraw all windows in case some display the same file */
		vis_draw(vis);
	}
	return keys;
}

static const char *earlier(Vis *vis, const char *keys, const Arg *arg) {
	size_t pos = EPOS;
	VisCountIterator it = vis_count_iterator_get(vis, 1);
	while (vis_count_iterator_next(&it))
		pos = text_earlier(vis_text(vis));
	if (pos != EPOS) {
		view_cursor_to(vis_view(vis), pos);
		/* redraw all windows in case some display the same file */
		vis_draw(vis);
	}
	return keys;
}

static const char *later(Vis *vis, const char *keys, const Arg *arg) {
	size_t pos = EPOS;
	VisCountIterator it = vis_count_iterator_get(vis, 1);
	while (vis_count_iterator_next(&it))
		pos = text_later(vis_text(vis));
	if (pos != EPOS) {
		view_cursor_to(vis_view(vis), pos);
		/* redraw all windows in case some display the same file */
		vis_draw(vis);
	}
	return keys;
}

static const char *delete(Vis *vis, const char *keys, const Arg *arg) {
	vis_operator(vis, VIS_OP_DELETE);
	vis_motion(vis, arg->i);
	return keys;
}

static const char *insert_register(Vis *vis, const char *keys, const Arg *arg) {
	if (!keys[0])
		return NULL;
	const char *next = vis_keys_next(keys);
	if (next - keys > 1)
		return next;
	enum VisRegister reg = vis_register_from(vis, keys[0]);
	if (reg != VIS_REG_INVALID) {
		vis_register(vis, reg);
		vis_operator(vis, VIS_OP_PUT_BEFORE_END);
	}
	return keys+1;
}

static const char *prompt_show(Vis *vis, const char *keys, const Arg *arg) {
	vis_prompt_show(vis, arg->s);
	return keys;
}

static const char *insert_verbatim(Vis *vis, const char *keys, const Arg *arg) {
	Rune rune = 0;
	char buf[512], type = keys[0];
	const char *data = NULL;
	int len = 0, count = 0, base = 0;
	switch (type) {
	case '\0':
		return NULL;
	case 'o':
	case 'O':
		count = 3;
		base = 8;
		break;
	case 'U':
		count = 4;
		/* fall through */
	case 'u':
		count += 4;
		base = 16;
		break;
	case 'x':
	case 'X':
		count = 2;
		base = 16;
		break;
	default:
		if ('0' <= type && type <= '9') {
			rune = type - '0';
			count = 2;
			base = 10;
		}
		break;
	}

	if (base) {
		for (keys++; keys[0] && count > 0; keys++, count--) {
			int v = 0;
			if (base == 8 && '0' <= keys[0] && keys[0] <= '7') {
				v = keys[0] - '0';
			} else if ((base == 10 || base == 16) && '0' <= keys[0] && keys[0] <= '9') {
				v = keys[0] - '0';
			} else if (base == 16 && 'a' <= keys[0] && keys[0] <= 'f') {
				v = 10 + keys[0] - 'a';
			} else if (base == 16 && 'A' <= keys[0] && keys[0] <= 'F') {
				v = 10 + keys[0] - 'A';
			} else {
				count = 0;
				break;
			}
			rune = rune * base + v;
		}

		if (count > 0)
			return NULL;
		if (type == 'u' || type == 'U') {
			len = runetochar(buf, &rune);
		} else {
			buf[0] = rune;
			len = 1;
		}

		data = buf;
	} else {
		const char *next = vis_keys_next(keys);
		if (!next)
			return NULL;
		if ((len = vis_keys_utf8(vis, keys, buf)) > 0) {
			if (buf[0] == '\n') buf[0] = '\r';
			data = buf;
		} else {
			vis_info_show(vis, "Unknown key");
		}
		keys = next;
	}

	if (len > 0)
		vis_insert_key(vis, data, len);
	return keys;
}

static const char *wscroll(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	int count = vis_count_get(vis);
	switch (arg->i) {
	case -PAGE:
		view_scroll_page_up(view);
		break;
	case +PAGE:
		view_scroll_page_down(view);
		break;
	case -PAGE_HALF:
		view_scroll_halfpage_up(view);
		break;
	case +PAGE_HALF:
		view_scroll_halfpage_down(view);
		break;
	default:
		if (count == VIS_COUNT_UNKNOWN)
			count = arg->i < 0 ? -arg->i : arg->i;
		if (arg->i < 0)
			view_scroll_up(view, count);
		else
			view_scroll_down(view, count);
		break;
	}
	vis_count_set(vis, VIS_COUNT_UNKNOWN);
	return keys;
}

static const char *wslide(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	int count = vis_count_get(vis);
	if (count == VIS_COUNT_UNKNOWN)
		count = arg->i < 0 ? -arg->i : arg->i;
	if (arg->i >= 0)
		view_slide_down(view, count);
	else
		view_slide_up(view, count);
	vis_count_set(vis, VIS_COUNT_UNKNOWN);
	return keys;
}

static const char *call(Vis *vis, const char *keys, const Arg *arg) {
	arg->f(vis);
	return keys;
}

static const char *window(Vis *vis, const char *keys, const Arg *arg) {
	arg->w(vis_view(vis));
	return keys;
}

static const char *openline(Vis *vis, const char *keys, const Arg *arg) {
	vis_operator(vis, VIS_OP_MODESWITCH, VIS_MODE_INSERT);
	if (arg->i > 0) {
		vis_motion(vis, VIS_MOVE_LINE_END);
		vis_keys_feed(vis, "<Enter>");
	} else {
		if (vis_get_autoindent(vis)) {
			vis_motion(vis, VIS_MOVE_LINE_START);
			vis_keys_feed(vis, "<vis-motion-line-start>");
		} else {
			vis_motion(vis, VIS_MOVE_LINE_BEGIN);
			vis_keys_feed(vis, "<vis-motion-line-begin>");
		}
		vis_keys_feed(vis, "<Enter><vis-motion-line-up>");
	}
	return keys;
}

static const char *join(Vis *vis, const char *keys, const Arg *arg) {
	bool normal = (vis_mode_get(vis) == VIS_MODE_NORMAL);
	vis_operator(vis, VIS_OP_JOIN, arg->s);
	if (normal) {
		int count = vis_count_get_default(vis, 0);
		if (count)
			vis_count_set(vis, count-1);
		vis_motion(vis, VIS_MOVE_LINE_NEXT);
	}
	return keys;
}

static const char *normalmode_escape(Vis *vis, const char *keys, const Arg *arg) {
	if (vis_count_get(vis) == VIS_COUNT_UNKNOWN)
		selections_clear(vis, keys, arg);
	else
		vis_count_set(vis, VIS_COUNT_UNKNOWN);
	return keys;
}

static const char *visualmode_escape(Vis *vis, const char *keys, const Arg *arg) {
	if (vis_count_get(vis) == VIS_COUNT_UNKNOWN)
		vis_mode_switch(vis, VIS_MODE_NORMAL);
	else
		vis_count_set(vis, VIS_COUNT_UNKNOWN);
	return keys;
}

static const char *switchmode(Vis *vis, const char *keys, const Arg *arg) {
	vis_mode_switch(vis, arg->i);
	return keys;
}

static const char *insertmode(Vis *vis, const char *keys, const Arg *arg) {
	vis_operator(vis, VIS_OP_MODESWITCH, VIS_MODE_INSERT);
	vis_motion(vis, arg->i);
	return keys;
}

static const char *replacemode(Vis *vis, const char *keys, const Arg *arg) {
	vis_operator(vis, VIS_OP_MODESWITCH, VIS_MODE_REPLACE);
	vis_motion(vis, arg->i);
	return keys;
}

static const char *unicode_info(Vis *vis, const char *keys, const Arg *arg) {
	View *view = vis_view(vis);
	Text *txt = vis_text(vis);
	size_t start = view_cursor_get(view);
	size_t end = text_char_next(txt, start);
	char *grapheme = text_bytes_alloc0(txt, start, end-start), *codepoint = grapheme;
	if (!grapheme)
		return keys;
	String info;
	string_init(&info);
	mbstate_t ps = { 0 };
	Iterator it = text_iterator_get(txt, start);
	for (size_t pos = start; it.pos < end; pos = it.pos) {
		if (!text_iterator_codepoint_next(&it, NULL)) {
			vis_info_show(vis, "Failed to parse code point");
			goto err;
		}
		size_t len = it.pos - pos;
		wchar_t wc = 0xFFFD;
		size_t res = mbrtowc(&wc, codepoint, len, &ps);
		bool combining = false;
		if (res != (size_t)-1 && res != (size_t)-2)
			combining = (wc != L'\0' && wcwidth(wc) == 0);
		unsigned char ch = *codepoint;
		if (ch < 128 && !isprint(ch))
			string_appendf(&info, "<^%c> ", ch == 127 ? '?' : ch + 64);
		else
			string_appendf(&info, "<%s%.*s> ", combining ? " " : "", (int)len, codepoint);
		if (arg->s[0] == 'a') { // UNICODE - a
			string_appendf(&info, "U+%04"PRIX32" ", (uint32_t)wc);
		} else { // UTF-8 - 8
			for (size_t i = 0; i < len; i++)
				string_appendf(&info, "%02x ", (uint8_t)codepoint[i]);
		}
		codepoint += len;
	}
	vis_info_show(vis, "%s", string_content0(&info));
err:
	free(grapheme);
	string_release(&info);
	return keys;
}

static const char *percent(Vis *vis, const char *keys, const Arg *arg) {
	if (vis_count_get(vis) == VIS_COUNT_UNKNOWN)
		vis_motion(vis, VIS_MOVE_BRACKET_MATCH);
	else
		vis_motion(vis, VIS_MOVE_PERCENT);
	return keys;
}

static const char *jumplist(Vis *vis, const char *keys, const Arg *arg) {
	if (arg->i < 0)
		vis_jumplist_prev(vis);
	else if (arg->i > 0)
		vis_jumplist_next(vis);
	else
		vis_jumplist_save(vis);
	return keys;
}

static Vis *vis;

static void signal_handler(int signum, siginfo_t *siginfo, void *context) {
	vis_signal_handler(vis, signum, siginfo, context);
}

int main(int argc, char *argv[]) {

	VisEvent event = {
		.init = vis_lua_init,
		.start = vis_lua_start,
		.quit = vis_lua_quit,
		.mode_insert_input = vis_lua_mode_insert_input,
		.mode_replace_input = vis_lua_mode_replace_input,
		.file_open = vis_lua_file_open,
		.file_save_pre = vis_lua_file_save_pre,
		.file_save_post = vis_lua_file_save_post,
		.file_close = vis_lua_file_close,
		.win_open = vis_lua_win_open,
		.win_close = vis_lua_win_close,
		.win_highlight = vis_lua_win_highlight,
		.win_status = vis_lua_win_status,
	};

	Ui *ui = ui_term_new();
	vis = vis_new(ui, &event);
	if (!vis)
		return EXIT_FAILURE;

	for (int i = 0; i < LENGTH(vis_action); i++) {
		const KeyAction *action = &vis_action[i];
		if (!vis_action_register(vis, action))
			vis_die(vis, "Could not register action: %s\n", action->name);
	}

	for (int i = 0; i < LENGTH(default_bindings); i++) {
		for (const KeyBinding **binding = default_bindings[i]; binding && *binding; binding++) {
			for (const KeyBinding *kb = *binding; kb->key; kb++) {
				vis_mode_map(vis, i, false, kb->key, kb);
			}
		}
	}

	for (const char **k = keymaps; k[0]; k += 2)
		vis_keymap_add(vis, k[0], k[1]);

	/* install signal handlers etc. */
	struct sigaction sa;
	memset(&sa, 0, sizeof sa);
	sigfillset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = signal_handler;
	if (sigaction(SIGBUS, &sa, NULL) == -1 ||
	    sigaction(SIGINT, &sa, NULL) == -1 ||
	    sigaction(SIGCONT, &sa, NULL) == -1 ||
	    sigaction(SIGTSTP, &sa, NULL) == -1 ||
	    sigaction(SIGWINCH, &sa, NULL) == -1 ||
	    sigaction(SIGTERM, &sa, NULL) == -1 ||
	    sigaction(SIGHUP, &sa, NULL) == -1) {
		vis_die(vis, "Failed to set signal handler: %s\n", strerror(errno));
	}

	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGPIPE, &sa, NULL) == -1)
		vis_die(vis, "Failed to ignore SIGPIPE\n");

	sigset_t blockset;
	sigemptyset(&blockset);
	sigaddset(&blockset, SIGBUS);
	sigaddset(&blockset, SIGCONT);
	sigaddset(&blockset, SIGWINCH);
	sigaddset(&blockset, SIGTERM);
	sigaddset(&blockset, SIGTSTP);
	sigaddset(&blockset, SIGHUP);
	if (sigprocmask(SIG_BLOCK, &blockset, NULL) == -1)
		vis_die(vis, "Failed to block signals\n");

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			continue;
		} else if (strcmp(argv[i], "-") == 0) {
			continue;
		} else if (strcmp(argv[i], "--") == 0) {
			break;
		} else if (strcmp(argv[i], "-v") == 0) {
			printf("vis %s%s%s%s%s%s%s\n", VERSION,
			       CONFIG_CURSES ? " +curses" : "",
			       CONFIG_LUA ? " +lua" : "",
			       CONFIG_LPEG ? " +lpeg" : "",
			       CONFIG_TRE ? " +tre" : "",
			       CONFIG_ACL ? " +acl" : "",
			       CONFIG_SELINUX ? " +selinux" : "");
			return 0;
		} else {
			fprintf(stderr, "Unknown command option: %s\n", argv[i]);
			return 1;
		}
	}

	char *cmd = NULL;
	bool end_of_options = false, win_created = false;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' && !end_of_options) {
			if (strcmp(argv[i], "-") == 0) {
				if (!vis_window_new_fd(vis, STDOUT_FILENO))
					vis_die(vis, "Can not create empty buffer\n");
				ssize_t len = 0;
				char buf[PIPE_BUF];
				Text *txt = vis_text(vis);
				while ((len = read(STDIN_FILENO, buf, sizeof buf)) > 0)
					text_insert(txt, text_size(txt), buf, len);
				if (len == -1)
					vis_die(vis, "Can not read from stdin\n");
				text_snapshot(txt);
				int fd = ui->open(ui);
				if (fd == -1)
					vis_die(vis, "Can not reopen stdin\n");
				dup2(fd, STDIN_FILENO);
				close(fd);
			} else if (strcmp(argv[i], "--") == 0) {
				end_of_options = true;
				continue;
			}
		} else if (argv[i][0] == '+' && !end_of_options) {
			cmd = argv[i] + (argv[i][1] == '/' || argv[i][1] == '?');
			continue;
		} else if (!vis_window_new(vis, argv[i])) {
			vis_die(vis, "Can not load '%s': %s\n", argv[i], strerror(errno));
		}
		win_created = true;
		if (cmd) {
			vis_prompt_cmd(vis, cmd);
			cmd = NULL;
		}
	}

	if (!vis_window(vis) && !win_created) {
		if (!vis_window_new(vis, NULL))
			vis_die(vis, "Can not create empty buffer\n");
		if (cmd)
			vis_prompt_cmd(vis, cmd);
	}

	int status = vis_run(vis);
	vis_free(vis);
	return status;
}
