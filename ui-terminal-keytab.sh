#!/bin/sh
echo "const char *ui_terminal_keytab[][3] = {"
(
	tmp=$(mktemp -d)
	infocmp |
		sed -n 's/, /\n/g; s/,$//g; s/^\t//; /^k/p; ' |
		tr '=' ' ' |
		sort -k1b,1 > "$tmp/1" 

	sed '/^$/d' keytab.in | sort -k1b,1 > "$tmp/2"

	join "$tmp/1" "$tmp/2" | while read -r termcap sequence key long code desc; do
		case "$sequence" in
		^?)
		esac
		printf ' { "%s",\t"%s",\t"%s"\t},\t// %s\t%s\t%s\n' "$termcap" "$sequence" "$key" "$long" "$code" "$desc"
	done | column -ts'	'
) | sed 's,^,\t,'
echo "};"
