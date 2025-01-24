-- LuaLPEG vis-extentions
---
-- @type File

--- Check whether LPeg pattern matches at a given file position.
-- @function match_at
-- @param pattern the LPeg pattern
-- @tparam int pos the absolute file position which should be tested for a match
-- @tparam[opt] int horizon the number of bytes around `pos` to consider (defaults to 1K)
-- @treturn int start,end the range of the matched region or `nil`
vis.types.file.match_at = function(file, pattern, pos, horizon)
	horizon = horizon or 1024
	local lpeg = vis.lpeg
	if not lpeg then return nil end
	local before, after = pos - horizon, pos + horizon
	if before < 0 then before = 0 end
	local data = file:content(before, after - before)
	local string_pos = pos - before + 1

	local I = lpeg.Cp()
	local p = lpeg.P{ I * pattern * I + 1 * lpeg.V(1) }
	local s, e = 1
	while true do
		s, e = p:match(data, s)
		if not s then return nil end
		if s <= string_pos and string_pos < e then
			return before + s - 1, before + e - 1
		end
		s = e
	end
end
