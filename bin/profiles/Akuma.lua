Akuma = {}

function Akuma.setupEnvironment(params)
	-- Override environment info
	for k, v in pairs(params) do
		MOAIEnvironment[k] = v
	end

	if params.screenWidth and params.screenHeight then
		local title = params.devName or "Akuma"
		MOAISim.openWindow(title, params.screenWidth, params.screenHeight)
	else
		error("Device profile does not provide screenWidth and screenHeight")
	end
end
