#ifndef TSET_FX_THRESHOLD_H
#define TSET_FX_THRESHOLD_H

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

class TSet_FX_Threshold : public ToolSettings // 8500
{
public:
	float level;
	CColor lowColor;
	CColor upperColor;
	float mixAmount;
	float mixLower;
	float mixUpper;

	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_FX_Threshold();
	void initializeData(CColor FGColor, CColor BGColor);
};

#endif