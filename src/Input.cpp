#include "Input.h"
#include <aku/AKU.h>
#include <vector>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

using namespace std;

namespace
{
	const int TOUCH_RADIUS = 10;
	const int TOUCH_RADIUS_SQUARED = TOUCH_RADIUS * TOUCH_RADIUS;

	struct Touch
	{
		int x, y;
		int id;
	};

	vector<int> freeIds;
	int lastTouchId = 0;

	int makeTouchId()
	{
		if(freeIds.size() != 0)
		{
			int touchId = freeIds.back();
			freeIds.pop_back();
			return touchId;
		}

		return ++lastTouchId;
	}

	void releaseTouchId(int id)
	{
		freeIds.push_back(id);
	}

	vector<Touch> touches;
	Touch* currentTouch;
	unsigned int touchIndex;

	inline int sqrLength(int x1, int y1, int x2, int y2)
	{
		int xDiff = x2 - x1;
		int yDiff = y2 - y1;
		return xDiff * xDiff + yDiff * yDiff;
	}
	
	inline void handleMouseDown(int x, int y)
	{
		//First, check if there is a touch at that point
		for(unsigned int i = 0; i < touches.size(); ++i)
		{
			Touch& touch = touches[i];

			if(sqrLength(x, y, touch.x, touch.y) < TOUCH_RADIUS_SQUARED)
			{
				currentTouch = &touch;
				touchIndex = i;

				return;
			}
		}
		
		int newTouchId = makeTouchId();
		Touch newTouch = {x, y, newTouchId};
		touchIndex = touches.size();
		touches.push_back(newTouch);
		currentTouch = &(touches.back());

		AKUEnqueueTouchEvent(
			InputDevice::Device,
			InputSensor::Touch,
			newTouchId,
			true,
			static_cast<float>(x),
			static_cast<float>(y),
			1
		);
	}

	inline void handleMouseUp(int x, int y, bool isCtrlDown)
	{
		if(currentTouch != 0)
		{
			if(!isCtrlDown)//remove this touch
			{
				int touchId = currentTouch->id;

				AKUEnqueueTouchEvent(
					InputDevice::Device,
					InputSensor::Touch,
					touchId,
					false,
					static_cast<float>(x),
					static_cast<float>(y),
					1
				);

				releaseTouchId(touchId);
				touches[touchIndex] = touches.back();
				touches.pop_back();

				//printf("Removed %d\n", touchId);
			}			

			currentTouch = 0;
		}
	}

	inline void handleMouseMove(int x, int y)
	{
		if(currentTouch == 0)
			return;

		currentTouch->x = x;
		currentTouch->y = y;

		AKUEnqueueTouchEvent(
			InputDevice::Device,
			InputSensor::Touch,
			currentTouch->id,
			true,
			static_cast<float>(x),
			static_cast<float>(y),
			1
		);
	}

	void clearTouches()
	{
		lastTouchId = 0;
		touches.clear();
		currentTouch = 0;
	}
}

void initInput()
{
	AKUSetInputConfigurationName("simulator");

	AKUReserveInputDevices(InputDevice::Count);
	AKUSetInputDevice(InputDevice::Device, "device");
	AKUSetInputDeviceActive(InputDevice::Device, true);

	AKUReserveInputDeviceSensors(InputDevice::Device, InputSensor::Count);
	AKUSetInputDeviceButton(InputDevice::Device, InputSensor::HomeButton, "homeButton");
	AKUSetInputDeviceButton(InputDevice::Device, InputSensor::BackButton, "backButton");
	AKUSetInputDeviceButton(InputDevice::Device, InputSensor::MenuButton, "menuButton");
	AKUSetInputDeviceButton(InputDevice::Device, InputSensor::SearchButton, "searchButton");
	AKUSetInputDeviceCompass(InputDevice::Device, InputSensor::Compass, "compass");
	AKUSetInputDeviceKeyboard(InputDevice::Device, InputSensor::Keyboard, "keyboard");
	AKUSetInputDeviceLevel(InputDevice::Device, InputSensor::Accelerometer, "level");
	AKUSetInputDeviceTouch(InputDevice::Device, InputSensor::Touch, "touch");

	clearTouches();
}

void injectInput(const SDL_Event& ev)
{
	switch(ev.type)
	{
	case SDL_KEYDOWN:
		/*if(ev.Key.Code < 127)
			AKUEnqueueKeyboardEvent(InputDevice::Device, InputSensor::Keyboard, ev.Key.Code, true);*/
		break;
	case SDL_KEYUP:
		/*if(ev.Key.Code < 127)
			AKUEnqueueKeyboardEvent(InputDevice::Device, InputSensor::Keyboard, ev.Key.Code, false);*/
		break;
	case SDL_MOUSEBUTTONDOWN:
		if(ev.button.button == SDL_BUTTON_LEFT)
			handleMouseDown(ev.button.x, ev.button.y);
		break;
	case SDL_MOUSEBUTTONUP:
		if(ev.button.button == SDL_BUTTON_LEFT)
			handleMouseUp(
				ev.button.x,
				ev.button.y,
				(SDL_GetModState() & KMOD_CTRL) != 0
			);
		break;
	case SDL_MOUSEMOTION:
		handleMouseMove(ev.motion.x, ev.motion.y);
		break;
	}
}