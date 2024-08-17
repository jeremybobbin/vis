.POSIX:
.SUFFIXES: .o .c

include config.mk

SRC = array.c \
	buffer.c \
	libutf.c \
	main.c \
	map.c \
	sam.c \
	text.c \
	text-common.c \
	text-io.c \
	text-iterator.c \
	text-motions.c \
	text-objects.c \
	text-util.c \
	ui-terminal.c \
	view.c \
	vis.c \
	vis-lua.c \
	vis-marks.c \
	vis-modes.c \
	vis-motions.c \
	vis-operators.c \
	vis-prompt.c \
	vis-registers.c \
	vis-text-objects.c \
	$(REGEX_SRC)

OBJ=$(SRC:c=o)

ELF = vis vis-menu vis-digraph
EXECUTABLES = $(ELF) vis-clipboard vis-complete vis-open

MANUALS = $(EXECUTABLES:=.1)

DOCUMENTATION = LICENSE README.md

CFLAGS_VIS = $(CFLAGS_AUTO) $(CFLAGS_CURSES) $(CFLAGS_ACL) \
	$(CFLAGS_SELINUX) $(CFLAGS_TRE) $(CFLAGS_LUA) $(CFLAGS_LPEG) $(CFLAGS_STD) \
	$(CFLAGS_LIBC) \
	-DVIS_PATH=\"${SHAREPREFIX}/vis\" \
	-DCONFIG_HELP=${CONFIG_HELP} \
	-DCONFIG_CURSES=${CONFIG_CURSES} \
	-DCONFIG_LUA=${CONFIG_LUA} \
	-DCONFIG_LPEG=${CONFIG_LPEG} \
	-DCONFIG_TRE=${CONFIG_TRE} \
	-DCONFIG_SELINUX=${CONFIG_SELINUX} \
	-DCONFIG_ACL=${CONFIG_ACL} \
	-DVERSION=\"${VERSION}\"

LDFLAGS_VIS = $(LDFLAGS_AUTO) $(LDFLAGS_CURSES) $(LDFLAGS_ACL) \
	$(LDFLAGS_SELINUX) $(LDFLAGS_TRE) $(LDFLAGS_LUA) $(LDFLAGS_LPEG) $(LDFLAGS_STD)

STRIP=strip
TAR=tar
DOCKER=docker

all: $(ELF)

.c.o:
	$(CC) ${CFLAGS} ${CFLAGS_VIS} ${CFLAGS_STD} ${CFLAGS_EXTRA} -c $< -o $@

config.h:
	cp config.def.h config.h

config.mk:
	@touch $@

$(OBJ): config.mk config.h


map.o: map.h
array.o: array.h util.h
buffer.o: buffer.h util.h
libutf.o: libutf.h util.h

text-common.o: text.h
text-io.o: text.h text-internal.h text-util.h util.h
text-iterator.o: text.h util.h
text-motions.o: text-motions.h text-util.h util.h text-objects.h
text-objects.o: text-motions.h text-objects.h text-util.h util.h
text.o: text.h text-util.h text-motions.h util.h array.h text-internal.h

ui-terminal-keytab.h: keytab.in
	./ui-terminal-keytab.sh > $@

sam.o: sam.h vis-core.h buffer.h text.h text-motions.h text-objects.h text-regex.h util.h vis-cmds.c
$(REGEX_SRC:.c=.o): text-regex.h $(REGEX_SRC)
text-util.o: text-util.h util.h
ui-terminal-vt100.o: buffer.h
ui-terminal.o: ui-terminal-keytab.h vis.h vis-core.h text.h util.h text-util.h ui-terminal-curses.c ui-terminal-vt100.c
view.o: view.h text.h text-motions.h text-util.h util.h
vis-cmds.o: vis-lua.h sam.c
vis-lua.o: vis-lua.h vis-core.h text-motions.h util.h

vis-marks.o: vis-core.h
vis-registers.o: vis-core.h
vis-modes.o: vis-core.h text-motions.h util.h
vis-motions.o: vis-core.h text-motions.h text-objects.h text-util.h util.h
vis-operators.o: vis-core.h text-motions.h text-objects.h text-util.h util.h
vis-prompt.o: vis-core.h text-motions.h text-objects.h text-util.h
vis-text-objects.o: vis-core.h text-objects.h util.h

vis.o: vis.h text-util.h text-motions.h text-objects.h util.h vis-core.h sam.h ui.h
main.o: ui-terminal.h vis.h vis-lua.h text-util.h text-motions.h text-objects.h util.h libutf.h array.h buffer.h config.h

vis: \
	$(REGEX_SRC:.c=.o) \
	array.o \
	buffer.o \
	map.o \
	sam.o \
	text-common.o \
	text-io.o \
	text-iterator.o \
	text-motions.o \
	text.o \
	text-objects.o \
	text-util.o \
	view.o \
	vis-lua.o \
	vis-modes.o \
	vis-marks.o \
	vis-prompt.o \
	vis-motions.o \
	vis-registers.o \
	vis-text-objects.o \
	vis-operators.o \
	libutf.o \
	ui-terminal.o \
	main.o \
	vis.o
	${CC} ${CFLAGS} ${CFLAGS_VIS} ${CFLAGS_EXTRA} $^  -o $@  ${LDFLAGS} ${LDFLAGS_VIS}

