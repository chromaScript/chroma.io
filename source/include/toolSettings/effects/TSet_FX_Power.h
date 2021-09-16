#ifndef TSET_FX_POWER_H
#define TSET_FX_POWER_H

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

#define TSET_FX_POWER_GAMMA_MIN 0.001f
#define TSET_FX_POWER_GAMMA_MAX 20.0f

class TSet_FX_Power : public ToolSettings // 9000
{
public:
	float mixAmount; // 9005
	float gammaRed; // 9010
	float gammaGreen; // 9011
	float gammaBlue; // 9012

	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_FX_Power();
	void initializeData(CColor FGColor, CColor BGColor);
};

#endif