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
#include "../../structs.h"

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
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Basic();
};

#endif