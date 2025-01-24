local function paths()
	local ep  = os.getenv("PATH")

	-- package.config is a string - the first line of this is the OS path seperator
	-- '/', for example
	local ps  = package.config:sub(1, 1) -- path seperator

	-- we use this to determine:
	-- - the platform
	-- - the seperator for the PATH variable
	local d   = ps == "/" and ":" or ";"

	-- the following is an iterator which uses the path & seperator provided above
	--
	-- a path like "/usr/local/bin:/usr/bin"
	-- will turn to
	-- - /usr/local/bin
	-- - /usr/bin
	--
	return function()
		local b, e, r
                if ep == nil then
                        return nil
                end
		if d == "\\" and ep:sub(1, 1) == '"' then
			-- on windows, the PATH doesn't have escapes, instead,
			-- it wraps the FS entry in double-quotes
			--
			-- for example:
			--  C:\\"I\have\a;in\my\path";C\\:Users\...
			--
	                b, e = ep:find("[^\\]\";") -- UNTESTED
		else
	                b, e = ep:find("[^\\]" .. d)
		end
		if e == nil or b == 1 then
			r = ep
			ep = nil
		else
			r = ep:sub(1, e-1)
	                ep = ep:sub(e+1)
		end
                return r
        end
end

local function dirname(path)
	local ps  = package.config:sub(1, 1) -- path seperator
	return path:match("(.*[^"..ps.."])"..ps)
end

--[[
local i = 1
local function assert_eq(a, b)
	if a ~= b then
		error(string.format("%d: %s != %s", i, tostring(a), tostring(b)))
	end
	i = i +  1
end

assert_eq(dirname("./."), ".")
assert_eq(dirname("/home/home"), "/home")
assert_eq(dirname("/home/"), "/")
assert_eq(dirname("/home///."), "/home")
assert_eq(dirname("/"), "/")
assert_eq(dirname("./"), ".")
--]]

local function contains(haystack, needle)
	for v in haystack do
		if v == needle then
			return true
		end
	end
	return false
end

-- create a backup of the package.path
--
-- this is relavent if we're not able to load
-- the rc file from the path of the vis exe
--
local backup = package.path
local dir = dirname(vis.args[0])

if dir ~= nil and not contains(paths(), dir) then
	-- if VIS isn't invoked from $PATH, then include the plugins in the
	-- relative directory from which vis was called
	--
	-- ../editors/vis -> include ../editors/vis/

	local path = dir .. "/lua"

	package.path = string.format(
		"%s/?.lua;%s/?/init.lua",
		path,
		path
	)

	local fn

	-- <for testing>
	-- load the local rc.lua - ideally, this should be done
	-- through a cli arg
	fn = loadfile("./rc.lua")
	-- </for testing>

	if not fn then
		fn = loadfile(path .. "/rc.lua")
	end

	-- if we're able to load lua/vis/rc.lua from the same directory
	-- from which the vis executable was invoked,
	-- then we will assume that it's a development repo
	--
	-- if it's a development repo, only load plugins from that directory
	if fn then
		local status, err = pcall(fn)
		if not status then
			-- error occured
			vis:message(string.format(
				"error initializing from vis repository:\n\n%s\n", err
			))
			vis:exit(1)
		end
		return
	end
end

-- now that we're certain this isn't a development version of vis,
-- we'll load VIS_USER_PATH, or VIS_PATH, or the default paths described
-- in the man page

-- VIS_PATH_LUA takes precedence over all else
local vp = os.getenv("VIS_PATH_LUA")

if vp then
	package.path = vp
	require("vis/rc")
	return
end

vp = os.getenv("VIS_PATH")
package.path = backup

-- paths in increasing order of priority
local prefixes = {
	"/usr/share/vis",
	"/usr/local/share/vis",
	"/etc/vis",
}

local config = os.getenv("XDG_CONFIG_HOME")
if config then
	table.insert(prefixes, config.."/vis")
else
	local home = os.getenv("HOME")
	if home then
		table.insert(prefixes, home..".config/vis")
	end
end

if vp then
	table.insert(prefixes, vp)
end


for _, path in ipairs(prefixes) do
	package.path = string.format(
		"%s/?.lua;%s/?/init.lua;%s",
		path,
		path,
		package.path
	)
end

for i = #prefixes, 1, -1 do
	local path = prefixes[i] .. "/rc.lua"
	local fn = loadfile(path)
	if fn then
		vis.rc = path
		fn()
		return
	end
end

vis:info("WARNING: failed to load visrc")
