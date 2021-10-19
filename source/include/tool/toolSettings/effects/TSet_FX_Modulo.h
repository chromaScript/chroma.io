#ifndef TSET_FX_MODULO_H
#define TSET_FX_MODULO_H

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

#define TSET_FX_MODULO_VALUE_A_MIN 10.0f
#define TSET_FX_MODULO_VALUE_A_MAX 1000.0f
#define TSET_FX_MODULO_VALUE_B_MIN 1.0f
#define TSET_FX_MODULO_VALUE_B_MAX 2000.0f

class TSet_FX_Modulo : public ToolSettings // 9100
{
public:
	float mixAmount; // 9105
	float valueA; // 9110
	float valueB; // 9111
	bool useX; // 9112
	bool useY; // 9113
	bool useDirection; // 9114

	float XMixRed; // 9120
	float XMixGreen; // 9121
	float XMixBlue; // 9122
	float YMixRed; // 9123
	float YMixGreen; // 9124
	float YMixBlue; // 9125
	float DirMixRed; // 9126
	float DirMixGreen; // 9127
	float DirMixBlue; // 9128

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_FX_Modulo();
	void initializeData(CColor FGColor, CColor BGColor);
};

#endif