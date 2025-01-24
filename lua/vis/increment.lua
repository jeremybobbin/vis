-- increment/decrement number in dec/hex/oct format
local lexer, lpeg
lexer = require("scintillua/lexers/lexer")
lpeg = require("lpeg")
if not lexer.load or not lpeg then return end

local Cp = lpeg.Cp()
local dec_num = lpeg.S('+-')^-1 * lexer.dec_num
local pattern = lpeg.P{ Cp * (lexer.hex_num + lexer.oct_num + dec_num) * Cp + 1 * lpeg.V(1) }

local change = function(delta)
	local win = vis.win
	local file = win.file
	local count = vis.count
	if not count then count = 1 end
	vis.count = nil -- reset count, otherwise it affects next motion

	local fn = function(sel)
		if not sel.pos then return end
		local word = file:text_object_word(sel.pos);
		if not word then return end
		local data = file:content(word.start, 1024)
		if not data then return end
		local s, e = pattern:match(data)
		if not s then return end
		data = string.sub(data, s, e-1)
		if #data == 0 then return end
		-- align start and end for fileindex
		s = word.start + s - 1
		e = word.start + e - 1
		local base, format, padding = 10, 'd', 0
		if lexer.oct_num:match(data) then
			base = 8
			format = 'o'
			padding = #data
		elseif lexer.hex_num:match(data) then
			base = 16
			format = 'x'
			padding = #data - #"0x"
		end
		local number = tonumber(data, base == 8 and 8 or nil)
		if not number then return end
		number = number + delta * count
		-- string.format does not support negative hex/oct values
		if base ~= 10 and number < 0 then number = 0 end
		number = string.format((base == 16 and "0x" or "") .. "%0"..padding..format, number)
		if base == 8 and string.sub(number, 0, 1) ~= "0" then
			number = '0' .. number
		end
		file:delete(s, e - s)
		file:insert(s, number)
		sel.pos = s
	end

	for sel in win:selections_iterator() do
		fn(sel)
	end
end

vis:map(vis.modes.NORMAL, "<C-a>", function() change( 1) end, "Increment number")
vis:map(vis.modes.NORMAL, "<C-x>", function() change(-1) end, "Decrement number")
