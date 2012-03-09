local viewport = MOAIViewport.new ()
local devWidth, devHeight = MOAIEnvironment.getViewSize()
viewport:setSize (devWidth, devHeight)
viewport:setScale (devWidth, devHeight)

local layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

local touchDeck = MOAIScriptDeck.new()
local touchSensor = MOAIInputMgr.device.touch
touchDeck:setDrawCallback(function(index, xOff, yOff, xScale, yScale)
	for _, id in ipairs{touchSensor:getActiveTouches()} do
		local x, y = touchSensor:getTouch(id)
		MOAIGfxDevice.setPenColor(1, 1, 1, 0.75)
		MOAIDraw.fillCircle (x, y, 10, 10)
	end
end)
touchDeck:setRect(-devWidth, -devHeight, devWidth, devHeight)

local prop = MOAIProp2D.new()
prop:setDeck(touchDeck)
layer:insertProp(prop)
prop:setLoc(layer:wndToWorld(0,0))
prop:setScl(1, -1)
prop:setBlendMode(MOAIProp2D.BLEND_ADD)
