#ifndef TSET_FX_INVERT_H
#define TSET_FX_INVERT_H

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

class TSet_FX_Invert : public ToolSettings // 8400
{
public:
	float mixAmount; // 8405
	bool invertRed; // 8410
	bool invertGreen; // 8411
	bool invertBlue; // 8412

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_FX_Invert();
	void initializeData(CColor FGColor, CColor BGColor);
};

#endif