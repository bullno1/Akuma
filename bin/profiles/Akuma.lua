module("Akuma", package.seeall)

local env = MOAIEnvironment
local newEnv = newproxy(true)
local overloadedEnv = setmetatable({}, {__index = env})
getmetatable(newEnv).__index = overloadedEnv
_G.MOAIEnvironment = newEnv

local function overload(funcName, retVal)
	if retVal ~= nil then
		overloadedEnv[funcName] = function()
			return retVal
		end
	end
end

function setupEnvironment(params)
	overload("getDevBrand", params.devBrand)
	overload("getDevName", params.devName)
	overload("getDevManufacturer", params.devManufacturer)
	overload("getDevModel", params.devModel)
	overload("getDevProduct", params.devProduct)
	overload("getOSBrand", params.OSBrand)
	overload("getOSVersion", params.OSVersion)

	if params.width and params.height then
		local title = params.devName or "Akuma"
		MOAISim.openWindow(title, params.width, params.height)
	end
end
