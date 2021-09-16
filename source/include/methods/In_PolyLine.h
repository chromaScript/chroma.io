#ifndef IN_POLYLINE_H
#define IN_POLYLINE_H

#include "InputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_PolyLine : public InputMethod
{
private:
	// Tool Settings
	TSet_PolyLine polyLine;
protected:
public:
	In_PolyLine(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::polyLine;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::drag; }
		interestMask = { 
			TSetType::basic, 
			TSetType::image, 
			TSetType::character,
			TSetType::polyline, 
			TSetType::alpha,
			TSetType::color,
			TSetType::scatter,
			TSetType::effects
		};
		fragData.constantSize = false;
		fragData.linearStream = false;
		fragData.connectEnds = true;
		this->maxBufferLength = 10;
		this->anchorIDCount = 0;
		this->name = "Poly Line";
	};
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif