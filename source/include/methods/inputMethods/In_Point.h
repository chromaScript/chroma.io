#ifndef IN_POINT_H
#define IN_POINT_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <iostream>

class In_Point : public InputMethod
{
private:
protected:
public:
	In_Point(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_point;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::drag; }
		interestMask = { TSetType::sampler }; 
		fragData.constantSize = true; 
		this->maxBufferLength = 5;
		this->anchorIDCount = 0;
		this->name = "Point";
	}
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
};

#endif