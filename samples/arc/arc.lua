module("arc", package.seeall)

--Modules
local copas = require "copas"
local socket = require "socket"
--Constants
local EQUAL = ("="):byte()

--Convert a list of parameters to a string, separated with tab
local function paramsToString(...)
	local result = {}
	for _, v in ipairs(arg) do
		table.insert(result, tostring(v))
	end
	return table.concat(result, "\t")
end

local function consoleHandler(skt)
	skt = copas.wrap(skt)
	--Greet user
	skt:send("Welcome to Akuma Remote Console\n>")

	local env = {}
	--Override print to send output to remote end
	local outputBuff = {}
	local printed = false
	function env.print(...)
		--buffer print output since lua 5.1 does not allow yield accross metamethod
		printed = true
		table.insert(outputBuff, paramsToString(...))
	end
	--Connect to global environment
	setmetatable(env, {__index = _G})

	while true do
		local data, err = skt:receive()
		--in case of quit or closed connection
		if data == "quit" or err == "closed" then
			break
		end
		if data then
			--if it begins with = , replace it with return
			if data:byte(1) == EQUAL then
				data = "return "..data:sub(2)
			end
			--compile the string
			local func, err = loadstring(data, "remote command")
			if func then
				setfenv(func, env)
				local ret = {xpcall(func, debug.traceback)}
				local status = ret[1]
				table.remove(ret, 1)--remove status code
				if status then--no error
					--send return value
					skt:send(paramsToString(unpack(ret)))
				else
					--send error
					skt:send(ret[1])
				end
			else
				skt:send(err)
			end
			--flush output buffer
			if printed then
				skt:send(table.concat(outputBuff, "\n"))
				printed = false
				outputBuff = {}
			end
			--send prompt
			skt:send("\n>")
		end
	end
end

function start(port)
	port = port or 9090

	local serverSkt = socket.tcp()
	serverSkt:bind('*', port)
	serverSkt:listen(1)
	copas.addserver(serverSkt, consoleHandler, 0)
	local updateThread = MOAICoroutine.new()
	updateThread:run(function()
		while true do
			copas.step(0)
			coroutine.yield()
		end
	end)
end
