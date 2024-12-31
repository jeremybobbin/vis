#!/bin/sh -e
(
	tmp=$(mktemp -d)
	infocmp |
		sed -n '/[a-zA-Z0-9]\+=/ { s/[ \t]//g; s/,/\n/; s/=/ /; s/\\E/\\x1b/g; P; D; }' |
		sort -k1b,1 > "$tmp/1"

	echo "const char *ui_terminal_keytab[][3] = {"
	awk -v "tmp=$tmp"  '/^k/ { print } !/^k/ && NF { print > tmp"/3" }' keytab.in | sort -k1b,1 > "$tmp/2"
		join "$tmp/1" "$tmp/2" | while read -r termcap sequence key long code desc; do
			printf ' { "%s",\t"%s",\t"%s"\t},\t// %s\t%s\t%s\n' "$termcap" "$sequence" "$key" "$long" "$code" "$desc"
		done | column -ts'	' | sed 's,^,\t,'
	echo "};"
	echo
	echo "const char *ui_terminal_control[][2] = {"
	sort -k1b,1 "$tmp/3" | join "$tmp/1" - | while read -r termcap sequence long code desc; do
		printf ' { "%s",\t"%s"\t},\t// %s\t%s\t%s\n' "$termcap" "$sequence" "$long" "$code" "$desc"
	done | column -ts'	' | sed 's,^,\t,'
	echo "};"

)
