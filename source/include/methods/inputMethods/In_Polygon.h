#ifndef IN_POLYGON_H
#define IN_POLYGON_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_Polygon : public InputMethod
{
private:
	// Tool Settings
	TSet_Polygon polygon;
protected:
public:
	In_Polygon(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_polygon;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::onepoint; }
		interestMask = { 
			TSetType::basic, 
			TSetType::image, 
			TSetType::character,
			TSetType::polygon, 
			TSetType::alpha,
			TSetType::color,
			TSetType::scatter,
			TSetType::effects
		};
		fragData.constantSize = true;
		fragData.linearStream = true;
		this->maxBufferLength = 10;
		this->anchorIDCount = 0;
		this->name = "Polygon";
	};
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
	virtual InputHandlerFlag key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys);
};

#endif