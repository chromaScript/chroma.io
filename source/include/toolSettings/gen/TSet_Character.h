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
#include "../../structs.h"

////////////////////////////////////////////////////////////////
// Character Settings
////////////////////////////////////////////////////////////////

class TSet_Character : public ToolSettings // 2300
{
public:
	// Note: All the bool/float values underneath the Control/Noise variables are temporary
	// placeholders which will get wrapped into those once the UI element is ready.
	TSetController scale_control;
	TSetNoise scale_noise;
	bool scale_usePressure; // 2330
	float scale_pressureMin; // 2331
	float scale_pressureMax; // 2332
	float scale_noiseMin; // 2333
	float scale_noiseMax; // 2334
	TSetController sizeX_control;
	TSetNoise sizeX_noise;
	bool sizeX_usePressure; // 2340
	float sizeX_pressureMin; // 2341
	float sizeX_pressureMax; // 2342
	float sizeX_noiseMin; // 2343
	float sizeX_noiseMax; // 2344
	TSetController sizeY_control;
	TSetNoise sizeY_noise;
	bool sizeY_usePressure; // 2350
	float sizeY_pressureMin; // 2351
	float sizeY_pressureMax; // 2352
	float sizeY_noiseMin; // 2353
	float sizeY_noiseMax; // 2354
	TSetController rotation_control;
	TSetNoise rotation_noise;
	bool rotation_usePressure; // 2360
	float rotation_pressureMin; // 2361
	float rotation_pressureMax; // 2362
	float rotation_noiseMin; // 2363
	float rotation_noiseMax; // 2364
	TSetController skewX_control;
	TSetNoise skewX_noise;
	bool skewX_usePressure; // 2370
	float skewX_pressureMin; // 2371
	float skewX_pressureMax; // 2372
	float skewX_noiseMin; // 2373
	float skewX_noiseMax; // 2374
	TSetController skewY_control;
	TSetNoise skewY_noise;
	bool skewY_usePressure; // 2370
	float skewY_pressureMin; // 2371
	float skewY_pressureMax; // 2372
	float skewY_noiseMin; // 2373
	float skewY_noiseMax; // 2374
	TSetController balloon_control;
	TSetNoise balloon_noise;
	bool balloon_usePressure; // 2380
	float balloon_pressureMin; // 2381
	float balloon_pressureMax; // 2382
	float balloon_noiseMin; // 2383
	float balloon_noiseMax; // 2384

	bool flipX_random; // 2320
	bool flipY_random; // 2321

	virtual TSetType getType() { return type; }
	virtual std::shared_ptr<CObject> putProperty(
		std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
		int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString);
	TSet_Character();
	float modulateScale(float pressure);
	float noiseScale(float preCalculatedScale, float pressure);
};

#endif