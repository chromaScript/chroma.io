#ifndef TSET_FX_GAUSSIANBLUR_H
#define TSET_FX_GAUSSIANBLUR_H

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

#define TSET_FX_BLUR_QUALITY_MIN 0.1f
#define TSET_FX_BLUR_QUALITY_MAX 16.0f
#define TSET_FX_BLUR_RADIUS_MIN 1.0f
#define TSET_FX_BLUR_RADIUS_MAX 128.0f
#define TSET_FX_BLUR_DIRECTION_MIN 2.0f
#define TSET_FX_BLUR_DIRECTION_MAX 48.0f

class TSet_FX_GaussianBlur : public ToolSettings // 9500
{
public:
	float mixAmount; // 9505
	float quality; // 9510
	float directions; // 9511
	float radius; // 9512
	float mixRed; // 9513
	float mixGreen; // 9514
	float mixBlue; // 9515
	float mixAlpha; // 9516

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_FX_GaussianBlur();
	void initializeData(CColor FGColor, CColor BGColor);
};

#endif