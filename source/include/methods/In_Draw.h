#ifndef IN_DRAW_H
#define IN_DRAW_H

#include "InputMethod.h"
#include "../ToolSettings.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_Draw : public InputMethod
{
private:
	//double constraintOriginX = 0.0;
	//double constraintOriginY = 0.0;
	//glm::vec3 constOriginX = glm::vec3(0, 0, 0);
	//glm::vec3 constOriginY = glm::vec3(0, 0, 0);
	//int constraintAxis = AXIS_NONE;
	//int constraintDetectionThreshold = 3; // Should incrememnt only on movement, not pauses.
	// 
	//glm::vec3 constraintOrigin = glm::vec3(0, 0, 0);
	//bool constraintDirty = false;
	//float constraintAngle = AXIS_EMPTY;
	// Tool Settings
	TSet_Smoothing smoothing;
protected:
public:
	In_Draw(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{ 
		interestMask = { TSetType::basic, TSetType::smoothing, TSetType::image, TSetType::alpha };
		fragData.constantSize = false;
		fragData.linearStream = true;
		fragData.centerAboutOrigin = false;
		fragData.connectEnds = false;
		this->maxBufferLength = 80;
		this->anchorIDCount = 0;
	};
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif