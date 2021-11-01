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
Input::Input(float uniform)
{
	this->pressure = uniform;
	this->rotation = uniform;
	this->tiltX = uniform;
	this->tiltY = uniform;
	this->velocity = uniform;
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
Input Input::operator*(const float& in)
{
	Input input = Input(0.0f);
	input.pressure = this->pressure * in;
	input.rotation = this->rotation * in;
	input.tiltX = this->tiltX * in;
	input.tiltY = this->tiltY * in;
	input.velocity = this->velocity * in;
	return input;
}
Input Input::operator*=(const float& in)
{
	this->pressure *= in;
	this->rotation *= in;
	this->tiltX *= in;
	this->tiltY *= in;
	this->velocity *= in;
	return *this;
}
Input Input::operator/(const Input& in)
{
	Input input = Input(0.0f);
	input.pressure = this->pressure / in.pressure;
	input.rotation = this->rotation / in.rotation;
	input.tiltX = this->tiltX / in.tiltX;
	input.tiltY = this->tiltY / in.tiltY;
	input.velocity = this->velocity / in.velocity;
	return input;
}
Input Input::operator/(const float& in)
{
	Input input = Input(0.0f);
	input.pressure = this->pressure / in;
	input.rotation = this->rotation / in;
	input.tiltX = this->tiltX / in;
	input.tiltY = this->tiltY / in;
	input.velocity = this->velocity / in;
	return input;
}
Input Input::operator/=(const Input& in)
{
	this->pressure /= in.pressure;
	this->rotation /= in.rotation;
	this->tiltX /= in.tiltX;
	this->tiltY /= in.tiltY;
	this->velocity /= in.velocity;
	return *this;
}
Input Input::operator/=(const float& in)
{
	this->pressure /= in;
	this->rotation /= in;
	this->tiltX /= in;
	this->tiltY /= in;
	this->velocity /= in;
	return *this;
}
Input Input::operator+(const Input& in)
{
	Input input = Input(0.0f);
	input.pressure = this->pressure + in.pressure;
	input.rotation = this->rotation + in.rotation;
	input.tiltX = this->tiltX + in.tiltX;
	input.tiltY = this->tiltY + in.tiltY;
	input.velocity = this->velocity + in.velocity;
	return input;
}
Input Input::operator+=(const Input& in)
{
	this->pressure += in.pressure;
	this->rotation += in.rotation;
	this->tiltX += in.tiltX;
	this->tiltY += in.tiltY;
	this->velocity += in.velocity;
	return *this;
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

////////////////////////////////////////////////////////////////
//
// Helper Functions
//
////////////////////////////////////////////////////////////////

Input lerpInputs(Input* inputA, Input* inputB, float t)
{
	Input outInput = Input();
	outInput.pressure = lerpf(inputA->pressure, inputB->pressure, t);
	outInput.rotation = lerpf(inputA->rotation, inputB->rotation, t);
	outInput.tiltX = lerpf(inputA->tiltX, inputB->tiltX, t);
	outInput.tiltY = lerpf(inputA->tiltY, inputB->tiltY, t);
	outInput.velocity = lerpf(inputA->velocity, inputB->velocity, t);
	return outInput;
}

Input averageInputs(InputData* input, size_t size, float weight, float intensity, bool fromBack)
{
	size_t len = input->inputEvents.size();
	size_t depth = (len - size <= 0) ? depth = len : size;
	Input outInput = Input(0.0f);
	if (fromBack) {
		size_t d = 0;
		Input weighted = input->inputEvents[size_t(len - size_t((float)depth * (1.0f - weight)))];
		for (size_t i = len - 1; i >= 0 && i >= len - depth; i--) {
			outInput += lerpInputs(&input->inputEvents.at(i), &weighted, intensity);
			d++;
		}
		outInput /= clampf((float)depth, 1.0f, (float)size);
	}
	return outInput;
}