-- std = "min"
globals = { "vis" }
include_files = { "lua/*.lua", "lua/**/*.lua", "test/lua/*.lua" }
exclude_files = { "test/lua/*.lua", "lua/scintillua/**/*.lua" }
files["test/lua"] = { std = "+busted" }
