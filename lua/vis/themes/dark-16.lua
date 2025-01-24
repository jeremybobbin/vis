-- Eight-color scheme
local t = {
	DEFAULT ='back:black,fore:white',
	NOTHING = 'back:black',
	CLASS = 'fore:yellow,bold',
	COMMENT = 'fore:blue,bold',
	CONSTANT = 'fore:cyan,bold',
	DEFINITION = 'fore:blue,bold',
	ERROR = 'fore:red,italics',
	FUNCTION = 'fore:blue,bold',
	KEYWORD = 'fore:yellow,bold',
	LABEL = 'fore:green,bold',
	NUMBER = 'fore:red,bold',
	OPERATOR = 'fore:cyan,bold',
	REGEX = 'fore:green,bold',
	STRING = 'fore:red,bold',
	PREPROCESSOR = 'fore:magenta,bold',
	TAG = 'fore:red,bold',
	TYPE = 'fore:green,bold',
	VARIABLE = 'fore:blue,bold',
	WHITESPACE = '',
	EMBEDDED = 'back:blue,bold',
	IDENTIFIER = 'fore:white',
	LINENUMBER = 'fore:white',
	CURSOR = 'reverse',
	CURSOR_LINE = 'underlined',
	COLOR_COLUMN = 'back:red',
	SELECTION = 'back:white',
	STATUS = 'reverse',
	STATUS_FOCUSED = 'reverse,bold',
	INFO = 'fore:default,back:default,bold',
	EOF = '',
}

t.LINENUMBER_CURSOR = t.LINENUMBER
t.CURSOR_PRIMARY = t.CURSOR
t.SEPERATOR = t.DEFAULT

return {t}
