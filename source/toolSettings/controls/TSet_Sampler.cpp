#include "../../include/ToolSettings.h"
#include "../../include/keys.h"
#include "../../include/Color.h"

class CFunction;
#include "../../include/cscript/CInterpreter.h"
#include "../../include/cscript/CObject.h"
#include "../../include/Tool.h"

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>
#include <string>
#include <vector>
#include <memory>
#include <random>

#include "../../include/structs.h"
#include "../../include/toolSettings/controls/TSet_Sampler.h"

////////////////////////////////////////////////////////////////
// Sampler Settings
////////////////////////////////////////////////////////////////

TSet_Sampler::TSet_Sampler()
{
	this->type = TSetType::sampler;
	this->isEnabled = true;
	// Sampler Shape Type & Settings
	this->sampleType = TSetProp::allLayers; // allLayers / currentLayer / currentBelow
	this->shapeType = TSetProp::point; // Pothis->/ Radius / Shape / Tip
	this->pointRandAmount = 0;
	this->isCircle = true;
	this->shapeRadius = 1;
	this->shapeSides = 3;
	this->shapeRotation = 0;
	this->tipData = nullptr;
	this->tipSize = 1;
	// Sample Background Color
	this->sampleBGModKey = INPUT_MOD_ALT;
	// Auto Color Jitter & Control
	this->useFGVariance = false;
	this->FGValueControl;
	this->FGHueControl;
	this->FGSaturationControl;
	this->FGValueNoise;
	this->FGHueNoise;
	this->FGSaturationNoise;
	this->useAutoBGUpdater = false;
	this->bgValueOffset = 0;
	this->bgHueOffset = 0;
	this->bgSaturationOffset = 0;
	this->BGValueControl;
	this->BGHueControl;
	this->BGSaturationControl;
	this->BGValueNoise;
	this->BGHueNoise;
	this->BGSaturationNoise;
	// Palette Constraints
	this->usePaletteConstrain = false;
	this->paletteType = TSetProp::simple; // Simple / Complex
	this->valueMin = 0;
	this->valueMax = 0;
	this->hueMin = 0;
	this->hueMax = 0;
	this->saturationMin = 0;
	this->saturationMax = 0;
	this->paletteShapes;
	this->paletteValueMin = 0;
	this->paletteValueMax = 0;
	// Block Callback Notifications?
	this->shouldNotify = true;
}
std::shared_ptr<CObject> TSet_Sampler::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (asString) { return std::make_shared<CObject>(makeSettingString(0.0f, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}