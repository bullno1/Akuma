require "arc"

arc.start()

local viewWidth, viewHeight = MOAIGfxDevice.getViewSize()
viewport = MOAIViewport.new ()
viewport:setSize ( viewWidth, viewHeight )
viewport:setScale ( viewWidth, viewHeight )

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
prop:moveRot ( 360 * 6, 1.5 * 10 )
