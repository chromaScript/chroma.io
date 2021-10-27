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
	TSetProp sampleType = TSetProp::allLayers; // allLayers / currentLayer / currentBelow // 900
	TSetProp shapeType = TSetProp::point; // Point / Radius / Shape / Tip // 901
	int radius = 3; // 902
	// Sample Background Color
	Keybind sampleBGModKey = Keybind(InputKey::unknown, InputModKey::alt); // 928
	// Auto Color Jitter & Control
	bool useFGVariance = false; // 929
	TSetControl_Node FGHueRedNode = TSetControl_Node(-1.0f, 1.0f, true, "FOREGROUND HUE/RED"); // 930
	TSetControl_Node FGSatGreenNode = TSetControl_Node(-1.0f, 1.0f, true, "FOREGROUND SAT/GREEN"); // 931
	TSetControl_Node FGValBlueNode = TSetControl_Node(-1.0f, 1.0f, true, "FOREGROUND VAL/BLUE"); // 932
	TSetController FGController = TSetController(); // 933
	TSetNoise FGHueRedNoise = TSetNoise(-1.0f, 1.0f, true, "FOREGROUND HUE/RED"); // 934
	TSetNoise FGSatGreenNoise = TSetNoise(-1.0f, 1.0f, true, "FOREGROUND SAT/GREEN"); // 935
	TSetNoise FGValBlueNoise = TSetNoise(-1.0f, 1.0f, true, "FOREGROUND VAL/BLUE"); // 936
	TSetProp FGTypeMode = TSetProp::hsl; // 937
	bool useAutoBGUpdater = false; // 940
	float bgHueRedOffset = 0.0f; // 941
	float bgSatGreenOffset = 0.0f; // 942
	float bgValBlueOffset = 0.0f; // 943
	bool useModulatedFG = false; // 944
	TSetControl_Node BGHueRedNode = TSetControl_Node(-1.0f, 1.0f, true, "BACKGROUND HUE/RED"); // 950
	TSetControl_Node BGSatGreenNode = TSetControl_Node(-1.0f, 1.0f, true, "BACKGROUND SAT/GREEN"); // 951
	TSetControl_Node BGValBlueNode = TSetControl_Node(-1.0f, 1.0f, true, "BACKGROUND VAL/BLUE"); // 952
	TSetController BGController = TSetController(); // 953
	TSetNoise BGHueRedNoise = TSetNoise(-1.0f, 1.0f, true, "BACKGROUND HUE/RED"); // 954
	TSetNoise BGSatGreenNoise = TSetNoise(-1.0f, 1.0f, true, "BACKGROUND SAT/GREEN"); // 955
	TSetNoise BGValBlueNoise = TSetNoise(-1.0f, 1.0f, true, "BACKGROUND VAL/BLUE"); // 956
	TSetProp BGTypeMode = TSetProp::hsl; // 957
	// Palette Constraints
	bool usePaletteConstrain = false; // 960
	TSetProp paletteType = TSetProp::simple; // Simple / Complex // 961
	float hueMin = 0.0f; // 962
	float hueMax = 1.0f; // 963
	float saturationMin = 0.0f; // 964
	float saturationMax = 1.0f; // 965
	float valueMin = 0.0f; // 966
	float valueMax = 1.0f; // 967
	TSetProp simplePaletteTypeMode = TSetProp::hsl; // 968
	// Complex Palette Settings
	std::vector<std::vector<glm::vec2>> paletteShapes;
	float paletteValueMin = 0.0f;
	float paletteValueMax = 0.0f;
	// Block Callback Notifications?
	bool shouldNotify = true;
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	virtual TSetType getType() { return type; }

	void initialize(float* trueSpacing, int* entityCount, glm::ivec2* canvasDimensions);
	void modulateColor(CColor& outColorFG, CColor& outColorBG, CColor* inColor,
		glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
		int* shardCount, int* anchorCount, int* splineCount);

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	TSet_Sampler();
};

#endif