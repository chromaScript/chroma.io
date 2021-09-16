#ifndef TSET_ALPHA_H
#define TSET_ALPHA_H

class CFunction;
class CObject;
class CInterpreter;
class Tool;

#include <glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <map>

class ToolSettings;

#include "../../ToolSettings.h"
#include "../../structs.h"

////////////////////////////////////////////////////////////////
// Alpha Settings
////////////////////////////////////////////////////////////////

#define TSET_ALPHA_OPACITY_MIN 0.0f
#define TSET_ALPHA_OPACITY_MAX 1.0f
#define TSET_ALPHA_FLOW_MIN 0.0f
#define TSET_ALPHA_FLOW_MAX 1.0f

class TSet_Alpha : public ToolSettings // 2800
{
public:
	bool usePressureOpacity; // 2800
	float minOpacity; // 2801
	float maxOpacity; // 2802
	float opacityBias; // 2803
	bool usePressureFlow; // 2810
	float minFlow; // 2811
	float maxFlow; // 2812
	float flowBias; // 2813
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Alpha();
	void modulateAlpha(float& outOpacity, float& outFlow, glm::vec3 dir, float pressure, float rotation, float tiltX, float tiltY, float velocity);
};

#endif