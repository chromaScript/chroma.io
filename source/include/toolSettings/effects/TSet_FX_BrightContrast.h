#ifndef TSET_FX_BRIGHTCONTRAST_H
#define TSET_FX_BRIGHTCONTRAST_H

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

#define TSET_FX_BRIGHT_BRIGHT_MIN -1.0f
#define TSET_FX_BRIGHT_BRIGHT_MAX 1.0f
#define TSET_FX_BRIGHT_CONTRAST_MIN -1.0f
#define TSET_FX_BRIGHT_CONTRAST_MAX 1.0f

class TSet_FX_BrightContrast : public ToolSettings // 8600
{
public:
	float mixAmount; // 8605
	float brightnessMin; // 8610
	float brightnessMax;  // 8611
	float brightnessMix; // 8612
	float contrastMin; // 8613
	float contrastMax; // 8614
	float contrastMix; // 8615

	float finalBrightness = 0.0f;
	float finalContrast = 0.0f;

	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_FX_BrightContrast();
	void initializeData(CColor FGColor, CColor BGColor);
};

#endif