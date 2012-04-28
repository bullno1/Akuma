viewport = MOAIViewport.new ()
function a()
	viewport:cause_panic()
end
function b()
	a()
end
b()
