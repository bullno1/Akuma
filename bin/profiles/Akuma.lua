module("Akuma", package.seeall)

function setupEnvironment(params)
	local MOAIXEnvironment = MOAIEnvironment.extend(
		function(class, super)
			local function overload(funcName, retVal)
				if retVal ~= nil then
					class[funcName] = function()
						return retVal
					end
				end
			end

			overload("getDevBrand", params.devBrand)
			overload("getDevName", params.devName)
			overload("getDevManufacturer", params.devManufacturer)
			overload("getDevModel", params.devModel)
			overload("getDevProduct", params.devProduct)
			overload("getOSBrand", params.OSBrand)
			overload("getOSVersion", params.OSVersion)
			overload("getDocumentDirectory", os.getenv("AKUMA_DOCUMENT_DIR"))
		end
	)

	if params.width and params.height then
		local title = params.devName or "Akuma"
		MOAISim.openWindow(title, params.width, params.height)
	end

	_G.MOAIEnvironment = MOAIXEnvironment
end
