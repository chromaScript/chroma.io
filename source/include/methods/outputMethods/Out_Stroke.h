#ifndef OUT_STROKE_H
#define OUT_STROKE_H

#include "../OutputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include "../../entities/visuals/Fragment.h"
#include "../../entities/visuals/Stroke.h"

class Application;

class Out_Stroke : public OutputMethod
{
private:
	unsigned int activePointsLayer = 0;
	unsigned int activeLinesLayer = 0;
	unsigned int activeBoundsLayer = 0;
protected:
	bool requestNewStroke = true;
	bool isPaused = false;
	bool pauseGate = true;
	int pauseIndex = 0;
	float pauseTime = 0;
	bool doStrokeBuildup = true; // Note: Need to read from tool settings later
	float buildUpFactor = 3.2f;
	float posThresholdLength = 2.4f;
	float posThresholdAbsolute = 1.8f;
	float dirThresholdAngle = 20.0f;
	float pressureThreshold = 0.3f;
	float tiltThreshold = 0.8f;
	float rotationThreshold = 40.0f;
	float velocityThreshold = 15.0f;
	float intensityThreshold = 0.05f;
	size_t pauseBuffer = 4;
	size_t stabilizationMin = 0; // Note: Need to read these from tool settings later
	size_t stabilizationMax = 0;
	std::shared_ptr<Stroke> activeFrag; // Active Fragment being added
	bool activeStroke = false;
	int anchorIndex = 0;
	// New variables
	size_t waitSize = 0;
	int lastAnchorIndex = 0;
	int lastAnchorArrayIndex = 0;
public:
	Out_Stroke(MethodType id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner }
	{
		this->type = MethodType::out_stroke;
		this->interestMask = { 
			TSetType::basic, 
			TSetType::smoothing,
			TSetType::color,
			TSetType::scatter,
			TSetType::image, 
			TSetType::character,
			TSetType::polygon,
			TSetType::polyline,
			TSetType::vortex,
			TSetType::field,
			TSetType::fan,
			TSetType::spline,
			TSetType::multiSpline,
			TSetType::rake,
			TSetType::alpha,
			TSetType::effects };
		this->name = "Stroke";
	}
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
	bool compareInputByProperties(FragmentAnchor* anchor1, FragmentAnchor* anchor2);
	FragmentAnchor average2Anchors(FragmentAnchor* anchor1, FragmentAnchor* anchor2);
};

#endif