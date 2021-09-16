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
#include "../../structs.h"

////////////////////////////////////////////////////////////////
// Scatter Settings
////////////////////////////////////////////////////////////////

#define TSET_SCATTER_ANGLE_MIN -180.0f
#define TSET_SCATTER_ANGLE_MAX 180.0f
#define TSET_SCATTER_DIST_MIN -350.0f
#define TSET_SCATTER_DIST_MAX 350.0f
#define TSET_SCATTER_COUNT_MIN 1
#define TSET_SCATTER_COUNT_MAX 30

class TSet_Scatter : public ToolSettings // 2600
{
public:
	bool useAngle; // 2600
	TSetController angleControl; // 2610
	TSetNoise angleNoise; // 2611
	float angleMin; // 2612
	float angleMax; // 2613
	float angleBias; // 2614
	bool useDirection; // 2616
	bool relativeDistance; // 2617
	TSetController distanceControl; // 2620
	TSetNoise distanceNoise; // 2621
	float distanceXMin; // 2622
	float distanceXMax; // 2623
	float distanceXBias; // 2624
	float distanceYMin; // 2625
	float distanceYMax; // 2626
	float distanceYBias; // 2627
	TSetController countControl; // 2630
	TSetNoise countNoise; // 2631
	float countMin; // 2632
	float countMax; // 2633
	float countBias; // 2634

	float maxDistance;
	float maxCount;
	float padding = 0.0f;

	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Scatter();
	glm::vec3 modulatePosition(glm::vec3 pos, glm::vec3 dir, float tipSize);
	int modulateCount();
	void updatePadding();
	int fetchPadding(int tipSize);
};

#endif