#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "string.h"
#include "util.h"

#ifndef STRING_SIZE
#define STRING_SIZE 1024
#endif

void string_init(String *str) {
	memset(str, 0, sizeof *str);
}

bool string_reserve(String *str, size_t size) {
	/* ensure minimal buffer size, to avoid repeated realloc(3) calls */
	if (size < STRING_SIZE)
		size = STRING_SIZE;
	if (str->size < size) {
		size = MAX(size, str->size*2);
		char *data = realloc(str->data, size);
		if (!data)
			return false;
		str->size = size;
		str->data = data;
	}
	return true;
}

bool string_grow(String *str, size_t len) {
	size_t size;
	if (!addu(str->len, len, &size))
		return false;
	return string_reserve(str, size);
}

bool string_terminate(String *str) {
	return !str->data || str->len == 0 || str->data[str->len-1] == '\0' ||
	        string_append(str, "\0", 1);
}

void string_release(String *str) {
	if (!str)
		return;
	free(str->data);
	string_init(str);
}

void string_clear(String *str) {
	str->len = 0;
}

bool string_put(String *str, const void *data, size_t len) {
	if (!string_reserve(str, len))
		return false;
	memmove(str->data, data, len);
	str->len = len;
	return true;
}

bool string_put0(String *str, const char *data) {
	return string_put(str, data, strlen(data)+1);
}

bool string_remove(String *str, size_t pos, size_t len) {
	size_t end;
	if (len == 0)
		return true;
	if (!addu(pos, len, &end) || end > str->len)
		return false;
	memmove(str->data + pos, str->data + pos + len, str->len - pos - len);
	str->len -= len;
	return true;
}

bool string_insert(String *str, size_t pos, const void *data, size_t len) {
	if (pos > str->len)
		return false;
	if (len == 0)
		return true;
	if (!string_grow(str, len))
		return false;
	size_t move = str->len - pos;
	if (move > 0)
		memmove(str->data + pos + len, str->data + pos, move);
	memcpy(str->data + pos, data, len);
	str->len += len;
	return true;
}

bool string_insert0(String *str, size_t pos, const char *data) {
	if (pos == 0)
		return string_prepend0(str, data);
	if (pos == str->len)
		return string_append0(str, data);
	return string_insert(str, pos, data, strlen(data));
}

bool string_append(String *str, const void *data, size_t len) {
	return string_insert(str, str->len, data, len);
}

bool string_append0(String *str, const char *data) {
	size_t nul = (str->len > 0 && str->data[str->len-1] == '\0') ? 1 : 0;
	str->len -= nul;
	bool ret = string_append(str, data, strlen(data)+1);
	if (!ret)
		str->len += nul;
	return ret;
}

bool string_prepend(String *str, const void *data, size_t len) {
	return string_insert(str, 0, data, len);
}

bool string_prepend0(String *str, const char *data) {
	return string_prepend(str, data, strlen(data) + (str->len == 0));
}

static bool buffer_vappendf(String *str, const char *fmt, va_list ap) {
	va_list ap_save;
	va_copy(ap_save, ap);
	int len = vsnprintf(NULL, 0, fmt, ap);
	if (len == -1 || !string_grow(str, len+1)) {
		va_end(ap_save);
		return false;
	}
	size_t nul = (str->len > 0 && str->data[str->len-1] == '\0') ? 1 : 0;
	str->len -= nul;
	bool ret = vsnprintf(str->data+str->len, len+1, fmt, ap_save) == len;
	str->len += ret ? (size_t)len+1 : nul;
	va_end(ap_save);
	return ret;
}

bool string_appendf(String *str, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	bool ret = buffer_vappendf(str, fmt, ap);
	va_end(ap);
	return ret;
}

bool string_printf(String *str, const char *fmt, ...) {
	string_clear(str);
	va_list ap;
	va_start(ap, fmt);
	bool ret = buffer_vappendf(str, fmt, ap);
	va_end(ap);
	return ret;
}

size_t string_length0(String *str) {
	size_t len = str->len;
	if (len > 0 && str->data[len-1] == '\0')
		len--;
	return len;
}

size_t string_length(String *str) {
	return str->len;
}

size_t string_capacity(String *str) {
	return str->size;
}

const char *string_content(String *str) {
	return str->data;
}

const char *string_content0(String *str) {
	if (str->len == 0 || !string_terminate(str))
		return "";
	return str->data;
}

char *string_move(String *str) {
	char *data = str->data;
	string_init(str);
	return data;
}
