#include "Simulator.h"
#include <iostream>
#include <string>
#include <aku/AKU.h>
#include <aku/AKU-luaext.h>
#include <aku/AKU-untz.h>
#include <lua-headers/moai_lua.h>
#include <FileWatcher/FileWatcher.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_syswm.h>
#include "Utils.h"
#include "Input.h"
#include "lua.hpp"

using namespace std;
namespace fs = boost::filesystem;

namespace
{
	AKUContextID akuContext;
	FW::FileWatcher fw;
	Uint32 frameDelta = (Uint32)(1000.0 / 60.0);
	bool windowOpened = false;

	struct ProjectFolderWatchListener: public FW::FileWatchListener
	{
		void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action)
		{
			//consult the script before reporting this event
			lua_State* L = AKUGetLuaState();
			int top = lua_gettop(L);
			lua_getglobal(L, "Akuma");
			lua_getfield(L, -1, "onFileEvent");
			if(lua_isfunction(L, -1))
			{
				lua_pushstring(L, filename.c_str());
				lua_pushstring(L, dir.c_str());
				lua_pushnumber(L, (lua_Number)action);
				if(lua_pcall(L, 3, 1, 0) == 0)
				{
					dirty |= lua_toboolean(L, -1) != 0;
				}
				else
				{
					cout << "Error while reporting file change event" << endl;
					cout << lua_tostring(L, -1) << endl;
					dirty = true;
				}
			}
			else//no handler, just report this event
			{
				dirty = true;
			}
			lua_settop(L, top);
		}
	
		bool dirty;
	} projListener;
}

void enterFullscreenMode()
{
	cout << "Fullscreen mode is not supported" << endl;
}

void exitFullscreenMode()
{
	cout << "Fullscreen mode is not supported" << endl;
}

void openWindow(const char* title, int width, int height)
{
	if((SDL_WasInit(0) & (SDL_INIT_TIMER | SDL_INIT_VIDEO)) == 0)
	{
		SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	}

	SDL_SetVideoMode(width, height, 32, SDL_OPENGL);
	SDL_WM_SetCaption(title, 0);
#ifdef WIN32
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	if(SDL_GetWMInfo(&wmInfo) > 0)
	{
		HANDLE icon = LoadImage(GetModuleHandle(0), "mainIcon", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
		HWND windowHandle = wmInfo.window;
		SetClassLong(windowHandle, GCL_HICON, (LONG)icon);
		SetClassLong(windowHandle, GCL_HICONSM, (LONG)icon);
		SetConsoleTitle("Log");
	}
#endif
	AKUDetectGfxContext();
	AKUSetScreenSize(width, height);
	windowOpened = true;
}

void closeWindow()
{
	windowOpened = false;
	AKUReleaseGfxContext();
}

ExitReason::Enum startGameLoop()
{
	if(!windowOpened)
	{
		cout << "Rendering window was not initialized" << endl;
		return ExitReason::Error;
	}

	try
	{
		Uint32 lastFrame = SDL_GetTicks();
		while(true)
		{
			SDL_Event ev;
			while(SDL_PollEvent(&ev))
			{
				switch(ev.type)
				{
				case SDL_QUIT:
					closeWindow();
					return ExitReason::UserAction;
				case SDL_KEYDOWN:
					if(ev.key.keysym.sym == SDLK_r && (SDL_GetModState() & KMOD_CTRL))
					{
						closeWindow();
						return ExitReason::Restart;
					}
					break;
				}
	
				injectInput(ev);
			}
	
			fw.update();
			if(!projListener.dirty)
			{
				AKUUpdate();
				glClear(GL_COLOR_BUFFER_BIT);
				AKURender();
				SDL_GL_SwapBuffers();
	
				Uint32 currentFrame = SDL_GetTicks();
				Uint32 delta = currentFrame - lastFrame;
				if(delta < frameDelta)
					SDL_Delay(frameDelta - delta);
				lastFrame = SDL_GetTicks();
	
				continue;
			}
			else
			{
				cout << "Change to project folder detected" << endl;
				while(true)//delay reload in case of a lot of changes
				{
					projListener.dirty = false;
					SDL_Delay(100);
					fw.update();
					if(!projListener.dirty)
						break;
				}
				return ExitReason::Restart;
			}
		}
	}
	catch(std::exception& e)
	{
		cout << "Exception: " << e.what() << endl;
	}
	catch(...)
	{
		cout << "Unknown error" << endl;
		return ExitReason::Error;
	}

	return ExitReason::Error;
}

void initSimulator(const fs::path& profilePath, const char* profile)
{
	writeSeparator();
	cout << "Initializing AKU" << endl;

	akuContext = AKUCreateContext();
	//Load extensions
	AKUExtLoadLuacrypto();
	AKUExtLoadLuacurl();
	AKUExtLoadLuasocket();
	AKUExtLoadLuasql();
	AKUExtLoadLuafilesystem();
	//Load untz
	AKUUntzInit();

	initInput();

	AKUSetFunc_EnterFullscreenMode(&enterFullscreenMode);
	AKUSetFunc_ExitFullscreenMode(&exitFullscreenMode);
	AKUSetFunc_OpenWindow(&openWindow);

	//Load base scripts
	AKURunBytecode(moai_lua, moai_lua_SIZE);
	AKUSetWorkingDirectory(profilePath.string().c_str());
	AKURunScript("Akuma.lua");
	cout << "Loading profile " << profile << endl;
	AKURunScript((string(profile) + ".lua").c_str());
	writeSeparator();
}

ExitReason::Enum startSimulator(
	const boost::filesystem::path& profilePath,
	const char* profile,
	const boost::filesystem::path& pathToMain
)
{
	initSimulator(profilePath, profile);

	fs::path filename = pathToMain.filename();
	fs::path projectDir = pathToMain.parent_path();
	//change working directory
	AKUSetWorkingDirectory(projectDir.string().c_str());
	//run the main script
	AKURunScript(filename.string().c_str());
	//Start the watcher
	projListener.dirty = false;
	FW::WatchID watchID = fw.addWatch(projectDir.string(), &projListener, true);
	ExitReason::Enum exitReason = startGameLoop();
	fw.removeWatch(watchID);
	//Ensure that window is closed
	closeWindow();
	AKUDeleteContext(akuContext);

	return exitReason;
}