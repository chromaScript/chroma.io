#ifndef TSET_SCATTER_H
#define TSET_SCATTER_H

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
// Scatter Settings
////////////////////////////////////////////////////////////////

class TSet_Scatter : public ToolSettings // 2600
{
public:
	TSetControl_Node angleNode = TSetControl_Node(-1.0f, 1.0f, true, "ANGLE"); // 2600
	TSetController angleController; // 2601
	TSetNoise angleNoise = TSetNoise(-1.0f, 1.0f, true, "ANGLE"); // 2602

	TSetControl_Node distanceXNode = TSetControl_Node(-1.0f, 1.0f, true, "DIST X"); // 2610
	TSetController distanceXController; // 2611
	TSetNoise distanceXNoise = TSetNoise(-1.0f, 1.0f, true, "DIST X"); // 2612

	TSetControl_Node distanceYNode = TSetControl_Node(-1.0f, 1.0f, true, "DIST Y"); // 2620
	TSetController distanceYController; // 2621
	TSetNoise distanceYNoise = TSetNoise(-1.0f, 1.0f, true, "DIST Y"); // 2622

	TSetControl_Node countNode = TSetControl_Node(0.0f, 1.0f, false, "COUNT"); // 2630
	TSetController countController; // 2631
	TSetNoise countNoise = TSetNoise(0.0f, 1.0f, false, "COUNT"); // 2632

	float distRange = 500.0f;
	float countRange = 50.0f;

	TSetGraph opacityByDist; // 2640
	TSetGraph flowByDist; // 2641
	TSetGraph scaleByDist; // 2642
	TSetGraph hueByDist; // 2643
	TSetGraph satByDist; // 2644
	TSetGraph valByDist; // 2645
	TSetGraph rotByDist; // 2646

	bool useAngle = false; // 2660
	bool useDirection = false; // 2661
	bool relativeDistance = false; // 2662
	float offsetAngle = 0.0f; // 2663

	float maxDistance = 500.0f;
	float maxCount = 50.0f;
	float padding = 0.0f;
	float imageSize = 0.0f;

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Scatter();
	void initialize(float* trueSpacing, int* entityCount, int* tipSize, glm::ivec2* canvasDimensions);
	glm::vec3 modulatePosition(
		glm::vec3& finalPos, int* tipSize,
		glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
		int* shardCount, int* anchorCount, int* splineCount);
	int modulateCount(
		int& outCount,
		glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
		int* shardCount, int* anchorCount, int* splineCount);
	void updatePadding();
	int fetchPadding(int tipSize);
};

#endif