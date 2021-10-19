#ifndef TSET_FX_HSV_H
#define TSET_FX_HSV_H

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

#define TSET_FX_HSV_HUE_MIN -180.0f
#define TSET_FX_HSV_HUE_MAX 180.0f
#define TSET_FX_HSV_SAT_MIN -1.0f
#define TSET_FX_HSV_SAT_MAX 1.0f
#define TSET_FX_HSV_LUM_MIN -1.0f
#define TSET_FX_HSV_LUM_MAX 1.0f

class TSet_FX_HSV : public ToolSettings // 8700
{
public:
	float mixAmount; // 8705
	float hueMin; // 8710
	float hueMax; // 8711
	float satMin; // 8712
	float satMax; // 8713
	float valMin; // 8714
	float valMax; // 8715

	float finalHue = 0.0f;
	float finalSat = 0.0f;
	float finalVal = 0.0f;

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_FX_HSV();
	void initializeData(CColor FGColor, CColor BGColor);
};

#endif