Akuma Simulator
===============

How to build
------------

* Get the dependencies:
	- [SDL](http://www.libsdl.org/)
	- [Boost](http://www.boost.org) with *boost.filesystem* built
* Copy Moai's lua-5.1.3.lib to deps. (e.g: if you use VS2010 it will be in MoaiSDKRoot/vs2010/lua-lib/Release)
* Use [premake4](http://industriousone.com/premake/download) to generate a suitable project file or build script.

Usage
-----

Akuma &lt;profile&gt; &lt;path to main.lua&gt;
A list of existing profiles can be found in bin/profiles

Environment variables:

* AKUMA_DOCUMENT_DIR: MOAIEnvironment.documentDirectory will have the value of this variable.


During simulation:

* Press Ctrl+R to restart
* The simulator will automatically restarts if change to the project folder is detected
* To simulate multitouch, hold down Ctrl before you release you left mouse button to create a "sticky" touch. Click the touch again to move it.
