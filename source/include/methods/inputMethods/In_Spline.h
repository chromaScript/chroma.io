#ifndef IN_SPLINE_H
#define IN_SPLINE_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_Spline : public InputMethod
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
	bool isRecording = false;
	bool splineActive = false;
	VertexData activeSpline = VertexData();

	TSet_Smoothing smoothing;
	TSet_Image image;
protected:
public:
	In_Spline(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_draw;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::continuous; }
		interestMask = {
			TSetType::basic,
			TSetType::smoothing,
			TSetType::character,
			TSetType::image,
			TSetType::spline,
			TSetType::scatter,
			TSetType::alpha,
			TSetType::color,
			TSetType::scatter,
			TSetType::effects
		};
		fragData.centerAboutOrigin = fragData.connectEnds = fragData.constantSize = false;
		fragData.linearStream = true;
		splineData.centerAboutOrigin = splineData.connectEnds = splineData.constantSize = false;
		splineData.linearStream = true;
		this->maxBufferLength = 80;
		this->anchorIDCount = 0;
		this->name = "Draw";
	};
	void beginRecording();
	void stopRecording();
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
};

#endif