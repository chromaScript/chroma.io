#ifndef TSET_VORTEX_H
#define TSET_VORTEX_H

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
// Vortex Settings
////////////////////////////////////////////////////////////////

class TSet_Vortex : public ToolSettings // 5400
{
public:
	bool constantRingCount;
	float rings;
	float radius;
	int ringDensity;
	TSetProp ringSpacingMode;
	float ringSpacing_simple;
	float ringRotationOffset;
	float ringRotationRandom;
	TSetProp scatteringMode;
	float angleVariance_simple;
	float radiusVariance_simple;
	float offsetVarianceX_simple;
	float offsetVarianceY_simple;
	TSetProp connectionOrdering;
	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Vortex();
};

#endif