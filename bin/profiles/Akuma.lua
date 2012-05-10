local trace = (require "StackTracePlus").stacktrace
MOAISim.setTraceback(function(...)
	print(trace(...))
end)

Akuma = {}

function Akuma.setupEnvironment(params)
	-- Override environment info
	for k, v in pairs(params) do
		MOAIEnvironment[k] = v
	end

	MOAIEnvironment.documentDirectory = os.getenv("AKUMA_DOCUMENT_DIR")

	if params.screenWidth and params.screenHeight then
		local title = params.devName or "Akuma"
		MOAISim.openWindow(title, params.screenWidth, params.screenHeight)
	else
		error("Device profile does not provide screenWidth and screenHeight")
	end
end
