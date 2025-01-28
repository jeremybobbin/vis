.POSIX:

include config.mk

.SUFFIXES: .o .c .a .lua

ELF=vis vis-menu vis-digraph vis-keys
EXECUTABLES=$(ELF) vis-clipboard vis-complete vis-open

MANUALS=$(EXECUTABLES:=.1)

DOCUMENTATION=LICENSE README.md

CFLAGS_VIS=$(CFLAGS_AUTO) $(CFLAGS_CURSES) $(CFLAGS_ACL) $(CFLAGS_SELINUX) $(CFLAGS_TRE) $(CFLAGS_LUA) $(CFLAGS_LPEG) $(CFLAGS_STD) -DVIS_PATH=\"$(SHAREPREFIX)/vis\"

LDFLAGS_VIS=$(LDFLAGS_AUTO) $(LDFLAGS_CURSES) $(LDFLAGS_ACL) $(LDFLAGS_SELINUX) $(LDFLAGS_TRE) $(LDFLAGS_LUA) $(LDFLAGS_LPEG) $(LDFLAGS_STD)

STRIP=strip
TAR=tar
DOCKER=docker

all: $(ELF)

.c.o:
	$(CC) $(CFLAGS) $(CFLAGS_VIS) $(CFLAGS_EXTRA) -c $< -o $@

.o.a:
	$(AR) $(ARFLAGS) $@ $?

.a:
	$(CC) $(CFLAGS) $(CFLAGS_VIS) $(CFLAGS_EXTRA) $? -o $@ $(LDFLAGS) $(LDFLAGS_VIS)

# TODO: simplify
.lua.c:
	$(LUAC) $< | \
		od -v -A n -t oC | \
		sed 's+ +\\+g; s+^+	"+; 1,1s+^+const char $*[] = \n+; s+$$+"+; $$,$$s,$$,;\nint $*_size = sizeof($*)-1;,;' | \
		tr '/' '_' > $@

config.mk:
	./configure

ui-terminal-keytab.h: keytab.in
	./ui-terminal-keytab.sh > $@

map.c: map.h
array.c: array.h util.h
string.c: string.h util.h
libutf.c: libutf.h util.h

vis.h: vis-core.h ui.h view.h text-regex.h libutf.h array.h string.h

text.o: text.h text-internal.h text-util.h text-motions.h array.h util.h
text-common.o: text.h
text-io.o: text-internal.h  text.h text-util.h util.h
text-iterator.o: text.h util.h
text-motions.o: text-motions.h text-objects.h text-util.h util.h
text-objects.o: text-objects.h text-motions.h text-util.h util.h
$(REGEX).o: text-regex.h
text.a: text.o text-common.o text-iterator.o text-util.o text-io.o $(REGEX).o text-objects.o text-motions.o


sam.o: sam.h vis-core.h string.h text.h text-motions.h text-objects.h text-regex.h util.h vis-cmds.c
text-util.o: text-util.h util.h
ui-terminal.o: $(UI_TERMINAL_BACKEND).c ui.h libkey.h ui-terminal-keytab.h vis.h vis-core.h text.h util.h text-util.h
view.o: view.h text.h text-motions.h text-util.h util.h

vis-lua.o: vis-lua.h vis-core.h text-objects.h text-motions.h util.h
vis-marks.o: vis-core.h
vis-registers.o: vis-core.h
vis-modes.o: vis-core.h text-motions.h util.h
vis-motions.o: vis-core.h text-motions.h text-objects.h text-util.h util.h
vis-operators.o: vis-core.h text-motions.h text-objects.h text-util.h util.h
vis-prompt.o: vis-core.h text-motions.h text-objects.h text-util.h
vis-text-objects.o: vis-core.h text-objects.h util.h
util.o: util.h

vis-keys.o: ui-terminal-keytab.h

