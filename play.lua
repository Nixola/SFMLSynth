#!/usr/bin/env lua
local args = {...}
local filename = args[1]
local f = io.open(filename, 'r')

local r = f:read '*a'

f:close()

r = r:gsub("//.-\n", "")
r = r:gsub("*p*", 1000)

local command = './main'
local tot = ''

for i, v in r:gmatch("(%d+) (%d+)") do

	if i == "1000" then i = "-1000" end

	local s = command .. ' ' .. i .. ' ' .. v .. ' ; '
	tot = tot .. s

end

tot = tot:sub(1, -4)

if args[2] == 'print' then
	print(tot)
else
	os.execute(tot)
end