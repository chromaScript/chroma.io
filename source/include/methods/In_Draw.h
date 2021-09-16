#ifndef IN_DRAW_H
#define IN_DRAW_H

#include "InputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"
#include "../structs.h"

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
	TSet_Image image;
protected:
public:
	In_Draw(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::draw;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::continuous; }
		interestMask = { 
			TSetType::basic, 
			TSetType::smoothing, 
			TSetType::character,
			TSetType::image, 
			TSetType::scatter, 
			TSetType::alpha, 
			TSetType::color,
			TSetType::scatter,
			TSetType::effects
		};
		fragData.constantSize = false;
		fragData.linearStream = true;
		fragData.centerAboutOrigin = false;
		fragData.connectEnds = false;
		this->maxBufferLength = 80;
		this->anchorIDCount = 0;
		this->name = "Draw";
	};
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif