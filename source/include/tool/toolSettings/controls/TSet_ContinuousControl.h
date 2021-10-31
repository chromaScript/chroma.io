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
	TSetProp defaultMode = TSetProp::draw; // draw / line // 400
	Keybind alternateModeKey = Keybind(InputKey::unknown, InputModKey::alt); // 401
	Keybind altAngleSnapKeyA = Keybind(InputKey::unknown, InputModKey::shift); // 402
	float altAngleSnapA_angle = 15.0f; // 403
	Keybind altAngleSnapKeyB = Keybind(InputKey::unknown, InputModKey::ctrl); // 404
	float altAngleSnapB_angle = 5.0f; // 405
	//
	bool subdivide = false; // 406
	float pressureBoost = 1.0f; // 407
	bool closeShape = false; // 408
	float subdivideAmount = 1.0f; // 409
	Keybind connectLastStrokeKey = Keybind(InputKey::unknown, InputModKey::shift); // 410
	bool scatterConnections = false; // 411
	Keybind alternateSubModeKey = Keybind(InputKey::unknown, InputModKey::ctrl); // 412
	//
	float anchorSpacing = 2.5f; // 415
	bool relativeSpacing = true; // 416
	bool useCanvas = false; // 417
	bool enableAltSnapA = true; // 418
	bool enableAltSnapB = true; // 419
	float trueSpacing = 0.5f;

	float distRange = 500.0f;
	float countRange = 50.0f;

	TSetControl_Node angleNode = TSetControl_Node(-1.0f, 1.0f, true, "ANGLE"); // 420
	TSetController angleController; // 421
	TSetNoise angleNoise = TSetNoise(-1.0f, 1.0f, true, "ANGLE"); // 422

	TSetControl_Node distanceXNode = TSetControl_Node(-1.0f, 1.0f, true, "DIST X"); // 423
	TSetController distanceXController; // 424
	TSetNoise distanceXNoise = TSetNoise(-1.0f, 1.0f, true, "DIST X"); // 425

	TSetControl_Node distanceYNode = TSetControl_Node(-1.0f, 1.0f, true, "DIST Y"); // 426
	TSetController distanceYController; // 427
	TSetNoise distanceYNoise = TSetNoise(-1.0f, 1.0f, true, "DIST Y"); // 428

	TSetControl_Node countNode = TSetControl_Node(0.0f, 1.0f, false, "COUNT"); // 429
	TSetController countController; // 430
	TSetNoise countNoise = TSetNoise(0.0f, 1.0f, false, "COUNT"); // 431
	TSetProp connectionType = TSetProp::all; // 432
	int connectInterval = 1; // 433

	bool useAngle = false; // 434
	bool useDirection = false; // 435
	float offsetAngle = 0.0f; // 436
	bool relativeDistance = false; // 437

	// Constraint angles must always be between -90 to +90. Because they are an 'axis',
	// the opposite angle will also be considered. Entering a negative angle, or angle beyond 180
	// should be handled by the UI widget and corrected before sending, elsewise it will
	// get clamped.
	// The user may set a different mod key for each constraint, enabling more customization and
	// finer control when two desired constraints would be difficult to distinguish.
	float constraintLengthThreshold = 1.2f; // 440
	// Constraint Angle A
	bool enableConstrainA = true; // 441
	float constrainA_angle = 0.0f; // 442
	Keybind constrainA_modKey = Keybind(InputKey::unknown, InputModKey::shift); // 443
	// Constraint Angle B
	bool enableConstrainB = true; // 445
	float constrainB_angle = 90.0f; // 446
	Keybind constrainB_modKey = Keybind(InputKey::unknown, InputModKey::shift); // 447
	// Constraint Angle C
	bool enableConstrainC = true; // 450
	float constrainC_angle = 45.0f; // 451
	Keybind constrainC_modKey = Keybind(InputKey::unknown, InputModKey::shift); // 452
	// Constraint Angle D
	bool enableConstrainD = true; // 455
	float constrainD_angle = -45.0f; // 456
	Keybind constrainD_modKey = Keybind(InputKey::unknown, InputModKey::shift); // 457

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
	void initialize(float* trueSpacing, int* entityCount, int* tipSize, glm::ivec2* canvasDimensions);
	glm::vec3 modulatePosition(
		glm::vec3& finalPos, int* tipSize,
		glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
		int* shardCount, int* anchorCount, int* splineCount);
	int modulateCount(
		int& outCount,
		glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
		int* shardCount, int* anchorCount, int* splineCount);
};

#endif