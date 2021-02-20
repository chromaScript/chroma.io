#ifndef IN_RADIALDRAW_H
#define IN_RADIALDRAW_H

#include "InputMethod.h"
#include "../ToolSettings.h"

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
		interestMask = { TSetType::basic, TSetType::fan, TSetType::smoothing, TSetType::image, TSetType::alpha };
		fragData.constantSize = false;
		fragData.linearStream = true;
		fragData.centerAboutOrigin = false;
		fragData.connectEnds = false;
		this->maxBufferLength = 20;
		this->anchorIDCount = 0;
	};
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif