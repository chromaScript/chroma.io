#ifndef TSET_SMOOTHING_H
#define TSET_SMOOTHING_H

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

class TSet_Smoothing : public ToolSettings // 3700
{
public:
	// Note: Change this later? It tends to have the effect of turning a brush tip towards
	// the outer edge of a curve, rather than inwards. Might need to rotate by the negative
	// angle change as a different smoothing tool setting.
	bool smoothDirection;
	float directionThreshold;
	float directionFactor;
	bool smoothDirectionCurve;
	float curveDirectionThreshold;
	float curveDirectionFactor;
	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Smoothing();
};

#endif