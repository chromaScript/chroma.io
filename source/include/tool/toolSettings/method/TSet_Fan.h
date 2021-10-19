#ifndef TSET_FAN_H
#define TSET_FAN_H

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
// Fan Settings
////////////////////////////////////////////////////////////////

class TSet_Fan : public ToolSettings // 5500
{
public:
	TSetProp waitMode;
	float waitAmount;
	bool stopWait = false;
	int subdivisionCount;
	TSetProp scatteringMode;
	float angleVariance_simple;
	float radiusVariance_simple;
	float offsetVarianceX_simple;
	float offsetVarianceY_simple;
	TSetProp connectionOrdering;
	bool airbrushMode;
	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Fan();
};

#endif