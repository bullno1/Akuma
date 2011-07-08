----------------------------------------------------------------
-- Copyright (c) 2010-2011 Zipline Games, Inc.
-- All Rights Reserved.
-- http://getmoai.com
----------------------------------------------------------------

viewport = MOAIViewport.new ()
viewport:setSize ( MOAIEnvironment.getViewSize() )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

gfxQuad = MOAIGfxQuad2D.new ()
gfxQuad:setTexture ( "cathead.png" )
gfxQuad:setRect ( -64, -64, 64, 64 )
gfxQuad:setUVRect ( 1, 1, 0, 0)

prop = MOAIProp2D.new ()
prop:setDeck ( gfxQuad )
layer:insertProp ( prop )
prop:moveRot ( 360 * 3, 1.5 * 10 )

print(MOAIEnvironment.OS_BRAND_ANDROID)
print(MOAIEnvironment.OS_BRAND_IOS)

local sensor = MOAITouchSensor.new()
sensor:setCallback(function(...)
	print("a")
	print(...)
end)

local keyboardSensor = MOAIKeyboardSensor.new()
keyboardSensor:setCallback(function(...)
	print(...)
end)

MOAIThread.new():run(function()
	while true do
		if(sensor:up()) then
			print "up"
		end
		coroutine.yield()
	end
end)

MOAIInputMgr.device.touch:setCallback(function(...)
	print(...)
end)

MOAIInputMgr.device.keyboard:setCallback(function(key)
	--print(string.char(key))
end)
