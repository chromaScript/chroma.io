#ifndef TSET_CHARACTER_H
#define TSET_CHARACTER_H

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
// Character Settings
////////////////////////////////////////////////////////////////

class TSet_Character : public ToolSettings // 2300
{
public:
	// Note: All the bool/float values underneath the Control/Noise variables are temporary
	// placeholders which will get wrapped into those once the UI element is ready.
	
	TSetControl_Node scaleNode = TSetControl_Node(0.01f, 1.0f, false, "SCALE"); // 2300
	TSetController scaleController; // 2301
	TSetNoise scaleNoise = TSetNoise(0.01f, 1.0f, false, "SCALE"); // 2302
	
	TSetControl_Node sizeX_node = TSetControl_Node(-1.0f, 1.0f, true, "SIZE X"); // 2303
	TSetController sizeX_controller; // 2304
	TSetNoise sizeX_noise = TSetNoise(-1.0f, 1.0f, false, "SIZE X"); // 2305
	
	TSetControl_Node sizeY_node = TSetControl_Node(-1.0f, 1.0f, true, "SIZE Y"); // 2306
	TSetController sizeY_controller; // 2307
	TSetNoise sizeY_noise = TSetNoise(-1.0f, 1.0f, true, "SIZE Y"); // 2308
	
	TSetControl_Node rotation_node = TSetControl_Node(-1.0f, 1.0f, true, "ANGLE"); // 2320
	TSetController rotation_controller; // 2321
	TSetNoise rotation_noise = TSetNoise(-1.0f, 1.0f, true, "ANGLE"); // 2322
	bool applyRotationLast = false; // 2323

	TSetControl_Node skewX_node = TSetControl_Node(-1.0f, 1.0f, true, "SKEW X"); // 2330
	TSetController skewX_controller; // 2331
	TSetNoise skewX_noise = TSetNoise(-1.0f, 1.0f, true, "SKEW X"); // 2332

	TSetControl_Node skewY_node = TSetControl_Node(-1.0f, 1.0f, true, "SKEW Y"); // 2333
	TSetController skewY_controller; // 2334
	TSetNoise skewY_noise = TSetNoise(-1.0f, 1.0f, true, "SKEW Y"); // 2335

	TSetControl_Node shuffleA_node = TSetControl_Node(-1.0f, 1.0f, true, "SHUFFLE A"); // 2340
	TSetController shuffleA_controller; // 2341
	TSetNoise shuffleA_noise = TSetNoise(-1.0f, 1.0f, true, "SHUFFLE A"); // 2342
	bool shuffleA_vert0 = false; // 2343
	bool shuffleA_vert1 = false; // 2344
	bool shuffleA_vert2 = false; // 2345
	bool shuffleA_vert3 = false; // 2346

	TSetControl_Node shuffleB_node = TSetControl_Node(-1.0f, 1.0f, true, "SHUFFLE B"); // 2350
	TSetController shuffleB_controller; // 2351
	TSetNoise shuffleB_noise = TSetNoise(-1.0f, 1.0f, true, "SHUFFLE B"); // 2352
	bool shuffleB_vert0 = false; // 2353
	bool shuffleB_vert1 = false; // 2354
	bool shuffleB_vert2 = false; // 2355
	bool shuffleB_vert3 = false; // 2356

	TSetControl_Node spread_node = TSetControl_Node(0.0f, 1.0f, false, "SPREAD");; // 2360
	TSetController spread_controller; // 2361
	TSetNoise spread_noise = TSetNoise(0.0f, 1.0f, false, "SPREAD");; // 2362
	TSetProp spreadCorner = TSetProp::one; // 2363
	float spreadSymmetry = 0.0f; // 2364

	bool flipX_random = false; // 2380
	bool flipY_random = false; // 2381

	virtual TSetType getType() { return type; }

	virtual TSetControl_Node* getControlNode(int settingSig, int subSig);
	virtual TSetController* getController(int settingSig, int subSig);
	virtual TSetGraph* getGraph(int settingSig, int subSig);
	virtual TSetNoise* getNoise(int settingSig, int subSig);

	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Character();
	void initialize(float* trueSpacing, int* entityCount, glm::ivec2* canvasDimensions);
	float fetchPadding(float tipSize);
	void modulateScale(
		glm::vec4& outScale, float* imageSize, bool calcAll,
		glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input, 
		int* shardCount, int* anchorCount, int* splineCount);
	bool modulateRotation(
		glm::vec3& outDir,
		glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
		int* shardCount, int* anchorCount, int* splineCount);
	void modulateCharacter(
		glm::vec2& outVert0, glm::vec2& outVert1, glm::vec2& outVert2, glm::vec2& outVert3, float* imageSize,
		glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, Input* input,
		int* shardCount, int* anchorCount, int* splineCount);
};

#endif