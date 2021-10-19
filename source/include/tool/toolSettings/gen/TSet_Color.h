#ifndef TSET_COLOR_H
#define TSET_COLOR_H

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
// Color Settings
////////////////////////////////////////////////////////////////

#define TSET_COLOR_HUE_MIN -180.0f
#define TSET_COLOR_HUE_MAX 180.0f
#define TSET_COLOR_SAT_MIN -1.0f
#define TSET_COLOR_SAT_MAX 1.0f
#define TSET_COLOR_LUM_MIN -1.0f
#define TSET_COLOR_LUM_MAX 1.0f
#define TSET_COLOR_RGB_MIN -1.0f
#define TSET_COLOR_RGB_MAX 1.0f

class TSet_Color : public ToolSettings // 2700
{
public:
	TSetControl_Node mixColorNode = TSetControl_Node(0.0f, 1.0f, false, "MIX COLOR"); // 2700
	TSetController mixColorController; // 2701
	TSetNoise mixColorNoise = TSetNoise(0.0f, 1.0f, false, "MIX COLOR"); // 2702
	TSetProp colorMode = TSetProp::background; // 2703
	CColor colorActive = black; // 2704
	CColor_hsl colorActive_hsl = CColor_hsl(0.0, 0.0, 0.0); // Internal performance variable
	CColor colorStored = black; // 2705
	float colorHueOffset = 0.0f; // 2706
	float colorSatOffset = 0.0f; // 2707
	float colorValOffset = 0.0f; // 2708
	bool enableMixColor = false; // 2709
	
	TSetProp mixColorMode = TSetProp::rgb; // 2710
	TSetProp mixRampMode = TSetProp::rgb; // 2711
	
	TSetControl_Node colorHueRedNode = TSetControl_Node(-1.0f, 1.0f, true, "TARGET COLOR HUE/RED"); // 2720
	TSetControl_Node colorSatGreenNode = TSetControl_Node(-1.0f, 1.0f, true, "TARGET COLOR SAT/GREEN"); // 2721
	TSetControl_Node colorValBlueNode = TSetControl_Node(-1.0f, 1.0f, true, "TARGET COLOR VAL/BLUE"); // 2722
	TSetController colorController; // 2723
	TSetNoise colorHueRedNoise = TSetNoise(-1.0f, 1.0f, true, "TARGET COLOR HUE/RED"); // 2724
	TSetNoise colorSatGreenNoise = TSetNoise(-1.0f, 1.0f, true, "TARGET COLOR SAT/GREEN"); // 2725
	TSetNoise colorValBlueNoise = TSetNoise(-1.0f, 1.0f, true, "TARGET COLOR VAL/BLUE"); // 2726
	TSetProp colorTypeMode = TSetProp::hsl; // 2727
	bool enableColor = false; // 2728

	TSetControl_Node finalHueRedNode = TSetControl_Node(-1.0f, 1.0f, true, "FINAL COLOR HUE/RED");; // 2770
	TSetControl_Node finalSatGreenNode = TSetControl_Node(-1.0f, 1.0f, true, "FINAL COLOR SAT/GREEN"); // 2771
	TSetControl_Node finalValBlueNode = TSetControl_Node(-1.0f, 1.0f, true, "FINAL COLOR VAL/BLUE"); // 2772
	TSetController finalController; // 2773
	TSetNoise finalHueRedNoise = TSetNoise(-1.0f, 1.0f, true, "FINAL COLOR HUE/RED"); // 2774
	TSetNoise finalSatGreenNoise = TSetNoise(-1.0f, 1.0f, true, "FINAL COLOR SAT/GREEN"); // 2775
	TSetNoise finalValBlueNoise = TSetNoise(-1.0f, 1.0f, true, "FINAL COLOR VAL/BLUE"); // 2776
	TSetProp finalTypeMode = TSetProp::hsl; // 2777
	bool enableFinalColor = false; // 2778

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Color();
	void initialize(float* trueSpacing, int* entityCount, glm::ivec2* canvasDimensions);
	void modulateColor(CColor& outColor, CColor* FGcolor, CColor* BGcolor, 
		glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
		int* shardCount, int* anchorCount, int* splineCount);
};

#endif