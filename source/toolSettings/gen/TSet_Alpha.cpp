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
#include "../../include/toolSettings/gen/TSet_Alpha.h"

////////////////////////////////////////////////////////////////
// Alpha Dynamics
////////////////////////////////////////////////////////////////

TSet_Alpha::TSet_Alpha()
{
	this->type = TSetType::alpha;
	this->isEnabled = false;
	this->usePressureOpacity = true;
	this->minOpacity = 0.0f;
	this->maxOpacity = 1.0f;
	this->opacityBias = 0.0f;
	this->usePressureFlow = true;
	this->minFlow = 0.2f;
	this->maxFlow = 0.8f;
	this->flowBias = 0.0f;
}
std::shared_ptr<CObject> TSet_Alpha::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig == 2800)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(usePressureOpacity, settingsSig, subSig, "usePressureOpacity")); }
			return std::make_shared<CObject>(usePressureOpacity);
		}
		else
		{
			usePressureOpacity = std::get<bool>(object.get()->obj);
		}
	}
	else if (settingsSig == 2801)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(minOpacity, settingsSig, subSig, "minOpacity")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(minOpacity, TSET_ALPHA_OPACITY_MIN, TSET_ALPHA_OPACITY_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(minOpacity)); }
		}
		else
		{
			if (asPercentage) {
				minOpacity = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_ALPHA_OPACITY_MIN, TSET_ALPHA_OPACITY_MAX, false);
			}
			else { minOpacity = (float)std::get<double>(object.get()->obj); }
		}
	}
	else if (settingsSig == 2802)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(maxOpacity, settingsSig, subSig, "maxOpacity")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(maxOpacity, TSET_ALPHA_OPACITY_MIN, TSET_ALPHA_OPACITY_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(maxOpacity)); }
		}
		else
		{
			if (asPercentage) {
				maxOpacity = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_ALPHA_OPACITY_MIN, TSET_ALPHA_OPACITY_MAX, false);
			}
			else { maxOpacity = (float)std::get<double>(object.get()->obj); }
		}
	}
	else if (settingsSig == 2803)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(opacityBias, settingsSig, subSig, "opacityBias")); }
			return std::make_shared<CObject>(double(opacityBias));
		}
		else
		{
			opacityBias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
		}
	}
	else if (settingsSig == 2810)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(usePressureFlow, settingsSig, subSig, "usePressureFlow")); }
			if (asPercentage) {}
			else {}
			return std::make_shared<CObject>(usePressureFlow);
		}
		else
		{
			if (asPercentage) {}
			else {}
			usePressureFlow = std::get<bool>(object.get()->obj);
		}
	}
	else if (settingsSig == 2811)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(minFlow, settingsSig, subSig, "minFlow")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(minFlow, TSET_ALPHA_FLOW_MIN, TSET_ALPHA_FLOW_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(minFlow)); }
		}
		else
		{
			if (asPercentage) {
				minFlow = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_ALPHA_FLOW_MIN, TSET_ALPHA_FLOW_MAX, false);
			}
			else { minFlow = (float)std::get<double>(object.get()->obj); }
		}
	}
	else if (settingsSig == 2812)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(maxFlow, settingsSig, subSig, "maxFlow")); }
			if (asPercentage) {
				return std::make_shared<CObject>(double(
					percentRange(maxFlow, TSET_ALPHA_FLOW_MIN, TSET_ALPHA_FLOW_MAX, true)));
			}
			else { return std::make_shared<CObject>(double(maxFlow)); }
		}
		else
		{
			if (asPercentage) {
				maxFlow = percentRange(
					(float)std::get<double>(object.get()->obj), TSET_ALPHA_FLOW_MIN, TSET_ALPHA_FLOW_MAX, false);
			}
			else { maxFlow = (float)std::get<double>(object.get()->obj); }
		}
	}
	else if (settingsSig == 2813)
	{
		if (isGet)
		{
			if (asString) { return std::make_shared<CObject>(makeSettingString(flowBias, settingsSig, subSig, "flowBias")); }
			return std::make_shared<CObject>(double(flowBias));
		}
		else
		{
			flowBias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

void TSet_Alpha::modulateAlpha(float& outOpacity, float& outFlow, glm::vec3 dir, float pressure, float rotation, float tiltX, float tiltY, float velocity)
{
	if (usePressureOpacity)
	{
		outOpacity = lerpf(minOpacity, maxOpacity, pressure);
	}
	if (usePressureFlow)
	{
		outFlow = lerpf(minFlow, maxFlow, pressure);
	}
}