viewport = MOAIViewport.new ()
viewport:setSize ( MOAIEnvironment.screenWidth, MOAIEnvironment.screenHeight)
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
prop:moveRot ( 360 * 6, 1.5 * 10 )


function Akuma.onFileEvent(fileName, dir, action)
	print("Filename: ", fileName)
	print("Dir: ", dir)
	print("Action: ", action)
	print("--------------")
	return true
end
