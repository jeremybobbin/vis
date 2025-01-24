-- load standard vis module, providing parts of the Lua API
--require('vis')

-- default plugins
require('vis')
require('vis/digraph')
require('vis/completion')
require('vis/textobject-lexer')
require('vis/scintillua')
require('vis/increment')
require('vis/status-bar')

vis.events.subscribe(vis.events.INIT, function()
	-- Your global configuration options
end)


vis.events.subscribe(vis.events.WIN_OPEN, function(win) -- luacheck: no unused args
	-- Your per window configuration options e.g.
	-- vis:command('set number')
end)
