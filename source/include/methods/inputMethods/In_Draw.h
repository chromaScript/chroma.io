#ifndef IN_DRAW_H
#define IN_DRAW_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

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
	In_Draw(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_draw;
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
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
	virtual InputHandlerFlag key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys);
	void addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat);
	void generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat);
};

#endif