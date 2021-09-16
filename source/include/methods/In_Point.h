#ifndef IN_POINT_H
#define IN_POINT_H

#include "InputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"

#include <iostream>

class In_Point : public InputMethod
{
private:
protected:
public:
	In_Point(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner } 
	{
		this->type = MethodType::point;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::drag; }
		interestMask = { TSetType::sampler }; 
		fragData.constantSize = true; 
		this->maxBufferLength = 5;
		this->anchorIDCount = 0;
		this->name = "Point";
	}
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif