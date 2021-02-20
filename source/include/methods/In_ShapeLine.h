#ifndef IN_SHAPELINE_H
#define IN_SHAPELINE_H

#include "InputMethod.h"
#include "../ToolSettings.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_ShapeLine : public InputMethod
{
private:
	// Tool Settings
	TSet_Polygon polygon;
protected:
public:
	In_ShapeLine(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		interestMask = { TSetType::basic, TSetType::image, TSetType::polygon, TSetType::alpha };
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