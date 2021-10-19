#include "../../../include/tool/ToolSettings.h"
#include "../../../include/input/keys.h"
#include "../../../include/math/Color.h"

class CFunction;
#include "../../../include/cscript/CInterpreter.h"
#include "../../../include/cscript/CObject.h"
#include "../../../include/tool/Tool.h"

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>
#include <string>
#include <vector>
#include <memory>
#include <random>

#include "../../../include/tool/toolSettings/gen/TSet_Image.h"

////////////////////////////////////////////////////////////////
// Image Settings
////////////////////////////////////////////////////////////////

TSet_Image::TSet_Image()
{
	this->type = TSetType::image;
	this->isEnabled = true;
	this->tipSize = 30; // 2201
	this->relativeSpacing = true; // 2202
	this->spacing = 15.0f; // 2203 // When in relative mode, use (spacing / 100) to get the % value
	this->sampleRate = 0.0f; // 2205 // Performance based slider to modulate delta time between input samples

	this->colorTotalInfluence = 1.0f; // 2209
	this->fgInfluence_total = 1.0f; // 2210
	this->fgInfluence_maskRed = 0.0f; // 2211
	this->fgInfluence_maskBlue = 0.0f; // 2212
	this->fgInfluence_maskGreen = 0.0f; // 2213
	this->fgInfluence_maskValue = 0.0f; // 2214
	this->fgInfluence_maskAlpha = 0.0f; // 2215
	this->bgInfluence_total = 0.0f; // 2216
	this->bgInfluence_maskRed = 0.0f; // 2217 
	this->bgInfluence_maskBlue = 0.0f; // 2218
	this->bgInfluence_maskGreen = 0.0f; // 2219
	this->bgInfluence_maskValue = 0.0f; // 2220
	this->bgInfluence_maskAlpha = 0.0f; // 2221

	this->tipType = TSetProp::radius; // 2230

	this->flipX = false; // 2250
	this->flipY = false; // 2251
	this->scaleX = 1.0f; // 2252
	this->scaleY = 1.0f; // 2253
	this->rotation = 0.0f; // 2254
	this->offsetRelative = true; // 2255
	this->offsetX = 0.0f; // 2256
	this->offsetY = 0.0f; // 2257
	this->blurFinal_amount = 0.0f; // 2258
	this->blurFinal_quality = 16.0f; // 2259
	this->antiAliasing = 4.0f; // 2260

	updateTrueSpacing();
	updateTrueOffset();
}
std::shared_ptr<CObject> TSet_Image::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig == 2200) {}
	else if (settingsSig == 2201)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(tipSize, settingsSig, subSig, "tipSize")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange_cubed((float)tipSize, TSET_IMAGE_TIPSIZE_MIN, TSET_IMAGE_TIPSIZE_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(tipSize)); }
		}
		else
		{
			if (asPercentage) {
				tipSize = (int)std::round(
					percentRange_cubed((float)std::get<double>(object.get()->obj), TSET_IMAGE_TIPSIZE_MIN, TSET_IMAGE_TIPSIZE_MAX, false));
			}
			else {
				tipSize = clampi(
					(int)std::round(std::get<double>(object.get()->obj)), TSET_IMAGE_TIPSIZE_MIN, TSET_IMAGE_TIPSIZE_MAX);
			}
			updateTrueSpacing();
		}
	}
	else if (settingsSig == 2202)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(relativeSpacing, settingsSig, subSig, "relativeSpacing")); }
			return std::make_shared<CObject>(relativeSpacing);
		}
		else
		{
			relativeSpacing = std::get<bool>(object.get()->obj);
			updateTrueSpacing();
		}
	}
	else if (settingsSig == 2203)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(spacing, settingsSig, subSig, "spacing")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange_cubed(spacing, TSET_IMAGE_SPACING_MIN, TSET_IMAGE_SPACING_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(spacing)); }
		}
		else
		{
			if (asPercentage) {
				spacing = percentRange_cubed(
					(float)std::get<double>(object.get()->obj), TSET_IMAGE_SPACING_MIN, TSET_IMAGE_SPACING_MAX, false);
			}
			else { spacing = clampf((float)std::get<double>(object.get()->obj), TSET_IMAGE_SPACING_MIN, TSET_IMAGE_SPACING_MAX); }
			updateTrueSpacing();
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Image::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_Image::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_Image::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_Image::getNoise(int settingSig, int subSig)
{
	return nullptr;
}

void TSet_Image::updateTrueSpacing()
{
	if (relativeSpacing) { trueSpacing = tipSize * (spacing / 100.0f); }
	else { trueSpacing = spacing; }
}
void TSet_Image::updateTrueOffset()
{
	if (offsetRelative)
	{
		trueOffsetX = tipSize * (offsetX / 100.0f);
		trueOffsetY = tipSize * (offsetY / 100.0f);
	}
	else { trueOffsetX = offsetX; trueOffsetY = offsetY; }
}

