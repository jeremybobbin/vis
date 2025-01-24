local lexers
lexers = require("scintillua/lexers/lexer")

--- Cache of loaded lexers
--
-- Caching lexers causes lexer tables to be constructed once and reused
-- during each HIGHLIGHT event. Additionally it allows to modify the lexer
-- used for syntax highlighting from Lua code.
lexers.load = (function()
	local load, cache = lexers.load, {}
	return function(...)
		local lexer
		for _, name in ipairs({...}) do
			if cache and cache[name] then
				return cache[name]
			else
				-- scintillua modifies their import path like this:
				-- local path = M.property['scintillua.lexers']:gsub(';', '/?.lua;') .. '/?.lua'
				-- which is why we need to preempt it:
				local path = package.path
				package.path = package.path:gsub('%?%.lua', 'scintillua/lexers')
				lexer = load(name)
				package.path = path
			end

			if lexer and cache then
				cache[name] = lexer
				return lexer
			elseif lexer then
				return lexer
			end
		end
	end
end)()

---
-- @type Window

--- The file type associated with this window.
-- @tfield string syntax the syntax lexer name or `nil` if unset

--- Change syntax lexer to use for this window
-- @function set_syntax
-- @tparam string syntax the syntax lexer name or `nil` to disable syntax highlighting
-- @treturn bool whether the lexer could be changed
vis.types.window.set_syntax = function(win, syntax)
	if syntax == nil or syntax == '' or syntax == 'off' then
		win.syntax = nil
		return true
	end

	win.syntax = syntax

	local theme
	if win.theme == nil then
		theme = vis.ui.theme
		win.theme = vis.ui.theme
	else
		theme = win.theme
	end

	if theme == nil then
		return
	end
	local tokens = theme.tokens

	win:style_define(win.STYLE_DEFAULT,           tokens.DEFAULT           or '')
	win:style_define(win.STYLE_CURSOR,            tokens.CURSOR            or '')
	win:style_define(win.STYLE_CURSOR_PRIMARY,    tokens.CURSOR_PRIMARY    or '')
	win:style_define(win.STYLE_CURSOR_LINE,       tokens.CURSOR_LINE       or '')
	win:style_define(win.STYLE_SELECTION,         tokens.SELECTION         or '')
	win:style_define(win.STYLE_LINENUMBER,        tokens.LINENUMBER        or '')
	win:style_define(win.STYLE_LINENUMBER_CURSOR, tokens.LINENUMBER_CURSOR or '')
	win:style_define(win.STYLE_COLOR_COLUMN,      tokens.COLOR_COLUMN      or '')
	win:style_define(win.STYLE_STATUS,            tokens.STATUS            or '')
	win:style_define(win.STYLE_STATUS_FOCUSED,    tokens.STATUS_FOCUSED    or '')
	win:style_define(win.STYLE_SEPARATOR,         tokens.SEPARATOR         or '')
	win:style_define(win.STYLE_INFO,              tokens.INFO              or '')
	win:style_define(win.STYLE_EOF,               tokens.EOF               or '')

	if not lexers.load then
		return false
	end
	local lexer = lexers.load(syntax)
	if not lexer then
		return false
	end

	theme.raster = {}
	theme.styles = {}

	local  i = 1
	--local default = 1
	--theme.styles["default"] = ''
	--win:style_define(i, '')
	--i = i + 1

	for token, style in pairs(tokens) do
		-- styles: table of tokens -> style strings
		-- whitespace -> "#00000,none", function -> "#00FF00,fore:blue", variable -> "fore:red,bold"
		token = token:lower()
		theme.styles[token] = style

		win:style_define(i, style)

		-- styles: table of tokens -> color handles(0, 1, 2, 3)
		-- whitespace -> 1, function -> 2, variable -> 3
		theme.raster[token] = i
		i = i + 1
	end

	return true
end

vis:option_register("theme", "string", function(name)
	if name == nil then
		return
	end

	local path = 'vis/themes/'..name
	package.loaded[path] = nil
	local theme = {}

	-- tokens: function -> fore:red,bold
	-- colors: function -> fore:red,bold
	theme.tokens, theme.colors = table.unpack(require(path) or {})


	--if not theme.property then theme.lexer = theme.load("text") end
	--local default_colors = { "black", "red", "green", "yellow", "blue", "magenta", "cyan", "white" }
	--for _, c in ipairs(default_colors) do
	--	if not theme.colors[c] or theme.colors[c] == '' then
	--		--theme.colors[c] = c
	--	end
	--end

	vis.ui.theme = theme

	for win in vis:windows() do
		win.theme = theme
		win:set_syntax(win.file and win.file.syntax)
	end
	return true
end, "Color theme to use, filename without extension")

vis:option_register("syntax", "string", function(name)
	if not vis.win then return false end
	if not vis.win:set_syntax(name) then
		vis:info(string.format("Unknown syntax definition: `%s'", name))
		return false
	end
	return true
end, "Syntax highlighting lexer to use")

vis.events.subscribe(vis.events.INIT, function()
	if os.getenv("TERM_PROGRAM") == "Apple_Terminal" then
		vis:command("set change-256colors false");
	end
	vis:command("set theme ".. (vis.ui.colors <= 16 and "default-16" or "default-256"))
end)

vis.events.subscribe(vis.events.FILE_OPEN, function(file)
	if file.name == nil then
		return
	end

	local type = lexers.detect(file.name, file.lines[0])
	if type == nil then
		return
	end

	file.syntax = type
end)

vis.events.subscribe(vis.events.WIN_OPEN, function(win)
	if win.file == nil or win.file.syntax == nil then
		return
	end
	win:set_syntax(win.file.syntax)
end)

local function min(a, b)
	if a < b then
		return a
	else
		return b
	end
end

vis.events.subscribe(vis.events.WIN_HIGHLIGHT, function(win)
	local theme = win.theme or vis.ui.theme
	if not win.viewport or not win.syntax or not theme then
		return
	end

	local raster = theme.raster
	local styles = theme.styles

	if not raster or not styles then
		return
	end

	local lexer = lexers.load(win.syntax)
	if not lexer then
		return
	end


	-- TODO: improve heuristic for initial style
	local viewport = win.viewport
	local horizon = min(viewport.start, win.horizon or 32768)

	local view_start = viewport.start
	local lex_start = viewport.start - horizon
	viewport.start = lex_start
	local data = win.file:content(viewport)
	local tokens = lexer:lex(data, 1)
	local token_end = lex_start + (tokens[#tokens] or 1) - 1

	for i = #tokens - 1, 1, -2 do
		local token_start = lex_start + (tokens[i-1] or 1) - 1
		if token_end < view_start then
			break
		end
		local token = tokens[i] -- whitespace
		local handle = raster[token] -- 0, 1, 2 - the color handle from curses
		local count = 1
		while not handle and token ~= '' and count > 0 do
			token, count = token:gsub('%.[^.]+$', '')
			handle = raster[token]
			if handle ~= nil then
				raster[tokens[i]] = raster[token]
				styles[tokens[i]] = styles[token]
			end
		end
		if handle ~= nil then
			win:style(handle, token_start, token_end)
		end
		token_end = token_start - 1
	end
end)

return lexers
