#ifndef TSET_GRADIENT_H
#define TSET_GRADIENT_H

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
// Gradient Settings
////////////////////////////////////////////////////////////////

class TSet_Gradient : public ToolSettings // 5700
{
public:
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Gradient();
};

#endif