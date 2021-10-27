#ifndef TSET_BASIC_H
#define TSET_BASIC_H

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
// Basic Settings
////////////////////////////////////////////////////////////////

class TSet_Basic : public ToolSettings // 2100
{
public:
	// Color Settings - When Color is locked, updates are no longer
	// immediately pushed to the savedColor. The savedColors are also
	// stored in the saved settings for the tool if a user wants to 
	// create a brush that always returns to the same color
	// Locking the colors also causes the tool to reset to it's saved colors
	// when it becomes active again.
	bool lockFGColor;
	bool lockBGColor;
	CColor savedFGColor;
	CColor currentFGColor;
	CColor savedBGColor;
	CColor currentBGColor;
	// Quick Swap Keys (Overrides constraint angle keys & all other hotkeys)
	// These can be mapped to any tool and any regular key expression. Availability, accepted keys, and number of
	// hot swaps able to be set depends on the input control mode, and thus hot swap keys are bound within each
	// control mode for each tool, rather than as global key bindings.
	// Hotswap Angle A
	bool enableHotswapA; // 2140
	int hotswapA_toolID; // 2141
	Keybind hotswapA_modKey; // 2142
	// Hotswap Angle B
	bool enableHotswapB; // 2143
	int hotswapB_toolID; // 2144
	Keybind hotswapB_modKey; // 2145
	// Hotswap Angle C
	bool enableHotswapC; // 2146
	int hotswapC_toolID; // 2147
	Keybind hotswapC_modKey; // 2148
	// Hotswap Angle D
	bool enableHotswapD; // 2149
	int hotswapD_toolID; // 2150
	Keybind hotswapD_modKey; // 2151
	// Hotswap Globals
	bool copySwapGlobal;

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Basic();
};

#endif