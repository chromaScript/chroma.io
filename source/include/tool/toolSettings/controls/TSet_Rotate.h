#ifndef TSET_ROTATE_H
#define TSET_ROTATE_H

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
// Rotate Settings
////////////////////////////////////////////////////////////////

#define TSET_ROTATE_SPEED_MIN 0.1f
#define TSET_ROTATE_SPEED_MAX 5.0f
#define TSET_ROTATE_ANGLESNAP_MIN 2.5f
#define TSET_ROTATE_ANGLESNAP_MAX 90.0f
#define TSET_ROTATE_FACTORA_MIN 0.15f
#define TSET_ROTATE_FACTORA_MAX 1.45f

class TSet_Rotate : public ToolSettings // 300
{
public:
	bool rotateAboutCursor; // 300
	float rotationSpeed; // 301
	Keybind snapAngleA_modKey; // 310
	float snapAngleA_angle; // 311
	Keybind snapAngleB_modKey; // 312
	float snapAngleB_angle; // 313
	Keybind snapAngleC_modKey; // 314
	float snapAngleC_angle; // 315
	float snapTolerance_factorA; // 320
	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Rotate();
};

#endif