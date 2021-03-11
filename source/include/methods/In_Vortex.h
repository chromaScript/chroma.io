#ifndef IN_VORTEX_H
#define IN_VORTEX_H

#include "InputMethod.h"
#include "../ToolSettings.h"

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
	In_Vortex(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::drag; }
		interestMask = { TSetType::basic, TSetType::vortex, TSetType::image, TSetType::alpha };
		fragData.centerAboutOrigin = fragData.connectEnds = fragData.constantSize = false;
		splineData.centerAboutOrigin = splineData.connectEnds = splineData.constantSize = false;
		if (controlScheme == TSetType::drag || controlScheme == TSetType::drag)
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
	};
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif