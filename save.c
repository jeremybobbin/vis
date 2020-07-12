#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* memrchr(3) is non-standard */
#endif
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <wchar.h>
#include <stdint.h>
#include <libgen.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#if CONFIG_ACL
#include <sys/acl.h>
#endif
#if CONFIG_SELINUX
#include <selinux/selinux.h>
#endif

#include "text.h"
#include "text-util.h"
#include "text-motions.h"
#include "util.h"

struct TextSave {                  /* used to hold context between text_save_{begin,commit} calls */
	Text *txt;                 /* text to operate on */
	char *filename;            /* filename to save to as given to text_save_begin */
	char *tmpname;             /* temporary name used for atomic rename(2) */
	int fd;                    /* file descriptor to write data to using text_save_write */
	enum TextSaveMethod type;  /* method used to save file */
};

static bool preserve_acl(int src, int dest) {
#if CONFIG_ACL
	acl_t acl = acl_get_fd(src);
	if (!acl)
		return errno == ENOTSUP ? true : false;
	if (acl_set_fd(dest, acl) == -1) {
		acl_free(acl);
		return false;
	}
	acl_free(acl);
#endif /* CONFIG_ACL */
	return true;
}

static bool preserve_selinux_context(int src, int dest) {
#if CONFIG_SELINUX
	char *context = NULL;
	if (!is_selinux_enabled())
		return true;
	if (fgetfilecon(src, &context) == -1)
		return errno == ENOTSUP ? true : false;
	if (fsetfilecon(dest, context) == -1) {
		freecon(context);
		return false;
	}
	freecon(context);
#endif /* CONFIG_SELINUX */
	return true;
}

/* Create a new file named `.filename.vis.XXXXXX` (where `XXXXXX` is a
 * randomly generated, unique suffix) and try to preserve all important
 * meta data. After the file content has been written to this temporary
 * file, text_save_commit_atomic will atomically move it to  its final
 * (possibly already existing) destination using rename(2).
 *
 * This approach does not work if:
 *
 *   - the file is a symbolic link
 *   - the file is a hard link
 *   - file ownership can not be preserved
 *   - file group can not be preserved
 *   - directory permissions do not allow creation of a new file
 *   - POSXI ACL can not be preserved (if enabled)
 *   - SELinux security context can not be preserved (if enabled)
 */
static bool text_save_begin_atomic(TextSave *ctx) {
	int oldfd, saved_errno;
	if ((oldfd = open(ctx->filename, O_RDONLY)) == -1 && errno != ENOENT)
		goto err;
	struct stat oldmeta = { 0 };
	if (oldfd != -1 && lstat(ctx->filename, &oldmeta) == -1)
		goto err;
	if (oldfd != -1) {
		if (S_ISLNK(oldmeta.st_mode)) /* symbolic link */
			goto err;
		if (oldmeta.st_nlink > 1) /* hard link */
			goto err;
	}

	char suffix[] = ".vis.XXXXXX";
	size_t len = strlen(ctx->filename) + sizeof("./.") + sizeof(suffix);
	char *dir = strdup(ctx->filename);
	char *base = strdup(ctx->filename);

	if (!(ctx->tmpname = malloc(len)) || !dir || !base) {
		free(dir);
		free(base);
		goto err;
	}

	snprintf(ctx->tmpname, len, "%s/.%s%s", dirname(dir), basename(base), suffix);
	free(dir);
	free(base);

	if ((ctx->fd = mkstemp(ctx->tmpname)) == -1)
		goto err;

	if (oldfd == -1) {
		mode_t mask = umask(0);
		umask(mask);
		if (fchmod(ctx->fd, 0666 & ~mask) == -1)
			goto err;
	} else {
		if (fchmod(ctx->fd, oldmeta.st_mode) == -1)
			goto err;
		if (!preserve_acl(oldfd, ctx->fd) || !preserve_selinux_context(oldfd, ctx->fd))
			goto err;
		/* change owner if necessary */
		if (oldmeta.st_uid != getuid() && fchown(ctx->fd, oldmeta.st_uid, (uid_t)-1) == -1)
			goto err;
		/* change group if necessary, in case of failure some editors reset
		 * the group permissions to the same as for others */
		if (oldmeta.st_gid != getgid() && fchown(ctx->fd, (uid_t)-1, oldmeta.st_gid) == -1)
			goto err;
		close(oldfd);
	}

	ctx->type = TEXT_SAVE_ATOMIC;
	return true;
err:
	saved_errno = errno;
	if (oldfd != -1)
		close(oldfd);
	if (ctx->fd != -1)
		close(ctx->fd);
	ctx->fd = -1;
	free(ctx->tmpname);
	ctx->tmpname = NULL;
	errno = saved_errno;
	return false;
}

static bool text_save_commit_atomic(TextSave *ctx) {
	if (fsync(ctx->fd) == -1)
		return false;

	struct stat meta = { 0 };
	if (fstat(ctx->fd, &meta) == -1)
		return false;

	bool close_failed = (close(ctx->fd) == -1);
	ctx->fd = -1;
	if (close_failed)
		return false;

	if (rename(ctx->tmpname, ctx->filename) == -1)
		return false;

	free(ctx->tmpname);
	ctx->tmpname = NULL;

	int dir = open(dirname(ctx->filename), O_DIRECTORY|O_RDONLY);
	if (dir == -1)
		return false;

	if (fsync(dir) == -1 && errno != EINVAL) {
		close(dir);
		return false;
	}

	if (close(dir) == -1)
		return false;

	if (meta.st_mtime)
		ctx->txt->info = meta;
	return true;
}

static bool text_save_begin_inplace(TextSave *ctx) {
	Text *txt = ctx->txt;
	struct stat meta = { 0 };
	int newfd = -1, saved_errno;
	if ((ctx->fd = open(ctx->filename, O_CREAT|O_WRONLY, 0666)) == -1)
		goto err;
	if (fstat(ctx->fd, &meta) == -1)
		goto err;
	Block *block = txt->block;
	if (meta.st_dev == txt->info.st_dev && meta.st_ino == txt->info.st_ino &&
	    block && block->type == MMAP_ORIG && block->size) {
		/* The file we are going to overwrite is currently mmap-ed from
		 * text_load, therefore we copy the mmap-ed block to a temporary
		 * file and remap it at the same position such that all pointers
		 * from the various pieces are still valid.
		 */
		size_t size = block->size;
		char tmpname[32] = "/tmp/vis-XXXXXX";
		newfd = mkstemp(tmpname);
		if (newfd == -1)
			goto err;
		if (unlink(tmpname) == -1)
			goto err;
		ssize_t written = write_all(newfd, block->data, size);
		if (written == -1 || (size_t)written != size)
			goto err;
		void *data = mmap(block->data, size, PROT_READ, MAP_SHARED|MAP_FIXED, newfd, 0);
		if (data == MAP_FAILED)
			goto err;
		bool close_failed = (close(newfd) == -1);
		newfd = -1;
		if (close_failed)
			goto err;
		block->type = MMAP;
	}
	/* overwrite the existing file content, if something goes wrong
	 * here we are screwed, TODO: make a backup before? */
	if (ftruncate(ctx->fd, 0) == -1)
		goto err;
	ctx->type = TEXT_SAVE_INPLACE;
	return true;
err:
	saved_errno = errno;
	if (newfd != -1)
		close(newfd);
	if (ctx->fd != -1)
		close(ctx->fd);
	ctx->fd = -1;
	errno = saved_errno;
	return false;
}

static bool text_save_commit_inplace(TextSave *ctx) {
	if (fsync(ctx->fd) == -1)
		return false;
	struct stat meta = { 0 };
	if (fstat(ctx->fd, &meta) == -1)
		return false;
	if (close(ctx->fd) == -1)
		return false;
	ctx->txt->info = meta;
	return true;
}

TextSave *text_save_begin(Text *txt, const char *filename, enum TextSaveMethod type) {
	if (!filename)
		return NULL;
	TextSave *ctx = calloc(1, sizeof *ctx);
	if (!ctx)
		return NULL;
	ctx->txt = txt;
	ctx->fd = -1;
	if (!(ctx->filename = strdup(filename)))
		goto err;
	errno = 0;
	if ((type == TEXT_SAVE_AUTO || type == TEXT_SAVE_ATOMIC) && text_save_begin_atomic(ctx))
		return ctx;
	if (errno == ENOSPC)
		goto err;
	if ((type == TEXT_SAVE_AUTO || type == TEXT_SAVE_INPLACE) && text_save_begin_inplace(ctx))
		return ctx;
err:
	text_save_cancel(ctx);
	return NULL;
}

bool text_save_commit(TextSave *ctx) {
	if (!ctx)
		return true;
	bool ret;
	Text *txt = ctx->txt;
	switch (ctx->type) {
	case TEXT_SAVE_ATOMIC:
		ret = text_save_commit_atomic(ctx);
		break;
	case TEXT_SAVE_INPLACE:
		ret = text_save_commit_inplace(ctx);
		break;
	default:
		ret = false;
		break;
	}

	if (ret) {
		txt->saved_revision = txt->history;
		text_snapshot(txt);
	}
	text_save_cancel(ctx);
	return ret;
}

void text_save_cancel(TextSave *ctx) {
	if (!ctx)
		return;
	int saved_errno = errno;
	if (ctx->fd != -1)
		close(ctx->fd);
	if (ctx->tmpname && ctx->tmpname[0])
		unlink(ctx->tmpname);
	free(ctx->tmpname);
	free(ctx->filename);
	free(ctx);
	errno = saved_errno;
}

/* First try to save the file atomically using rename(2) if this does not
 * work overwrite the file in place. However if something goes wrong during
 * this overwrite the original file is permanently damaged.
 */
bool text_save(Text *txt, const char *filename) {
	return text_save_method(txt, filename, TEXT_SAVE_AUTO);
}

bool text_save_method(Text *txt, const char *filename, enum TextSaveMethod method) {
	if (!filename) {
		txt->saved_revision = txt->history;
		text_snapshot(txt);
		return true;
	}
	TextSave *ctx = text_save_begin(txt, filename, method);
	if (!ctx)
		return false;
	Filerange range = (Filerange){ .start = 0, .end = text_size(txt) };
	ssize_t written = text_save_write_range(ctx, &range);
	if (written == -1 || (size_t)written != text_range_size(&range)) {
		text_save_cancel(ctx);
		return false;
	}
	return text_save_commit(ctx);
}

ssize_t text_save_write_range(TextSave *ctx, Filerange *range) {
	return text_write_range(ctx->txt, range, ctx->fd);
}
