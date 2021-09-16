#ifndef IN_POLYGON_H
#define IN_POLYGON_H

#include "InputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"

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
	In_Polygon(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::polygon;
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
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif