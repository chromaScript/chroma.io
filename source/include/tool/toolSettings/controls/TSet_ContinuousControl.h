#ifndef TSET_CONTINUOUSCONTROL_H
#define TSET_CONTINUOUSCONTROL_H

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
#include "../../../input/keys.h"

////////////////////////////////////////////////////////////////
// Continuous Control Settings
////////////////////////////////////////////////////////////////

class TSet_ContinuousControl : public ToolSettings // 400
{
public:
	TSetProp defaultMode; // draw / polyDraw // 400
	Keybind alternateModeKey; // 401
	//
	float anchorSpacing; // 402
	bool relativeSpacing; // 403
	bool useCanvas; // 404
	float trueSpacing = 0.5f;
	bool useAngle; // 405
	float angleMin; // 406
	float angleMax; // 407
	float angleBias; // 408
	bool useDirection; // 410
	bool relativeDistance; // 411
	float splineRandomXMin; // 412
	float splineRandomXMax; // 413
	float splineRandomXBias; // 414
	float splineRandomYMin; // 415
	float splineRandomYMax; // 416
	float splineRandomYBias; // 417
	//
	bool fillOnShiftClick;
	bool fillOnPolyDraw;
	float connectPropertiesWeighting;
	// Constraint angles must always be between -90 to +90. Because they are an 'axis',
	// the opposite angle will also be considered. Entering a negative angle, or angle beyond 180
	// should be handled by the UI widget and corrected before sending, elsewise it will
	// get clamped.
	// The user may set a different mod key for each constraint, enabling more customization and
	// finer control when two desired constraints would be difficult to distinguish.
	float constraintLengthThreshold; // 430
	// Constraint Angle A
	bool enableConstrainA; // 431
	float constrainA_angle; // 432
	Keybind constrainA_modKey; // 433
	// Constraint Angle B
	bool enableConstrainB; // 435
	float constrainB_angle; // 436
	Keybind constrainB_modKey; // 437
	// Constraint Angle C
	bool enableConstrainC; // 440
	float constrainC_angle; // 441
	Keybind constrainC_modKey; // 442
	// Constraint Angle D
	bool enableConstrainD; // 445
	float constrainD_angle; // 446
	Keybind constrainD_modKey; // 447
	// Quick Swap Keys (Overrides constraint angle keys & all other hotkeys)
	// These can be mapped to any tool and any regular key expression. Availability, accepted keys, and number of
	// hot swaps able to be set depends on the input control mode, and thus hot swap keys are bound within each
	// control mode for each tool, rather than as global key bindings.
	// Hotswap Angle A
	bool enableHotswapA; // 450
	int hotswapA_toolID; // 451
	Keybind hotswapA_modKey; // 452
	// Hotswap Angle B
	bool enableHotswapB; // 453
	int hotswapB_toolID; // 454
	Keybind hotswapB_modKey; // 455
	// Hotswap Angle C
	bool enableHotswapC; // 456
	int hotswapC_toolID; // 457
	Keybind hotswapC_modKey; // 458
	// Hotswap Angle D
	bool enableHotswapD; // 459
	int hotswapD_toolID; // 460
	Keybind hotswapD_modKey; // 461
	// Control Variables (Unexposed)
	bool activated = false;
	Keybind activeKey = Keybind();
	glm::vec3 origin = glm::vec3(0);
	glm::vec4 line = glm::vec4(0);
	glm::vec3 dir = glm::vec3(0);
	glm::vec3 perpendicular = glm::vec3(0);
	float activeAngle = 0.0f;
	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_ContinuousControl();
	void clearConstraint();
	void updateTrueSpacing(std::shared_ptr<Tool> owner, int canvasWidth, int canvasHeight);
};

#endif