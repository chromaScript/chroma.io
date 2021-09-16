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
#include "../../include/toolSettings/method/TSet_Rake.h"

////////////////////////////////////////////////////////////////
// Rake Settings
////////////////////////////////////////////////////////////////

TSet_Rake::TSet_Rake()
{
	this->type = TSetType::rake;
	this->isEnabled = false;
	this->size = 100.0f;
	this->sizeRelative = true;
	this->pushToCanvasEdge = false;
	//TSetController sizeControl;
	//TSetNoise sizeNoise;
	this->usePressure_size = true;
	this->sizeTotal_min = 60.0f;
	this->sizeTotal_max = 90.0f;
	this->sizeTotal_bias = 0.0f;
	this->sizeNoise_min = 0.3f;
	this->sizeNoise_max = -0.3f;
	this->sizeNoise_bias = 0.0f;

	this->useDirection = true;
	this->splineOffsetX_min = 0.0f;
	this->splineOffsetX_max = 0.0f;
	this->splineOffsetY_min = 0.0f;
	this->splineOffsetY_max = 0.0f;
	this->rotation = 0.0f;
	//TSetController rotationControl;
	//TSetNoise rotationNoise;
	this->usePressure_rotation = false;
	this->rotationTotal_min = 0.0f;
	this->rotationTotal_max = 0.0f;
	this->rotationNoise_min = 0.0f;
	this->rotationNoise_max = 0.0f;

	// Whether points are placed from 0 to 1 or 1 to 0, or from -1 to 0/0 to 1 mirrored
	// when determining points position along the rake path
	this->mirrorPoints = true;
	this->flipPoints = false;
	this->pointsCount = 1;
	// The distribution of points along the line. Default is uniform, although other
	// non-linear distributions are available. Mirroring scales the distribution by 1/2 and flips it
	this->distribution = TSetProp::uniform;
	// Below this would be variables to control various input factors for distribution

// Random point distribution
	this->pointOffsetX_usePressure = false;
	this->pointOffsetX_min = 0.0f;
	this->pointOffsetX_max = 0.0f;
	this->pointOffsetY_usePressure = false;
	this->pointOffsetY_min = 0.0f;
	this->pointOffsetY_max = 0.0f;

	// Set the rake shape that gets placed along the input spline. This shape is not able to be mirrored
	// Linear / Arrow / Arc / Cone (Scatter)
	this->rakeShape = TSetProp::linear;
	this->shape_arrow_usePressure = false;
	this->shape_arrow_angleMin = 0.0f;
	this->shape_arrow_angleMax = 0.0f;
	this->shape_bend_usePressure = false;
	this->shape_bend_angleMin = 0.0f;
	this->shape_bend_angleMax = 0.0f;
	this->shape_cone_angleUsePressure = false;
	this->shape_cone_angleMin = 0.0f;
	this->shape_cone_angleMax = 0.0f;
	this->shape_cone_radiusUsePressure = false;
	this->shape_cone_radiusMin = 10.0f;
	this->shape_cone_radiusMax = 10.0f;

	// No Connect / use Given / Along Axis / Along Points / Along Points Shuffle
	this->connectionOrdering = TSetProp::axis;
}

