#ifndef IN_RAKE_H
#define IN_RAKE_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_Rake : public InputMethod
{
private:
	// Tool Settings
	TSet_Smoothing smoothing;
	TSet_Rake rake;
protected:
public:
	In_Rake(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_rake;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::continuous; }
		interestMask = { 
			TSetType::basic, 
			TSetType::rake, 
			TSetType::smoothing, 
			TSetType::character,
			TSetType::image, 
			TSetType::alpha,
			TSetType::color,
			TSetType::scatter,
			TSetType::effects
		};
		fragData.centerAboutOrigin = fragData.connectEnds = fragData.constantSize = false;
		fragData.linearStream = true;
		splineData.centerAboutOrigin = splineData.connectEnds = splineData.constantSize = false;
		splineData.linearStream = true;
		this->maxBufferLength = 80;
		this->anchorIDCount = 0;
		this->name = "Rake";
	};
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
	virtual InputHandlerFlag key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys);
};

#endif