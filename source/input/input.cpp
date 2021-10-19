#include "../include/input/input.h"
#include "../include/input/keys.h"

////////////////////////////////////////////////////////////////
//
// Input
//
////////////////////////////////////////////////////////////////

Input::Input()
{

}
Input::Input(float pressure, float rotation, float tiltX, float tiltY, float velocity)
{
	this->pressure = pressure;
	this->rotation = rotation;
	this->tiltX = tiltX;
	this->tiltY = tiltY;
	this->velocity = velocity;
}
Input::Input(InputModKey modKey, InputMouseButton button, InputAction action,
	InputFlag flagPrimary, InputFlag flagSecondary,
	double x, double y, float time,
	float pressure, float rotation, float tiltX, float tiltY, float velocity)
{
	this->modKey = modKey;
	this->button = button;
	this->action = action;
	this->flagPrimary = flagPrimary;
	this->flagSecondary = flagSecondary;
	this->x = x;
	this->y = y;
	this->time = time;
	this->pressure = pressure;
	this->rotation = rotation;
	this->tiltX = tiltX;
	this->tiltY = tiltY;
	this->velocity = velocity;
}
void Input::resetAll()
{
	resetAction();
	resetPos();
	resetStylus();
}
void Input::resetAction()
{
	isDouble = false;
	button = InputMouseButton::left;
	modKey = InputModKey::none;
	action = InputAction::release;
}
void Input::resetPos()
{
	time = 0.0f;
	x = y = 0.0;
}
void Input::resetStylus()
{
	velocity = rotation = tiltY = tiltX = pressure = 0.0f;
}

////////////////////////////////////////////////////////////////
//
// InputData
//
////////////////////////////////////////////////////////////////

InputData::InputData()
{

}

void InputData::reset()
{
	if (!inputEvents.empty()) { inputEvents.clear(); }
	//inputEvents.clear();
	inputEvents.swap(emptyEvents);
	inputModKey = InputModKey::none;
	start.resetAll();
	end.resetAll();
}