libkey.o: libkey.h map.o
vis.o: vis.h text-util.h util.h vis-core.h ui.h text-motions.h text-objects.h sam.h ui-terminal.h # TODO ui.o
main.o: config.h util.h array.h string.h libutf.h libkey.h text-util.h text-motions.h text-objects.h ui-terminal.h vis-lua.h vis.h

vis-core.a: \
		sam.o \
		libutf.o \
		map.o \
		libkey.o \
		lua/internal.o \
		ui-terminal.o \
		view.o \
		vis.o \
		vis-lua.o \
		vis-marks.o \
		vis-modes.o \
		vis-motions.o \
		vis-operators.o \
		vis-text-objects.o \
		util.o \
		vis-prompt.o \
		vis-registers.o \
		main.o
	$(AR) $(ARFLAGS) $@ $?



vis: array.o string.o text.a vis-core.a
	$(CC) $(CFLAGS) $(CFLAGS_VIS) $(CFLAGS_EXTRA) vis-core.a text.a array.o string.o  -o $@ $(LDFLAGS) $(LDFLAGS_VIS)


vis-menu: vis-menu.a
vis-keys.a: vis-keys.o map.o
vis-keys: vis-keys.a

vis-single-payload.inc: $(EXECUTABLES) lua/*
	for e in $(ELF); do \
		$(STRIP) "$$e"; \
	done
	echo '#ifndef VIS_SINGLE_PAYLOAD_H' > $@
	echo '#define VIS_SINGLE_PAYLOAD_H' >> $@
	echo 'static unsigned char vis_single_payload[] = {' >> $@
	$(TAR) --mtime='2014-07-15 01:23Z' --owner=0 --group=0 --numeric-owner --mode='a+rX-w' -c \
		$(EXECUTABLES) $$(find lua -name '*.lua' | LC_ALL=C sort) | xz -T 1 | \
		od -t x1 -A n -v | sed 's/\([0-9a-f]\{2\}\)/0x\1,/g' >> $@
	echo '};' >> $@
	echo '#endif' >> $@

vis-single: vis-single.c vis-single-payload.inc
	$(CC) $(CFLAGS) $(CFLAGS_AUTO) $(CFLAGS_STD) $(CFLAGS_EXTRA) $< $(LDFLAGS) $(LDFLAGS_STD) $(LDFLAGS_AUTO) -luntar -llzma -o $@
	$(STRIP) $@

docker-kill:
	-$(DOCKER) kill vis && $(DOCKER) wait vis

docker: docker-kill clean
	$(DOCKER) build -t vis .
	$(DOCKER) run --rm -d --name vis vis tail -f /dev/null
	$(DOCKER) exec vis apk update
	$(DOCKER) exec vis apk upgrade
	$(DOCKER) cp . vis:/build/vis
	$(DOCKER) exec -w /build/vis vis ./configure CC='cc --static' \
		--enable-acl \
		--enable-lua \
		--enable-lpeg-static
	$(DOCKER) exec -w /build/vis vis make VERSION="$(VERSION)" clean vis-single
	$(DOCKER) cp vis:/build/vis/vis-single vis
	$(DOCKER) kill vis

docker-clean: docker-kill clean
	-$(DOCKER) image rm vis

release: clean
	@$(MAKE) CFLAGS_EXTRA='$(CFLAGS_EXTRA) $(CFLAGS_RELEASE)'

debug: clean
	@$(MAKE) CFLAGS_EXTRA='$(CFLAGS_EXTRA) $(CFLAGS_DEBUG)'

profile: clean
	@$(MAKE) CFLAGS_AUTO='' LDFLAGS_AUTO='' CFLAGS_EXTRA='-pg -O2'

coverage: clean
	@$(MAKE) CFLAGS_EXTRA='--coverage'

test-update:
	git submodule init
	git submodule update --remote --rebase

test:
	[ -e test/Makefile ] || $(MAKE) test-update
	@$(MAKE) -C test

testclean:
	@echo cleaning the test artifacts
	[ ! -e test/Makefile ] || $(MAKE) -C test clean

clean:
	@echo cleaning
	@rm -f $(ELF) vis-single vis-single-payload.inc ui-terminal-keytab.h vis-*.tar.gz *.gcov *.gcda *.gcno *.o *.a lua/*.c lua/*.o

distclean: clean testclean
	@echo cleaning build configuration
	@rm -f config.mk

dist: distclean
	@echo creating dist tarball
	@git archive --prefix=vis-$(VERSION)/ -o vis-$(VERSION).tar.gz HEAD

man:
	@for m in $(MANUALS); do \
		echo "Generating $$m"; \
		sed -e "s/VERSION/$(VERSION)/" "man/$$m" | mandoc -W warning -T utf8 -T html -O man=%N.%S.html -O style=mandoc.css 1> "man/$$m.html" || true; \
	done

luadoc:
	@cd lua/doc && ldoc . && sed -e "s/RELEASE/$(VERSION)/" -i index.html

luadoc-all:
	@cd lua/doc && ldoc -a . && sed -e "s/RELEASE/$(VERSION)/" -i index.html

luacheck:
	@luacheck --config .luacheckrc lua test/lua | less -RFX

install: $(ELF)
	@echo installing executable files to $(DESTDIR)$(PREFIX)/bin
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@for e in $(EXECUTABLES); do \
		cp -f "$$e" $(DESTDIR)$(PREFIX)/bin && \
		chmod 755 $(DESTDIR)$(PREFIX)/bin/"$$e"; \
	done
	@if [ -n "$(CFLAGS_LUA)" ]; then \
		echo installing support files to $(DESTDIR)$(SHAREPREFIX)/vis; \
		mkdir -p $(DESTDIR)$(LUAPREFIX)/vis; \
		cp -r lua/vis $(DESTDIR)$(LUAPREFIX); \
		rm -rf "$(DESTDIR)$(SHAREPREFIX)/vis/doc"; \
	fi
	@echo installing documentation to $(DESTDIR)$(DOCPREFIX)/vis
	@mkdir -p $(DESTDIR)$(DOCPREFIX)/vis
	@for d in $(DOCUMENTATION); do \
		cp "$$d" $(DESTDIR)$(DOCPREFIX)/vis && \
		chmod 644 "$(DESTDIR)$(DOCPREFIX)/vis/$$d"; \
	done
	@echo installing manual pages to $(DESTDIR)$(MANPREFIX)/man1
	@mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	@for m in $(MANUALS); do \
		sed -e "s/VERSION/$(VERSION)/" < "man/$$m" >  "$(DESTDIR)$(MANPREFIX)/man1/$$m" && \
		chmod 644 "$(DESTDIR)$(MANPREFIX)/man1/$$m"; \
	done

install-strip: install
	@echo stripping executables
	@for e in $(ELF); do \
		$(STRIP) $(DESTDIR)$(PREFIX)/bin/"$$e"; \
	done

uninstall:
	@echo removing executable file from $(DESTDIR)$(PREFIX)/bin
	@for e in $(EXECUTABLES); do \
		rm -f $(DESTDIR)$(PREFIX)/bin/"$$e"; \
	done
	@echo removing documentation from $(DESTDIR)$(DOCPREFIX)/vis
	@for d in $(DOCUMENTATION); do \
		rm -f $(DESTDIR)$(DOCPREFIX)/vis/"$$d"; \
	done
	@echo removing manual pages from $(DESTDIR)$(MANPREFIX)/man1
	@for m in $(MANUALS); do \
		rm -f $(DESTDIR)$(MANPREFIX)/man1/"$$m"; \
	done
	@echo removing support files from $(DESTDIR)$(SHAREPREFIX)/vis
	@rm -rf $(DESTDIR)$(SHAREPREFIX)/vis

.PHONY: all clean testclean dist distclean install install-strip uninstall debug profile coverage test test-update luadoc luadoc-all luacheck man docker-kill docker docker-clean
