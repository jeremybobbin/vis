define vis_defaults
	file vis

	catch signal SIGILL
	handle SIGILL nostop nopass

	set print inferior-events off
end

define count_label_hits
	set $labels = ((char*[128]) {})
	set $li = 0

	break debug_label
	commands
		silent
		eval "set $v = $%s", label
		if $_isvoid($v)
			eval "set $%s = 0", label
			set $labels[$li] = label
			set $li = $li + 1
		end
		eval "set $%s = $%s + 1", label, label
		continue
	end

	break exit
	commands
		silent
		set $i = 0
		while $i < $li
			eval "set $v = $%s", $labels[$i]
			printf "%20s: %d\n", $labels[$i], $v
			set $i = $i + 1
		end
	end

end
