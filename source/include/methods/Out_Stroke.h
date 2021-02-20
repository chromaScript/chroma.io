#ifndef OUT_STROKE_H
#define OUT_STROKE_H

#include "OutputMethod.h"
#include "../ToolSettings.h"

#include "../entities/Fragment.h"
#include "../entities/Stroke.h"

class Application;

class Out_Stroke : public OutputMethod
{
private:
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
	Out_Stroke(int id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner }
	{
		this->interestMask = { 
			TSetType::basic, 
			TSetType::smoothing, 
			TSetType::image, 
			TSetType::polygon,
			TSetType::polyline,
			TSetType::vortex,
			TSetType::field,
			TSetType::fan,
			TSetType::rake,
			TSetType::alpha };
	}
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
	bool compareInputByProperties(FragmentAnchor* anchor1, FragmentAnchor* anchor2);
	FragmentAnchor average2Anchors(FragmentAnchor* anchor1, FragmentAnchor* anchor2);
};

#endif