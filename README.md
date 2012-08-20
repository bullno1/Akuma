Akuma Simulator
===============

How to build
------------

* Get the dependencies:
	- [SDL](http://www.libsdl.org/)
	- [Boost](http://www.boost.org) with *boost.filesystem* built
* Setup your environment:
	- MOAI\_INCLUDE must point to the location of moai headers(e.g: moai-dev/src)
	- MOAI\_LIB must point to the location of moai libraries(e.g: moai-dev/vs2010/bin/Win32/Release)
	- Copy Moai's lua library (from moai-dev/vs2010/lua-lib/Release) to the "deps" folder and rename it moai-lua
* Use [premake4](http://industriousone.com/premake/download) to generate a suitable project file or build script.

Usage
-----

To start the simulator:

		Akuma <profile> <path to main.lua>

A list of existing profiles can be found in bin/profiles

Environment variables:

* AKUMA\_DOCUMENT\_DIR: MOAIEnvironment.documentDirectory will have the value of this variable.

To intercept file changes:

Define Akuma.onFileEvent(directory, file, event). This function can return a boolean value to indicate whether the Simulator
needs to restart. See samples/fileEvent/ for more info.

During simulation:

* Press Ctrl+R to restart
* The simulator will automatically restarts if change to the project folder is detected
* To simulate multitouch, hold down Ctrl before you release you left mouse button to create a "sticky" touch. Click the touch again to move it.