vis-menu: vis-menu.c
	${CC} ${CFLAGS} ${CFLAGS_AUTO} ${CFLAGS_STD} ${CFLAGS_EXTRA} -DVERSION=\"${VERSION}\" $< ${LDFLAGS} ${LDFLAGS_STD} ${LDFLAGS_AUTO} -o $@

vis-digraph: vis-digraph.c
	${CC} ${CFLAGS} ${CFLAGS_AUTO} ${CFLAGS_STD} ${CFLAGS_EXTRA} -DVERSION=\"${VERSION}\" $< ${LDFLAGS} ${LDFLAGS_STD} ${LDFLAGS_AUTO} -o $@

vis-single-payload.inc: $(EXECUTABLES) lua/*
	for e in $(ELF); do \
		${STRIP} "$$e"; \
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
	${CC} ${CFLAGS} ${CFLAGS_AUTO} ${CFLAGS_STD} ${CFLAGS_EXTRA} $< ${LDFLAGS} ${LDFLAGS_STD} ${LDFLAGS_AUTO} -luntar -llzma -o $@
	${STRIP} $@

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

debug: clean
	@$(MAKE) CFLAGS_EXTRA='${CFLAGS_EXTRA} ${CFLAGS_DEBUG}'

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
	@rm -f $(ELF) vis-single vis-single-payload.inc vis-*.tar.gz *.gcov *.gcda *.gcno *.o

distclean: clean testclean
	@echo cleaning build configuration
	@rm -f config.h config.mk

dist: distclean
	@echo creating dist tarball
	@git archive --prefix=vis-${VERSION}/ -o vis-${VERSION}.tar.gz HEAD

man:
	@for m in ${MANUALS}; do \
		echo "Generating $$m"; \
		sed -e "s/VERSION/${VERSION}/" "man/$$m" | mandoc -W warning -T utf8 -T html -O man=%N.%S.html -O style=mandoc.css 1> "man/$$m.html" || true; \
	done

luadoc:
	@cd lua/doc && ldoc . && sed -e "s/RELEASE/${VERSION}/" -i index.html

luadoc-all:
	@cd lua/doc && ldoc -a . && sed -e "s/RELEASE/${VERSION}/" -i index.html

luacheck:
	@luacheck --config .luacheckrc lua test/lua | less -RFX

install: $(ELF)
	@echo installing executable files to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@for e in ${EXECUTABLES}; do \
		cp -f "$$e" ${DESTDIR}${PREFIX}/bin && \
		chmod 755 ${DESTDIR}${PREFIX}/bin/"$$e"; \
	done
	@test ${CONFIG_LUA} -eq 0 || { \
		echo installing support files to ${DESTDIR}${SHAREPREFIX}/vis; \
		mkdir -p ${DESTDIR}${SHAREPREFIX}/vis; \
		cp -r lua/* ${DESTDIR}${SHAREPREFIX}/vis; \
		rm -rf "${DESTDIR}${SHAREPREFIX}/vis/doc"; \
	}
	@echo installing documentation to ${DESTDIR}${DOCPREFIX}/vis
	@mkdir -p ${DESTDIR}${DOCPREFIX}/vis
	@for d in ${DOCUMENTATION}; do \
		cp "$$d" ${DESTDIR}${DOCPREFIX}/vis && \
		chmod 644 "${DESTDIR}${DOCPREFIX}/vis/$$d"; \
	done
	@echo installing manual pages to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@for m in ${MANUALS}; do \
		sed -e "s/VERSION/${VERSION}/" < "man/$$m" >  "${DESTDIR}${MANPREFIX}/man1/$$m" && \
		chmod 644 "${DESTDIR}${MANPREFIX}/man1/$$m"; \
	done

install-strip: install
	@echo stripping executables
	@for e in $(ELF); do \
		${STRIP} ${DESTDIR}${PREFIX}/bin/"$$e"; \
	done

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@for e in ${EXECUTABLES}; do \
		rm -f ${DESTDIR}${PREFIX}/bin/"$$e"; \
	done
	@echo removing documentation from ${DESTDIR}${DOCPREFIX}/vis
	@for d in ${DOCUMENTATION}; do \
		rm -f ${DESTDIR}${DOCPREFIX}/vis/"$$d"; \
	done
	@echo removing manual pages from ${DESTDIR}${MANPREFIX}/man1
	@for m in ${MANUALS}; do \
		rm -f ${DESTDIR}${MANPREFIX}/man1/"$$m"; \
	done
	@echo removing support files from ${DESTDIR}${SHAREPREFIX}/vis
	@rm -rf ${DESTDIR}${SHAREPREFIX}/vis

.PHONY: all clean testclean dist distclean install install-strip uninstall debug profile coverage test test-update luadoc luadoc-all luacheck man docker-kill docker docker-clean
