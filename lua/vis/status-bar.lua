-- standard vis event handlers

local modes = {
	[vis.modes.NORMAL] = '',
	[vis.modes.OPERATOR_PENDING] = '',
	[vis.modes.VISUAL] = 'VISUAL',
	[vis.modes.VISUAL_LINE] = 'VISUAL-LINE',
	[vis.modes.INSERT] = 'INSERT',
	[vis.modes.REPLACE] = 'REPLACE',
}

vis.events.subscribe(vis.events.WIN_STATUS, function(win)
	local left_parts = {}
	local right_parts = {}
	local file = win.file
	local selection = win.selection
	local range = selection.range

	local mode = modes[vis.mode]
	if mode ~= '' and vis.win == win then
		table.insert(left_parts, mode)
	end

	table.insert(left_parts, (file.name or '[No Name]') ..
		(file.modified and ' [+]' or '') .. (vis.recording and ' @' or ''))

	local count = vis.count
	local keys = vis.input_queue
	if keys ~= '' then
		table.insert(right_parts, keys)
	elseif count then
		table.insert(right_parts, count)
	elseif range and range.finish > range.start+1 then
		table.insert(right_parts, range.finish - range.start)
	end

	if #win.selections > 1 then
		table.insert(right_parts, selection.number..'/'..#win.selections)
	end

	local size = file.size
	local pos = selection.pos
	if not pos then pos = 0 end
	table.insert(right_parts, (size == 0 and "0" or math.ceil(pos/size*100)).."%")

	if not win.large then
		local col = selection.col
		table.insert(right_parts, selection.line..', '..col)
		if size > 33554432 or col > 65536 then
			win.large = true
		end
	end

	local left = ' ' .. table.concat(left_parts, " » ") .. ' '
	local right = ' ' .. table.concat(right_parts, " « ") .. ' '
	win:status(left, right);
end)