std::shared_ptr<CObject> TSet_Rake::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	// Size and Options
	if (settingsSig >= 5600 && settingsSig < 5610)
	{
		if (settingsSig == 5600)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(size, settingsSig, subSig, "size")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(size, TSET_RAKE_SIZE_MIN, TSET_RAKE_SIZE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(size)); }
			}
			else
			{
				if (asPercentage) {
					size = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_SIZE_MIN, TSET_RAKE_SIZE_MAX, false);
				}
				else { size = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_SIZE_MIN, TSET_RAKE_SIZE_MAX); }
			}
		}
		else if (settingsSig == 5601)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(sizeRelative, settingsSig, subSig, "sizeRelative")); }
				return std::make_shared<CObject>(sizeRelative);
			}
			else
			{
				sizeRelative = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5602)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(pushToCanvasEdge, settingsSig, subSig, "pushToCanvasEdge")); }
				return std::make_shared<CObject>(pushToCanvasEdge);
			}
			else
			{
				pushToCanvasEdge = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5603)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(usePressure_size, settingsSig, subSig, "usePressure_size")); }
				return std::make_shared<CObject>(usePressure_size);
			}
			else
			{
				usePressure_size = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5604)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(sizeTotal_min, settingsSig, subSig, "sizeTotal_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(sizeTotal_min, TSET_RAKE_SIZE_MIN, TSET_RAKE_SIZE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(sizeTotal_min)); }
			}
			else
			{
				if (asPercentage) {
					sizeTotal_min = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_SIZE_MIN, TSET_RAKE_SIZE_MAX, false);
				}
				else { sizeTotal_min = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_SIZE_MIN, TSET_RAKE_SIZE_MAX); }
			}
		}
		else if (settingsSig == 5605)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(sizeTotal_max, settingsSig, subSig, "sizeTotal_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(sizeTotal_max, TSET_RAKE_SIZE_MIN, TSET_RAKE_SIZE_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(sizeTotal_max)); }
			}
			else
			{
				if (asPercentage) {
					sizeTotal_max = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_SIZE_MIN, TSET_RAKE_SIZE_MAX, false);
				}
				else { sizeTotal_max = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_SIZE_MIN, TSET_RAKE_SIZE_MAX); }
			}
		}
		else if (settingsSig == 5606)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(sizeTotal_bias, settingsSig, subSig, "sizeTotal_bias")); }
				return std::make_shared<CObject>(double(sizeTotal_bias));
			}
			else
			{
				sizeTotal_bias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
			}
		}
		else if (settingsSig == 5607)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(sizeNoise_min, settingsSig, subSig, "sizeNoise_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(sizeNoise_min, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(sizeNoise_min)); }
			}
			else
			{
				if (asPercentage) {
					sizeNoise_min = percentRange(
						(float)std::get<double>(object.get()->obj), -1.0f, 1.0f, false);
				}
				else { sizeNoise_min = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
		}
		else if (settingsSig == 5608)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(sizeNoise_max, settingsSig, subSig, "sizeNoise_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(sizeNoise_max, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(sizeNoise_max)); }
			}
			else
			{
				if (asPercentage) {
					sizeNoise_max = percentRange(
						(float)std::get<double>(object.get()->obj), -1.0f, 1.0f, false);
				}
				else { sizeNoise_max = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
		}
		else if (settingsSig == 5609)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(sizeNoise_bias, settingsSig, subSig, "sizeNoise_bias")); }
				return std::make_shared<CObject>(double(sizeNoise_bias));
			}
			else
			{
				sizeNoise_bias = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f);
			}
		}
	}
	// Direction and Rotation Modulation
	else if (settingsSig >= 5610 && settingsSig < 5630)
	{
		if (settingsSig == 5610)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(useDirection, settingsSig, subSig, "useDirection")); }
				return std::make_shared<CObject>(useDirection);
			}
			else
			{
				useDirection = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5611)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineOffsetX_min, settingsSig, subSig, "splineOffsetX_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(splineOffsetX_min, TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(splineOffsetX_min)); }
			}
			else
			{
				if (asPercentage) {
					splineOffsetX_min = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, false);
				}
				else { splineOffsetX_min = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX); }
			}
		}
		else if (settingsSig == 5612)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineOffsetX_max, settingsSig, subSig, "splineOffsetX_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(splineOffsetX_max, TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(splineOffsetX_max)); }
			}
			else
			{
				if (asPercentage) {
					splineOffsetX_max = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, false);
				}
				else { splineOffsetX_max = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX); }
			}
		}
		else if (settingsSig == 5613)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineOffsetY_min, settingsSig, subSig, "splineOffsetY_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(splineOffsetY_min, TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(splineOffsetY_min)); }
			}
			else
			{
				if (asPercentage) {
					splineOffsetY_min = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, false);
				}
				else { splineOffsetY_min = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX); }
			}
		}
		else if (settingsSig == 5614)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineOffsetY_max, settingsSig, subSig, "splineOffsetY_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(splineOffsetY_max, TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(splineOffsetY_max)); }
			}
			else
			{
				if (asPercentage) {
					splineOffsetY_max = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, false);
				}
				else { splineOffsetY_max = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX); }
			}
		}
		else if (settingsSig == 5615)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(rotation, settingsSig, subSig, "rotation")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(rotation, TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(rotation)); }
			}
			else
			{
				if (asPercentage) {
					rotation = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, false);
				}
				else { rotation = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX); }
			}
		}
		else if (settingsSig == 5616)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(usePressure_rotation, settingsSig, subSig, "usePressure_rotation")); }
				return std::make_shared<CObject>(usePressure_rotation);
			}
			else
			{
				usePressure_rotation = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5617)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(rotationTotal_min, settingsSig, subSig, "rotationTotal_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(rotationTotal_min, TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(rotationTotal_min)); }
			}
			else
			{
				if (asPercentage) {
					rotationTotal_min = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, false);
				}
				else { rotationTotal_min = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX); }
			}
		}
		else if (settingsSig == 5618)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(rotationTotal_max, settingsSig, subSig, "rotationTotal_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(rotationTotal_max, TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(rotationTotal_max)); }
			}
			else
			{
				if (asPercentage) {
					rotationTotal_max = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, false);
				}
				else { rotationTotal_max = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX); }
			}
		}
		else if (settingsSig == 5619)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(rotationNoise_min, settingsSig, subSig, "rotationNoise_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(rotationNoise_min, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(rotationNoise_min)); }
			}
			else
			{
				if (asPercentage) {
					rotationNoise_min = percentRange(
						(float)std::get<double>(object.get()->obj), -1.0f, 1.0f, false);
				}
				else { rotationNoise_min = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
		}
		else if (settingsSig == 5620)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(rotationNoise_max, settingsSig, subSig, "rotationNoise_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(rotationNoise_max, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(rotationNoise_max)); }
			}
			else
			{
				if (asPercentage) {
					rotationNoise_max = percentRange(
						(float)std::get<double>(object.get()->obj), -1.0f, 1.0f, false);
				}
				else { rotationNoise_max = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
		}
	}
	// Distribution & Point Offset
	else if (settingsSig >= 5630 && settingsSig < 5650)
	{
		if (settingsSig == 5630)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(mirrorPoints, settingsSig, subSig, "mirrorPoints")); }
				return std::make_shared<CObject>(mirrorPoints);
			}
			else
			{
				mirrorPoints = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5631)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(flipPoints, settingsSig, subSig, "flipPoints")); }
				return std::make_shared<CObject>(flipPoints);
			}
			else
			{
				flipPoints = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5632)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(pointsCount, settingsSig, subSig, "pointsCount", false)); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(pointsCount, TSET_RAKE_POINTS_MIN, TSET_RAKE_POINTS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(pointsCount)); }
			}
			else
			{
				if (asPercentage) {
					pointsCount = (int)std::round(lerpf(percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false), TSET_RAKE_POINTS_MIN, TSET_RAKE_POINTS_MAX));
				}
				else { pointsCount = clampi(std::round(std::get<double>(object.get()->obj)), TSET_RAKE_POINTS_MIN, TSET_RAKE_POINTS_MAX); }
			}
		}
		else if (settingsSig == 5635)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(distribution, settingsSig, subSig, "distribution")); }
				return std::make_shared<CObject>(TSetPropToString(distribution));
			}
			else
			{
				distribution = stringToTSetProp(std::get<std::string>(object.get()->obj));
				switch (distribution)
				{
				case TSetProp::uniform:
					break;
				default:
					distribution = TSetProp::uniform;
				}
			}
		}
		else if (settingsSig == 5640)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(pointOffsetX_usePressure, settingsSig, subSig, "pointOffsetX_usePressure")); }
				return std::make_shared<CObject>(pointOffsetX_usePressure);
			}
			else
			{
				pointOffsetX_usePressure = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5641)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(pointOffsetX_min, settingsSig, subSig, "pointOffsetX_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(pointOffsetX_min, TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(pointOffsetX_min)); }
			}
			else
			{
				if (asPercentage) {
					pointOffsetX_min = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, false);
				}
				else { pointOffsetX_min = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX); }
			}
		}
		else if (settingsSig == 5642)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(pointOffsetX_max, settingsSig, subSig, "pointOffsetX_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(pointOffsetX_max, TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(pointOffsetX_max)); }
			}
			else
			{
				if (asPercentage) {
					pointOffsetX_max = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, false);
				}
				else { pointOffsetX_max = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX); }
			}
		}
		else if (settingsSig == 5643)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(pointOffsetY_usePressure, settingsSig, subSig, "pointOffsetY_usePressure")); }
				return std::make_shared<CObject>(pointOffsetY_usePressure);
			}
			else
			{
				pointOffsetY_usePressure = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5644)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(pointOffsetY_min, settingsSig, subSig, "pointOffsetY_min")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(pointOffsetY_min, TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(pointOffsetY_min)); }
			}
			else
			{
				if (asPercentage) {
					pointOffsetY_min = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, false);
				}
				else { pointOffsetY_min = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX); }
			}
		}
		else if (settingsSig == 5645)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(pointOffsetY_max, settingsSig, subSig, "pointOffsetY_max")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(pointOffsetY_max, TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(pointOffsetY_max)); }
			}
			else
			{
				if (asPercentage) {
					pointOffsetY_max = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, false);
				}
				else { pointOffsetY_max = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX); }
			}
		}
	}
	// Rake Shape & Settings
	else if (settingsSig >= 5650 && settingsSig < 5670)
	{
		if (settingsSig == 5650)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(rakeShape, settingsSig, subSig, "rakeShape")); }
				return std::make_shared<CObject>(TSetPropToString(rakeShape));
			}
			else
			{
				rakeShape = stringToTSetProp(std::get<std::string>(object.get()->obj));
				switch (rakeShape)
				{
				case TSetProp::linear:
				case TSetProp::arrow:
				case TSetProp::arc:
				case TSetProp::cone:
					break;
				default:
					rakeShape = TSetProp::linear;
				}
			}
		}
		else if (settingsSig == 5651)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_arrow_usePressure, settingsSig, subSig, "shape_arrow_usePressure")); }
				return std::make_shared<CObject>(shape_arrow_usePressure);
			}
			else
			{
				shape_arrow_usePressure = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5652)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_arrow_angleMin, settingsSig, subSig, "shape_arrow_angleMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(shape_arrow_angleMin, TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(shape_arrow_angleMin)); }
			}
			else
			{
				if (asPercentage) {
					shape_arrow_angleMin = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, false);
				}
				else { shape_arrow_angleMin = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX); }
			}
		}
		else if (settingsSig == 5653)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_arrow_angleMax, settingsSig, subSig, "shape_arrow_angleMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(shape_arrow_angleMax, TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(shape_arrow_angleMax)); }
			}
			else
			{
				if (asPercentage) {
					shape_arrow_angleMax = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, false);
				}
				else { shape_arrow_angleMax = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX); }
			}
		}
		else if (settingsSig == 5654)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_bend_usePressure, settingsSig, subSig, "shape_bend_usePressure")); }
				return std::make_shared<CObject>(shape_bend_usePressure);
			}
			else
			{
				shape_bend_usePressure = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5655)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_bend_angleMin, settingsSig, subSig, "shape_bend_angleMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(shape_bend_angleMin, TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(shape_bend_angleMin)); }
			}
			else
			{
				if (asPercentage) {
					shape_bend_angleMin = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, false);
				}
				else { shape_bend_angleMin = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX); }
			}
		}
		else if (settingsSig == 5656)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_bend_angleMax, settingsSig, subSig, "shape_bend_angleMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(shape_bend_angleMax, TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(shape_bend_angleMax)); }
			}
			else
			{
				if (asPercentage) {
					shape_bend_angleMax = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, false);
				}
				else { shape_bend_angleMax = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX); }
			}
		}
		else if (settingsSig == 5657)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_cone_angleUsePressure, settingsSig, subSig, "shape_cone_angleUsePressure")); }
				return std::make_shared<CObject>(shape_cone_angleUsePressure);
			}
			else
			{
				shape_cone_angleUsePressure = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5658)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_cone_angleMin, settingsSig, subSig, "shape_cone_angleMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(shape_cone_angleMin, TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(shape_cone_angleMin)); }
			}
			else
			{
				if (asPercentage) {
					shape_cone_angleMin = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, false);
				}
				else { shape_cone_angleMin = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX); }
			}
		}
		else if (settingsSig == 5659)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_cone_angleMax, settingsSig, subSig, "shape_cone_angleMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(shape_cone_angleMax, TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(shape_cone_angleMax)); }
			}
			else
			{
				if (asPercentage) {
					shape_cone_angleMax = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX, false);
				}
				else { shape_cone_angleMax = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_ROTATION_MIN, TSET_RAKE_ROTATION_MAX); }
			}
		}
		else if (settingsSig == 5660)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_cone_radiusUsePressure, settingsSig, subSig, "shape_cone_radiusUsePressure")); }
				return std::make_shared<CObject>(shape_cone_radiusUsePressure);
			}
			else
			{
				shape_cone_radiusUsePressure = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 5661)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_cone_radiusMin, settingsSig, subSig, "shape_cone_radiusMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(shape_cone_radiusMin, TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(shape_cone_radiusMin)); }
			}
			else
			{
				if (asPercentage) {
					shape_cone_radiusMin = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, false);
				}
				else { shape_cone_radiusMin = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX); }
			}
		}
		else if (settingsSig == 5662)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(shape_cone_radiusMax, settingsSig, subSig, "shape_cone_radiusMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(shape_cone_radiusMax, TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, true)));
				}
				else { return std::make_shared<CObject>(double(shape_cone_radiusMax)); }
			}
			else
			{
				if (asPercentage) {
					shape_cone_radiusMax = percentRange(
						(float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX, false);
				}
				else { shape_cone_radiusMax = clampf((float)std::get<double>(object.get()->obj), TSET_RAKE_OFFSETPOS_MIN, TSET_RAKE_OFFSETPOS_MAX); }
			}
		}
	}
	// Connection ordering
	else if (settingsSig >= 5670 && settingsSig < 5680)
	{
		if (settingsSig == 5670)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(connectionOrdering, settingsSig, subSig, "connectionOrdering")); }
				return std::make_shared<CObject>(TSetPropToString(connectionOrdering));
			}
			else
			{
				connectionOrdering = stringToTSetProp(std::get<std::string>(object.get()->obj));
				switch (connectionOrdering)
				{
				case TSetProp::useGiven:
				case TSetProp::points:
				case TSetProp::pointsShuffle:
				case TSetProp::axis:
					break;
				default:
					connectionOrdering = TSetProp::axis;
				}
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}