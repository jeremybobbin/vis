-- Solarized color codes Copyright (c) 2011 Ethan Schoonover
local colors = {
	 base03  = '#002b36', -- #002b36 dark cyan
	 base02  = '#073642', -- #073642 darker cyan
	 blue    = 'blue',    -- #268bd2
	 cyan    = 'cyan',    -- #2aa198
	 base01  = '#586e75', -- #586e75 grey blue tinge
	 base00  = '#657b83', -- #657b83 dark grey blue tinge
	 violet  = 'violet',  -- #6c71c4
	 base0   = '#839496', -- #839496 light-grey - cyan tinge
	 green   = 'green',   -- #859900
	 base1   = '#93a1a1', -- #93a1a1 lighter-grey - cyan tinge
	 yellow  = 'yellow',  -- #b58900
	 orange  = 'orange',  -- #cb4b16
	 magenta = 'magenta', -- #d33682
	 red     = 'red',     -- #dc322f
	 base2   = '#eee8d5', -- #eee8d5 very light yellow - orange tinge
	 base3   = '#fdf6e3', -- #fdf6e3 lighter very light yellow - orange tinge
}

colors.base03 = "black"
colors.base02 = "black"

colors.base01 = "red"
colors.base00 = "red"

colors.base0 = "green"
colors.base1 = "blue"

colors.base2 = "yellow"
colors.base3 = "yellow"

-- dark
local fg = 'fore:'..colors.base0..','
local bg = 'back:'..colors.base03..','
-- light
-- local fg = ',fore:'..colors.base03..','
-- local bg = ',back:'..colors.base3..','
-- solarized term
-- local fg = ',fore:default,'
-- local bg = ',back:default,'


local s = {
	DEFAULT = bg..fg,
	NOTHING = bg,
	CLASS = 'fore:yellow',
	COMMENT = 'fore:'..colors.base01,
	CONSTANT = 'fore:'..colors.cyan,
	DEFINITION = 'fore:'..colors.blue,
	ERROR = 'fore:'..colors.red..',italics',
	FUNCTION = 'fore:'..colors.blue,
	KEYWORD = 'fore:'..colors.green,
	LABEL = 'fore:'..colors.green,
	NUMBER = 'fore:'..colors.cyan,
	OPERATOR = 'fore:'..colors.green,
	REGEX = 'fore:green',
	STRING = 'fore:'..colors.cyan,
	PREPROCESSOR = 'fore:'..colors.orange,
	TAG = 'fore:'..colors.red,
	TYPE = 'fore:'..colors.yellow,
	VARIABLE = 'fore:'..colors.blue,
	WHITESPACE = 'fore:'..colors.base01,
	EMBEDDED = 'back:blue',
	IDENTIFIER = fg,

	LINENUMBER = 'fore:'..colors.base00..',back:'..colors.base02,
	LINENUMBER_CURSOR = 'back:'..colors.base00..',fore:'..colors.base02,
	CURSOR = 'fore:'..colors.base03..',back:'..colors.base0,
	CURSOR_PRIMARY = 'fore:'..colors.base03..',back:'..colors.base0,
	CURSOR_LINE = 'back:'..colors.base02,
	COLOR_COLUMN = 'back:'..colors.base02,
	SELECTION = 'back:'..colors.base02,
	STATUS = 'back:'..colors.base00..',fore:'..colors.base02,
	STATUS_FOCUSED = 'back:'..colors.base1..',fore:'..colors.base02,
	INFO = 'fore:default,back:default,bold',
	EOF = 'fore:'..colors.base01,
	ATTRIBUTE = 'fore:green,bold',

	-- lexer specific styles

}

s.SEPARATOR = s.DEFAULT

-- Diff
s.ADDITION = 'fore:green'
s.DELETION = 'fore:red'
s.CHANGE = 'fore:yellow'

-- CSS
s.PROPERTY = s.ATTRIBUTE
s.PSEUDOCLASS = ''
s.PSEUDOELEMENT = ''

-- HTML
s.TAG_UNKNOWN = s.TAG .. ',italics'
s.TAG_SINGLE = s.TAG
s.TAG_DOCTYPE = s.TAG .. ',bold'
s.ATTRIBUTE_UNKNOWN = s.ATTRIBUTE .. ',italics'

-- Latex, TeX, and Texinfo
s.COMMAND = KEYWORD
s.COMMAND_SECTION = s.CLASS
s.ENVIRONMENT = TYPE
s.ENVIRONMENT_MATH = s.NUMBER

-- Makefile
s.TARGET = ''

-- Markdown
s.HR = ''
s.BOLD = 'bold'
s.ITALIC = 'italics'
s.LIST = s.KEYWORD
s.LINK = s.KEYWORD
s.REFERENCE = s.KEYWORD
s.CODE = s.EMBEDDED

-- Output
s.STYE_FILENAME = ''
s.LINE = ''
s.COLUMN = ''
s.MESSAGE = ''

-- Python
s.KEYWORD_SOFT = ''

-- Taskpaper
s.NOTE = ''
s.TAG_EXTENDED = ''
s.TAG_DAY = 'fore:yellow'
s.TAG_OVERDUE = 'fore:red'
s.TAG_PLAIN = ''

-- XML
s.CDATA = ''

-- YAML
s.ERROR_INDENT = 'back:red'

-- The following are temporary s until their legacy lexers are migrated.

-- Antlr
s.ACTION = ''

-- Clojure
s.CLOJURE_KEYWORD = s.TYPE
s.CLOJURE_SYMBOL = s.TYPE .. ',bold'

-- Crystal
--SYMBOL = STRING

-- Gleam
s.MODULE = s.CONSTANT
s.DISCARD = s.COMMENT

-- Icon
s.SPECIAL_KEYWORD = TYPE

-- jq
s.FORMAT = s.CONSTANT
s.SYSVAR = s.CONSTANT .. ',bold'

-- Julia
-- SYMBOL = STRING
s.CHARACTER = s.CONSTANT

-- Mediawiki
s.BEHAVIOR_SWITCH = s.KEYWORD

-- Moonscript
s.TBL_KEY = s.REGEX
s.SELF_REF = s.LABEL
s.PROPER_IDENT = s.CLASS
s.FNDEF = s.PREPROCESSOR
-- SYMBOL = EMBEDDED

-- reST
s.LITERAL_BLOCK = s.EMBEDDED
s.FOOTNOTE_BLOCK = s.LABEL
s.CITATION_BLOCK = s.LABEL
s.LINK_BLOCK = s.LABEL
s.CODE_BLOCK = s.CODE
s.DIRECTIVE = s.KEYWORD
s.SPHINX_DIRECTIVE = s.KEYWORD
s.UNKNOWN_DIRECTIVE = s.KEYWORD
s.SUBSTITUTION = s.VARIABLE
s.INLINE_LITERAL = s.EMBEDDED
s.ROLE = s.CLASS
s.INTERPRETED = s.STRING

-- txt2tags
s.LINE = 'bold'
s.IMAGE = 'fore:green'
s.STRIKE = 'italics'
s.TAGGED = s.EMBEDDED
s.TAGGED_AREA = s.EMBEDDED
s.TABLE_SEP = 'fore:green'
s.HEADER_CELL_CONTENT = 'fore:green'

for i = 1,5 do
	s['H'..i] = s.HEADING
end

for i = 1,6 do
	s['HEADING_H'..i] = s.HEADING
end

return {s, colors}
