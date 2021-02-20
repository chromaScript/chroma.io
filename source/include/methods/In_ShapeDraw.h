#ifndef IN_SHAPEDRAW_H
#define IN_SHAPEDRAW_H

#include "InputMethod.h"
#include "../ToolSettings.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_ShapeDraw : public InputMethod
{
private:
	// Tool Settings
	TSet_Polygon polygon;
protected:
public:
	In_ShapeDraw(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		interestMask = { TSetType::basic, TSetType::image, TSetType::polygon, TSetType::alpha };
		fragData.centerAboutOrigin = fragData.connectEnds = fragData.constantSize = false;
		fragData.linearStream = true;
		splineData.centerAboutOrigin = splineData.connectEnds = splineData.constantSize = false;
		splineData.linearStream = true;
		this->maxBufferLength = 10;
		this->anchorIDCount = 0;
	};
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif