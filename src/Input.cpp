#include "Input.h"
#include <aku/AKU.h>

using namespace sf;

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
	AKUSetInputDeviceLevel(InputDevice::Device, InputSensor::Accelerometer, "accelerometer");
	AKUSetInputDeviceTouch(InputDevice::Device, InputSensor::Touch, "touch");
}

bool touchDown = false;

void injectInput(const Event& ev)
{
	switch(ev.Type)
	{
	case Event::KeyPressed:
		if(ev.Key.Code < 127)
			AKUEnqueueKeyboardEvent(InputDevice::Device, InputSensor::Keyboard, ev.Key.Code, true);
		break;
	case Event::KeyReleased:
		if(ev.Key.Code < 127)
			AKUEnqueueKeyboardEvent(InputDevice::Device, InputSensor::Keyboard, ev.Key.Code, false);
		break;
	case Event::MouseButtonPressed:
		AKUEnqueueTouchEvent(InputDevice::Device, InputSensor::Touch, 1, true, (float)ev.MouseButton.X, (float)ev.MouseButton.Y, 1);
		touchDown = true;
		break;
	case Event::MouseButtonReleased:
		AKUEnqueueTouchEvent(InputDevice::Device, InputSensor::Touch, 1, false, (float)ev.MouseButton.X, (float)ev.MouseButton.Y, 1);
		touchDown = false;
		break;
	case Event::MouseMoved:
		if(touchDown)
			AKUEnqueueTouchEvent(InputDevice::Device, InputSensor::Touch, 1, true, (float)ev.MouseMove.X, (float)ev.MouseMove.Y, 1);
		break;
	}
}