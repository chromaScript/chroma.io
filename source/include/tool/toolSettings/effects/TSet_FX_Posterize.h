#ifndef TSET_FX_POSTERIZE_H
#define TSET_FX_POSTERIZE_H

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

#define TSET_FX_POSTERIZE_LEVELS_MIN 1
#define TSET_FX_POSTERIZE_LEVELS_MAX 128
#define TSET_FX_POSTERIZE_GAMMA_MIN 0.1f
#define TSET_FX_POSTERIZE_GAMMA_MAX 3.0f

class TSet_FX_Posterize : public ToolSettings // 8300
{
public:
	float mixAmount; // 8305
	float levels; // 8310
	bool useValueRange; // 8311
	float gamma; // 8312

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_FX_Posterize();
	void initializeData(CColor FGColor, CColor BGColor);
};

#endif