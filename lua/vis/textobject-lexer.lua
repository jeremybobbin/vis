-- text object matching a lexer token

local MAX_CONTEXT = 32768

local lexers
lexers = require("vis/scintillua")

vis:textobject_new("ii", function(win, pos)

	if not win.syntax or not lexers.load then
		return nil
	end

	local lexer = lexers.load(win.syntax, nil, true)
	if not lexer then
		return nil
	end

	local before, after = pos - MAX_CONTEXT, pos + MAX_CONTEXT
	if before < 0 then
		before = 0
	end
	-- TODO make sure we start at a line boundary?

	local data = win.file:content(before, after - before)
	local tokens = lexer:lex(data)
	local cur = before

	for i = 1, #tokens, 2 do
		local token_next = before + tokens[i+1] - 1
		if cur <= pos and pos < token_next then
			return cur, token_next
		end
		cur = token_next
	end

	return nil
end, "Current lexer token")