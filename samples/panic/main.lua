viewport = MOAIViewport.new ()
function a()
	local d = 2
	viewport:cause_panic()
end
function b()
	local b = 1
	a()
end
b()
