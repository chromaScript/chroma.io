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
#include "../../structs.h"

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
	bool enableMixBG; // 2700
	TSetController mixBGControl; // 2701
	TSetNoise mixBGNoise; // 2702
	bool enableBGDynamics; // 2710
	TSetController BGColorControl; // 2711
	TSetNoise BGColorNoise; // 2712
	float BGHue_min; // 2713
	float BGHue_max; // 2714
	float BGHue_bias; // 2715
	float BGSat_min; // 2716
	float BGSat_max; // 2717
	float BGSat_bias; // 2718
	float BGValue_min; // 2719
	float BGValue_max; // 2720
	float BGValue_bias; // 2721
	bool enableFinalDynamics; // 2730
	TSetController finalColorControl; // 2731
	TSetNoise finalColorNoise; // 2732
	float finalHue_min; // 2733
	float finalHue_max; // 2734
	float finalHue_bias; // 2735
	float finalSat_min; // 2736
	float finalSat_max; // 2737
	float finalSat_bias; // 2738
	float finalValue_min; // 2739
	float finalValue_max; // 2740
	float finalValue_bias; // 2741
	bool useRGB_BG; // 2750
	float bgRed_min; // 2751
	float bgRed_max; // 2752
	float bgGreen_min; // 2753
	float bgGreen_max; // 2754
	float bgBlue_min; // 2755
	float bgBlue_max; //2756
	bool useRGB_FG; // 2760
	float finalRed_min; // 2761
	float finalRed_max; // 2762
	float finalGreen_min; // 2763
	float finalGreen_max; // 2764
	float finalBlue_min; // 2765
	float finalBlue_max; //2766
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Color();
	void modulateColor(CColor& FGcolor, CColor& BGcolor, glm::vec3 dir,
		float pressure, float rotation, float tiltX, float tiltY, float velocity);
};

#endif