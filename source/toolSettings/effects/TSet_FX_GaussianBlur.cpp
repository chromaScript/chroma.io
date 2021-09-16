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
#include "../../include/toolSettings/effects/TSet_FX_GaussianBlur.h"

////////////////////////////////////////////////////////////////
// FX GAUSSIAN BLUR Settings
////////////////////////////////////////////////////////////////

TSet_FX_GaussianBlur::TSet_FX_GaussianBlur()
{
	this->type = TSetType::fx_blur;
	this->isEnabled = false;
	this->mixAmount = 1.0f;
	this->quality = 4.0f; // 9510
	this->directions = 16.0f; // 9511
	this->radius = 16.0f; // 9512
	this->mixRed = 1.0f; // 9513
	this->mixGreen = 1.0f; // 9514
	this->mixBlue = 1.0f; // 9515
	this->mixAlpha = 1.0f; // 9516
}
std::shared_ptr<CObject> TSet_FX_GaussianBlur::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 9500 && settingsSig < 9510)
	{
		if (settingsSig == 9505)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(mixAmount, settingsSig, subSig, "mixAmount")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubedInvert(mixAmount, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(mixAmount)); }
			}
			else
			{
				if (asPercentage) {
					mixAmount = percentRange_cubedInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { mixAmount = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
	}
	else if (settingsSig >= 9510 && settingsSig < 9520)
	{
		if (settingsSig == 9510)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(quality, settingsSig, subSig, "quality")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(quality, TSET_FX_BLUR_QUALITY_MIN, TSET_FX_BLUR_QUALITY_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(quality)); }
			}
			else
			{
				if (asPercentage) {
					quality = percentRange_squared(
						(float)std::get<double>(object.get()->obj), TSET_FX_BLUR_QUALITY_MIN, TSET_FX_BLUR_QUALITY_MAX, false);
				}
				else { quality = clampf((float)std::get<double>(object.get()->obj), TSET_FX_BLUR_QUALITY_MIN, TSET_FX_BLUR_QUALITY_MAX); }
			}
		}
		else if (settingsSig == 9511)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(directions, settingsSig, subSig, "directions")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(directions, TSET_FX_BLUR_DIRECTION_MIN, TSET_FX_BLUR_DIRECTION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(directions)); }
			}
			else
			{
				if (asPercentage) {
					directions = percentRange_squared(
						(float)std::get<double>(object.get()->obj), TSET_FX_BLUR_DIRECTION_MIN, TSET_FX_BLUR_DIRECTION_MAX, false);
				}
				else { directions = clampf((float)std::get<double>(object.get()->obj), TSET_FX_BLUR_DIRECTION_MIN, TSET_FX_BLUR_DIRECTION_MAX); }
			}
		}
		else if (settingsSig == 9512)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(radius, settingsSig, subSig, "radius")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(radius, TSET_FX_BLUR_RADIUS_MIN, TSET_FX_BLUR_RADIUS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(radius)); }
			}
			else
			{
				if (asPercentage) {
					radius = percentRange_squared(
						(float)std::get<double>(object.get()->obj), TSET_FX_BLUR_RADIUS_MIN, TSET_FX_BLUR_RADIUS_MAX, false);
				}
				else { radius = clampf((float)std::get<double>(object.get()->obj), TSET_FX_BLUR_RADIUS_MIN, TSET_FX_BLUR_RADIUS_MAX); }
			}
		}
		else if (settingsSig == 9513)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(mixRed, settingsSig, subSig, "mixRed")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(mixRed, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(mixRed)); }
			}
			else
			{
				if (asPercentage) {
					mixRed = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { mixRed = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9514)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(mixGreen, settingsSig, subSig, "mixGreen")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(mixGreen, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(mixGreen)); }
			}
			else
			{
				if (asPercentage) {
					mixGreen = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { mixGreen = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9515)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(mixBlue, settingsSig, subSig, "mixBlue")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(mixBlue, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(mixBlue)); }
			}
			else
			{
				if (asPercentage) {
					mixBlue = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { mixBlue = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
		else if (settingsSig == 9516)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(mixAlpha, settingsSig, subSig, "mixAlpha")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(mixAlpha, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(mixAlpha)); }
			}
			else
			{
				if (asPercentage) {
					mixAlpha = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { mixAlpha = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}
void TSet_FX_GaussianBlur::initializeData(CColor FGColor, CColor BGColor)
{
}