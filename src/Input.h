#ifndef _AKUMA_INPUT_H_
#define _AKUMA_INPUT_H_

#include <SDL.h>

namespace InputDevice
{
	enum Enum
	{
		Device,

		Count
	};
}

namespace InputSensor
{
	enum Enum
	{
		Touch,
		Keyboard,
		Accelerometer,
		Gyroscope,
		Compass,
		Location,
		//Common buttons
		HomeButton,
		//Android buttons
		BackButton,
		SearchButton,
		MenuButton,

		Count
	};
}


void initInput();
void injectInput(const SDL_Event& ev);

#endif