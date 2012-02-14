#include <iostream>
#include <aku/AKU.h>
#include "Simulator.h"
#include <boost/filesystem.hpp>
#include <SDL.h>
#undef main

using namespace std;
namespace fs = boost::filesystem;

void simulationLoop(char* argv[])
{
	fs::path pathToExe = fs::system_complete(argv[0]);
	fs::path exeLocation(pathToExe.parent_path());
	fs::path profilePath(exeLocation / "profiles");
	fs::path pathToMain = fs::system_complete(argv[2]);

	while(true)
	{
		if(initSimulator(profilePath, argv[1]))
		{
			ExitReason::Enum exitReason = startSimulator(pathToMain);
			switch(exitReason)
			{
			case ExitReason::UserAction:
				return;
			case ExitReason::Error:
				cout << "Press enter to restart" << endl;
				cin.get();
				break;
			case ExitReason::Restart:
				cout << "Restarting simulator" << endl;
				break;
			}
		}
		else
		{		
			cout << "Error in intialization" << endl;
			cin.get();
			return;
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc!=3)
	{
		cout<<"Usage: Akuma <profile> <path to main.lua>"<<endl;
		return 0;
	}

	try
	{
		simulationLoop(argv);
	}
	catch(std::exception& e)
	{
		cout<<e.what()<<endl;
	}
	catch(...)
	{
		cout<<"Unknown error"<<endl;
	}
	
	AKUFinalize();

	return 0;
}