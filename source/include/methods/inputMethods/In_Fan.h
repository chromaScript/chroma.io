#ifndef IN_FAN
#define IN_FAN

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

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
	In_Fan(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_fan;
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
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
};

#endif