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
#include "../../include/toolSettings/controls/TSet_Zoom.h"

////////////////////////////////////////////////////////////////
// Zoom
////////////////////////////////////////////////////////////////

TSet_Zoom::TSet_Zoom()
{
	this->type = TSetType::zoom;
	this->isEnabled = true;
	this->scrubThreshold = 16; // 200
	this->minZoomFactor = 50.0f; // 201
	this->maxZoomFactor = 1500.0f; // 202
	this->stepZoomAmount = 200.0f; // 210
	this->zoomSpeed = 5.0f; // 220
	this->zoomFactor_A = 0.3f; // 221
}
std::shared_ptr<CObject> TSet_Zoom::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig == 200)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(scrubThreshold, settingsSig, subSig, "scrubThreshold", false)); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange_squared(scrubThreshold, TSET_ZOOM_SCRUBTHRESHOLD_MIN, TSET_ZOOM_SCRUBTHRESHOLD_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(scrubThreshold)); }
		}
		else
		{
			if (asPercentage) {
				scrubThreshold = (int)std::round(
					percentRange_squared(std::get<double>(object.get()->obj), TSET_ZOOM_SCRUBTHRESHOLD_MIN, TSET_ZOOM_SCRUBTHRESHOLD_MAX, false));
			}
			else {
				scrubThreshold = clampi(
					(int)std::round(std::get<double>(object.get()->obj)), TSET_ZOOM_SCRUBTHRESHOLD_MIN, TSET_ZOOM_SCRUBTHRESHOLD_MAX);
			}
		}
	}
	else if (settingsSig == 201)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(minZoomFactor, settingsSig, subSig, "minZoomFactor")); }
			if (asPercentage)
			{
				return std::make_shared<CObject>(double(
					percentRange_squared(minZoomFactor, TSET_ZOOM_ZOOMFACTOR_MIN, TSET_ZOOM_ZOOMFACTOR_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(minZoomFactor)); }

		}
		else
		{
			if (asPercentage) {
				minZoomFactor = percentRange_squared(std::get<double>(object.get()->obj), TSET_ZOOM_ZOOMFACTOR_MIN, TSET_ZOOM_ZOOMFACTOR_MAX, false);
			}
			else { minZoomFactor = std::get<double>(object.get()->obj); }
		}
	}
	else if (settingsSig == 202)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(maxZoomFactor, settingsSig, subSig, "maxZoomFactor")); }
			if (asPercentage)
			{
				return std::make_shared<CObject>(double(
					percentRange_squared(maxZoomFactor, TSET_ZOOM_ZOOMFACTOR_MIN, TSET_ZOOM_ZOOMFACTOR_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(maxZoomFactor)); }

		}
		else
		{
			if (asPercentage) {
				maxZoomFactor = percentRange_squared(std::get<double>(object.get()->obj), TSET_ZOOM_ZOOMFACTOR_MIN, TSET_ZOOM_ZOOMFACTOR_MAX, false);
			}
			else { maxZoomFactor = std::get<double>(object.get()->obj); }
		}
	}
	else if (settingsSig == 210)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(stepZoomAmount, settingsSig, subSig, "stepZoomAmount")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange_squared(stepZoomAmount, TSET_ZOOM_STEPAMOUNT_MIN, TSET_ZOOM_STEPAMOUNT_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(stepZoomAmount)); }

		}
		else
		{
			if (asPercentage) {
				stepZoomAmount = percentRange_squared(
					std::get<double>(object.get()->obj), TSET_ZOOM_STEPAMOUNT_MIN, TSET_ZOOM_STEPAMOUNT_MAX, false);
			}
			else { stepZoomAmount = clampf((float)std::get<double>(object.get()->obj), 10.0f, 1000.0f); }
		}
	}
	else if (settingsSig == 220)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(zoomSpeed, settingsSig, subSig, "zoomSpeed")); }
			if (asPercentage) { return std::make_shared<CObject>(double(percentRange_cubed(zoomSpeed, TSET_ZOOM_SPEED_MIN, TSET_ZOOM_SPEED_MAX, true))); }
			else { return std::make_shared<CObject>(double(zoomSpeed)); }
		}
		else
		{
			if (asPercentage) { zoomSpeed = percentRange_cubed(std::get<double>(object.get()->obj), TSET_ZOOM_SPEED_MIN, TSET_ZOOM_SPEED_MAX, false); }
			else { zoomSpeed = clampf((float)std::get<double>(object.get()->obj), 0.01f, 30.0f); }
		}
	}
	else if (settingsSig == 221)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(zoomFactor_A, settingsSig, subSig, "zoomFactor_A")); }
			if (asPercentage) {}
			else {}
			return std::make_shared<CObject>(double(zoomFactor_A));
		}
		else
		{
			if (asPercentage) {}
			else {}
			zoomFactor_A = (float)std::get<double>(object.get()->obj);
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}