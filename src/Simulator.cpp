#include "Simulator.h"
#include <iostream>
#include <string>
#include <aku/AKU.h>
#include <aku/AKU-luaext.h>
#include <aku/AKU-untz.h>
#include <AntTweakBar.h>
#include <SFML/Graphics.hpp>
#include "Utils.h"
#include "Input.h"
#include <FileWatcher/FileWatcher.h>

using namespace std;
using namespace sf;
namespace fs = boost::filesystem;

AKUContextID akuContext;
RenderWindow window;
ExitReason::Enum exitReason = ExitReason::Error;
FW::FileWatcher fw;

struct ProjectFolderWatchListener: public FW::FileWatchListener
{
	void handleFileAction(FW::WatchID watchid, const FW::String& dir, const FW::String& filename, FW::Action action)
	{
		dirty = true;
	}

	bool dirty;
};

ProjectFolderWatchListener projListener;

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
	if(window.IsOpened())
		return;

	window.Create(VideoMode((unsigned int)width, (unsigned int)height), title, Style::Titlebar | Style::Close);
	window.SetFramerateLimit(60);

	TwInit(TW_OPENGL, 0);
	TwWindowSize(width, height);
}

void closeWindow()
{
	if(window.IsOpened())
	{
		TwTerminate();
		window.Close();
	}	
}

void startGameLoop()
{
	if(!window.IsOpened())
		return;

	while(true)
	{
		Event ev;
		while(window.PollEvent(ev))
		{
			if(!TwEventSFML(&ev, SFML_VERSION_MAJOR, SFML_VERSION_MINOR))
			{
				switch(ev.Type)
				{
				case Event::Closed:
					closeWindow();
					exitReason = ExitReason::UserAction;
					return;
				case Event::KeyPressed:
					if(ev.Key.Control && ev.Key.Code == Key::R)
					{
						closeWindow();
						exitReason = ExitReason::Restart;
						return;
					}
					break;
				}

				injectInput(ev);
			}
		}

		fw.update();
		if(!projListener.dirty)
		{
			AKUUpdate();
			window.Clear();
			AKURender();
			TwDraw();
			window.Display();
			continue;
		}
		else
		{
			cout << "Change to project folder detected" << endl;
			while(true)//delay reload in case of a lot of changes
			{
				projListener.dirty = false;
				sf::Sleep(100);
				fw.update();
				if(!projListener.dirty)
					break;
			}
			exitReason = ExitReason::Restart;
			return;
		}
	}
}

bool initSimulator(const fs::path& profilePath, const char* profile)
{
	writeSeparator();
	cout << "Initializing AKU" << endl;

	akuContext = AKUCreateContext();
	//Load extensions
	AKUExtLoadLuacrypto();
	AKUExtLoadLuacurl();
	AKUExtLoadLuasocket();
	AKUExtLoadLuasql();
	//Load untz
	//AKUUntzInit();
	initInput();

	AKUSetFunc_EnterFullscreenMode(&enterFullscreenMode);
	AKUSetFunc_ExitFullscreenMode(&exitFullscreenMode);
	AKUSetFunc_OpenWindow(&openWindow);

	try
	{
		fs::current_path(profilePath);
	}
	catch(std::exception& e)
	{
		cout	<< "Error changing directory to: " << profilePath << endl
				<< e.what() << endl;
		return false;
	}

	cout << "Loading profile " << profile << endl;
	AKURunScript((string(profile) + ".lua").c_str());

	writeSeparator();

	AKUSetFunc_StartGameLoop(&startGameLoop);
	
	return true;
}

ExitReason::Enum startSimulator(const boost::filesystem::path& pathToMain)
{
	exitReason = ExitReason::Error;
	fs::path filename = pathToMain.filename();
	fs::path projectDir = pathToMain.parent_path();
	//Change working directory to the app's directory
	fs::current_path(projectDir);
	//Start the watcher
	projListener.dirty = false;
	FW::WatchID watchID = fw.addWatch(projectDir.string(), &projListener);
	//start the main script
	AKURunScript(filename.string().c_str());

	fw.removeWatch(watchID);
	//Ensure that window is closed
	closeWindow();
	AKUDeleteContext(akuContext);

	return exitReason;
}