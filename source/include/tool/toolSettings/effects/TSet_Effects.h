#ifndef TSET_EFFECTS_H
#define TSET_EFFECTS_H

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
class TSet_FX_Fill;
class TSet_FX_Gradient;
class TSet_FX_Posterize;
class TSet_FX_Invert;
class TSet_FX_Threshold;
class TSet_FX_BrightContrast;
class TSet_FX_HSV;

#include "../../ToolSettings.h"
#include "../../toolsettings/effects/TSet_FX_BrightContrast.h"
#include "../../toolsettings/effects/TSet_FX_HSV.h"
#include "../../toolsettings/effects/TSet_FX_Fill.h"
#include "../../toolsettings/effects/TSet_FX_Gradient.h"
#include "../../toolsettings/effects/TSet_FX_Invert.h"
#include "../../toolsettings/effects/TSet_FX_Posterize.h"
#include "../../toolsettings/effects/TSet_FX_Threshold.h"
#include "../../toolsettings/effects/TSet_FX_GaussianBlur.h"
#include "../../toolsettings/effects/TSet_FX_Power.h"
#include "../../toolsettings/effects/TSet_FX_Modulo.h"

class TSet_Effects : public ToolSettings // 8000
{
public:
	int effectsCount = 0;
	int effectsOrdering[16] =
	{ 0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0 }; // 8000

	TSet_FX_Fill fill = TSet_FX_Fill(); // 8010 // isEnabled // 8100
	TSet_FX_Gradient gradient = TSet_FX_Gradient(); // 8011 // isEnabled //8200
	TSet_FX_Posterize posterize = TSet_FX_Posterize(); // 8012 // isEnabled // 8300
	TSet_FX_Invert invert = TSet_FX_Invert(); // 8013 // isEnabled // 8400
	TSet_FX_Threshold threshold = TSet_FX_Threshold(); // 8014 // isEnabled // 8500
	TSet_FX_BrightContrast brightContrast = TSet_FX_BrightContrast(); // 8015 // isEnabled // 8600
	TSet_FX_HSV hsv = TSet_FX_HSV(); // 8016 // isEnabled // 8700
	TSet_FX_Power power = TSet_FX_Power(); // 8017 // isEnabled // 9000
	TSet_FX_Modulo modulo = TSet_FX_Modulo(); // 8018 // isEnabled // 9100
	TSet_FX_GaussianBlur blur = TSet_FX_GaussianBlur(); // 8024 // isEnabled // 9500

	// Used to have totalMask by RGBV, although the effects of these are negligable.
	// RGBV masking will be used for specific effects where it makes noticable effect.
	float totalBlend; // 8050
	float totalMask_alphaCenter; // 8051
	float totalMask_alphaRange; // 8052


	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet,
		bool asPercentage, bool asString);
	TSet_Effects();
	void initializeData(CColor FGColor, CColor BGColor, int tipSize);
	void updateEffectsOrdering(bool removeZero);
	void updateEffectsCount(bool checkVisible);
	std::vector<int> getOrdering_vec();
	int objectToFX_int(std::shared_ptr<CObject> object, bool isArraySet);
	std::string intToFX_string(int effectsSig);
};

#endif