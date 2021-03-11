#ifndef IN_SHAPEFIELD_H
#define IN_SHAPEFIELD_H

#include "InputMethod.h"
#include "../ToolSettings.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_ShapeField : public InputMethod
{
private:
	// Tool Settings
	TSet_Polygon polygon;
	TSet_Field field;
protected:
public:
	In_ShapeField(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::onepoint; }
		interestMask = { TSetType::basic, TSetType::image, TSetType::polygon, TSetType::field, TSetType::alpha };
		fragData.constantSize = false;
		fragData.linearStream = false;
		this->maxBufferLength = 10;
		this->anchorIDCount = 0;
	};
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif