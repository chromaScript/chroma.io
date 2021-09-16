#ifndef IN_RADIALDRAW_H
#define IN_RADIALDRAW_H

#include "InputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_Fan : public InputMethod
{
private:
	// Tool Settings
	TSet_Smoothing smoothing;
	TSet_Fan fan;
protected:
public:
	In_Fan(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::fan;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::continuous; }
		interestMask = { 
			TSetType::basic, 
			TSetType::fan, 
			TSetType::smoothing, 
			TSetType::character,
			TSetType::image, 
			TSetType::alpha, 
			TSetType::color, 
			TSetType::scatter,
			TSetType::effects };
		fragData.constantSize = false;
		fragData.linearStream = true;
		fragData.centerAboutOrigin = false;
		fragData.connectEnds = false;
		this->maxBufferLength = 20;
		this->anchorIDCount = 0;
		this->name = "Fan";
	};
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif