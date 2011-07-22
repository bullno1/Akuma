#ifndef _AKUMA_INPUT_H_
#define _AKUMA_INPUT_H_

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Input.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

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
void injectInput(const sf::Event& ev, const sf::Input& input);
void renderTouches(sf::RenderTarget& target);

#endif