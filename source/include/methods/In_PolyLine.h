#ifndef IN_POLYLINE_H
#define IN_POLYLINE_H

#include "InputMethod.h"
#include "../ToolSettings.h"

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
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::drag; }
		interestMask = { TSetType::basic, TSetType::image, TSetType::polyline, TSetType::alpha };
		fragData.constantSize = false;
		fragData.linearStream = false;
		fragData.connectEnds = true;
		this->maxBufferLength = 10;
		this->anchorIDCount = 0;
	};
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif