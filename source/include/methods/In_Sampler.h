#ifndef IN_SAMPLER_H
#define IN_SAMPLER_H

#include "InputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"

#include <iostream>

class In_Sampler : public InputMethod
{
private:
protected:
	TSet_Sampler sampler;
public:
	In_Sampler(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner } 
	{ 
		this->type = MethodType::sampler;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::usedefault; }
		interestMask = { TSetType::sampler };
		fragData.constantSize = true; 
		this->maxBufferLength = 4;
		this->anchorIDCount = 0;
		this->name = "Color Picker";
	}
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif