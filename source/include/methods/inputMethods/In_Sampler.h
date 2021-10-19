#ifndef IN_SAMPLER_H
#define IN_SAMPLER_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <iostream>

class In_Sampler : public InputMethod
{
private:
protected:
	TSet_Sampler sampler;
public:
	In_Sampler(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{ 
		this->type = MethodType::in_sampler;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::usedefault; }
		interestMask = { TSetType::sampler };
		fragData.constantSize = true; 
		this->maxBufferLength = 4;
		this->anchorIDCount = 0;
		this->name = "Color Picker";
	}
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
};

#endif