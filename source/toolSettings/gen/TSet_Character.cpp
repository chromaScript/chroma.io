#include "../../include/ToolSettings.h"
#include "../../include/keys.h"
#include "../../include/Color.h"

class CFunction;
#include "../../include/cscript/CInterpreter.h"
#include "../../include/cscript/CObject.h"
#include "../../include/Tool.h"

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>
#include <string>
#include <vector>
#include <memory>
#include <random>

#include "../../include/structs.h"
#include "../../include/toolSettings/gen/TSet_Character.h"

////////////////////////////////////////////////////////////////
// Character Settings
////////////////////////////////////////////////////////////////

TSet_Character::TSet_Character()
{
	this->type = TSetType::character;
	this->isEnabled = false;
	this->scale_usePressure = false; // 2330
	this->scale_pressureMin = 1.0f; // 2331
	this->scale_pressureMax = 1.0f; // 2332
	this->scale_noiseMin = 0.0f; // 2333
	this->scale_noiseMax = 0.0f; // 2334
	this->sizeX_usePressure = false; // 2340
	this->sizeX_pressureMin = 0.0f; // 2341
	this->sizeX_pressureMax = 1.0f; // 2342
	this->sizeX_noiseMin = 0.0f; // 2343
	this->sizeX_noiseMax = 0.0f; // 2344
	this->sizeY_usePressure = false; // 2350
	this->sizeY_pressureMin = 0.0f; // 2351
	this->sizeY_pressureMax = 1.0f; // 2352
	this->sizeY_noiseMin = 0.0f; // 2353
	this->sizeY_noiseMax = 0.0f; // 2354
	this->rotation_usePressure = false; // 2360
	this->rotation_pressureMin = 0.0f; // 2361
	this->rotation_pressureMax = 1.0f; // 2362
	this->rotation_noiseMin = 0.0f; // 2363
	this->rotation_noiseMax = 0.0f; // 2364
	this->skewX_usePressure = false; // 2370
	this->skewX_pressureMin = 0.0f; // 2371
	this->skewX_pressureMax = 1.0f; // 2372
	this->skewX_noiseMin = 0.0f; // 2373
	this->skewX_noiseMax = 0.0f; // 2374
	this->skewY_usePressure = false; // 2370
	this->skewY_pressureMin = 0.0f; // 2371
	this->skewY_pressureMax = 1.0f; // 2372
	this->skewY_noiseMin = 0.0f; // 2373
	this->skewY_noiseMax = 0.0f; // 2374
	this->balloon_usePressure = false; // 2380
	this->balloon_pressureMin = 0.0f; // 2381
	this->balloon_pressureMax = 1.0f; // 2382
	this->balloon_noiseMin = 0.0f; // 2383
	this->balloon_noiseMax = 0.0f; // 2384

	this->flipX_random = false; // 2320
	this->flipY_random = false; // 2321
}

std::shared_ptr<CObject> TSet_Character::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 2300 && settingsSig < 2340)
	{
		if (settingsSig == 2330)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(scale_usePressure, settingsSig, subSig, "scale_usePressure")); }
				return std::make_shared<CObject>(scale_usePressure);
			}
			else
			{
				scale_usePressure = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 2331)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(scale_pressureMin, settingsSig, subSig, "scale_pressureMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(scale_pressureMin, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(scale_pressureMin)); }
			}
			else
			{
				if (asPercentage) {
					scale_pressureMin = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { scale_pressureMin = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 2332)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(scale_pressureMax, settingsSig, subSig, "scale_pressureMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(scale_pressureMax, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(scale_pressureMax)); }
			}
			else
			{
				if (asPercentage) {
					scale_pressureMax = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { scale_pressureMax = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 2333)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(scale_noiseMin, settingsSig, subSig, "scale_noiseMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(scale_noiseMin, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(scale_noiseMin)); }
			}
			else
			{
				if (asPercentage) {
					scale_noiseMin = percentRange(
						(float)std::get<double>(object.get()->obj), -1.0f, 1.0f, false);
				}
				else { scale_noiseMin = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
		}
		else if (settingsSig == 2334)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(scale_noiseMax, settingsSig, subSig, "scale_noiseMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(scale_noiseMax, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(scale_noiseMax)); }
			}
			else
			{
				if (asPercentage) {
					scale_noiseMax = percentRange(
						(float)std::get<double>(object.get()->obj), -1.0f, 1.0f, false);
				}
				else { scale_noiseMax = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
		}
	}
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

float TSet_Character::modulateScale(float pressure)
{
	if (!isEnabled) { return 1.0f; }
	float outScale = 1.0f;

	std::random_device device;
	std::default_random_engine eng(device());
	std::uniform_real_distribution<float> distr(0, 1);

	if (scale_usePressure)
	{
		outScale = lerpf(scale_pressureMin, scale_pressureMax, pressure);
	}
	outScale = clampf(outScale + lerpf(scale_noiseMin, scale_noiseMax, distr(eng)), -1.0f, 1.0f);
	return outScale;
}
float TSet_Character::noiseScale(float preCalculatedScale, float pressure)
{
	if (!isEnabled) { return preCalculatedScale; }
	return preCalculatedScale * 1.0f;
}