#ifndef IN_VORTEX_H
#define IN_VORTEX_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_Vortex : public InputMethod
{
private:
	// Tool Settings
	TSet_Vortex vortex;
protected:
public:
	In_Vortex(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_vortex;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::drag; }
		interestMask = { 
			TSetType::basic, 
			TSetType::vortex, 
			TSetType::character,
			TSetType::image, 
			TSetType::alpha,
			TSetType::color,
			TSetType::scatter,
			TSetType::effects
		};
		fragData.centerAboutOrigin = fragData.connectEnds = fragData.constantSize = false;
		splineData.centerAboutOrigin = splineData.connectEnds = splineData.constantSize = false;
		if (controlScheme == TSetType::drag)
		{
			fragData.linearStream = false;
			splineData.linearStream = false;
		}
		else if (controlScheme == TSetType::continuous)
		{
			fragData.linearStream = true;
			splineData.linearStream = true;
		}
		this->maxBufferLength = 10;
		this->anchorIDCount = 0;
		this->name = "Vortex";
	};
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
};

#endif