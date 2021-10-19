#ifndef TSET_SAMPLER_H
#define TSET_SAMPLER_H

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

////////////////////////////////////////////////////////////////
// Sampler Settings
////////////////////////////////////////////////////////////////

class TSet_Sampler : public ToolSettings // 900
{
public:
	// Sampler Shape Type & Settings
	TSetProp sampleType = TSetProp::allLayers; // allLayers / currentLayer / currentBelow
	TSetProp shapeType = TSetProp::point; // Point / Radius / Shape / Tip
	int pointRandAmount;
	bool isCircle;
	unsigned char shapeRadius;
	unsigned char shapeSides;
	float shapeRotation;
	unsigned char* tipData;
	unsigned int tipSize;
	// Sample Background Color
	Keybind sampleBGModKey;
	// Auto Color Jitter & Control
	bool useFGVariance;
	TSetController FGValueControl;
	TSetController FGHueControl;
	TSetController FGSaturationControl;
	TSetNoise FGValueNoise;
	TSetNoise FGHueNoise;
	TSetNoise FGSaturationNoise;
	bool useAutoBGUpdater;
	float bgValueOffset;
	float bgHueOffset;
	float bgSaturationOffset;
	TSetController BGValueControl;
	TSetController BGHueControl;
	TSetController BGSaturationControl;
	TSetNoise BGValueNoise;
	TSetNoise BGHueNoise;
	TSetNoise BGSaturationNoise;
	// Palette Constraints
	bool usePaletteConstrain;
	TSetProp paletteType; // Simple / Complex
	float valueMin;
	float valueMax;
	float hueMin;
	float hueMax;
	float saturationMin;
	float saturationMax;
	std::vector<std::vector<glm::vec2>> paletteShapes;
	float paletteValueMin;
	float paletteValueMax;
	// Block Callback Notifications?
	bool shouldNotify;
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	TSet_Sampler();
};

#endif