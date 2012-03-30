viewport = MOAIViewport.new ()
viewport:setSize ( MOAIEnvironment.screenWidth, MOAIEnvironment.screenHeight )
viewport:setScale ( 320, 480 )

layer = MOAILayer2D.new ()
layer:setViewport ( viewport )
MOAISim.pushRenderPass ( layer )

print(MOAIEnvironment.getDocumentDirectory())
print("Refer to run.bat to see how document directory is set")
