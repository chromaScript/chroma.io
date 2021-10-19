#include "../include/tool/ToolSettings.h"
#include "../include/input/keys.h"
#include "../include/math/Color.h"
#include "../include/math/noise.h"
#include "../include/math/blend.h"

class CFunction;
#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CObject.h"
#include "../include/cscript/CToken.h"
#include "../include/tool/Tool.h"
#include "../include/entities/widgets/Widget.h"
#include "../include/entities/widgets/Graph_ToolControl.h"
#include "../include/Application.h"
#include "../include/tool/Toolbox.h"
#include "../include/methods/MethodType.h"

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>
#include <string>
#include <vector>
#include <memory>
#include <random>
#include <cmath>
#include <deque>

extern std::shared_ptr<Application> chromaIO;

////////////////////////////////////////////////////////////////
// Settings Global
////////////////////////////////////////////////////////////////
std::string makeSettingString(Keybind value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + keybindToString(value) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(MethodType value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += methodTypeStringMap.at(value);
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(int value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += std::to_string(value);
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(float value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += std::to_string(value);
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(bool value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	if (value) { out += "\"true\""; }
	else { out += "\"false\""; }
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::string value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + value + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::vector<std::string> value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + stringVecToString(value, false) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::vector<int> value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("array num") + intVecToString(value) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::vector<float> vector, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("array num") + floatVecToString(vector) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(float* vector, int size, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("array num") + floatArrayToString(vector, size) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(glm::vec2 value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("vec2") + vec2ToString(value) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(BlendMode value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + BlendModeToString(value) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(TSetProp value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + TSetPropToString(value) + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(TSetController value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("NULL") + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(TSetNoise value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("NULL") + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(TSetGraph value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("NULL") + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::shared_ptr<CFunction> value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	out += "\"" + std::string("NULL") + "\"";
	out += TSETTINGSWRITER_P5;
	return out;
}
std::string makeSettingString(std::shared_ptr<CObject> value, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
	out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
	std::string valueToString = "";
	CLiteralTypes valType = value.get()->objType.type;
	if (valType == CLiteralTypes::_CString_Array)
	{
		std::vector<std::string> stringVec;
		std::vector<std::shared_ptr<CObject>> objVec = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(value.get()->obj).get();
		for (std::shared_ptr<CObject> obj : objVec)
		{
			stringVec.push_back(std::get<std::string>(obj.get()->obj));
		}
		out = makeSettingString(stringVec, settingsSig, subSig, settingName);
	}
	else
	{
		out += TSETTINGSWRITER_P1 + std::to_string(settingsSig) + TSETTINGSWRITER_P2 + std::to_string(subSig);
		out += TSETTINGSWRITER_P3 + settingName + TSETTINGSWRITER_P4;
		out += "\"" + std::string("NULL") + "\"";
		out += TSETTINGSWRITER_P5;
	}
	return out;
}
std::string makeSettingString(std::shared_ptr<Tool> target, TSetControl_Node* controlNode, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 0, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 1, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 2, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 3, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 4, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 10, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 11, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 12, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 13, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 14, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controlNode->putProperty(nullptr, target, settingsSig, 15, nullptr, true, false, true).get()->obj);
	return out;
}
std::string makeSettingString(std::shared_ptr<Tool> target, TSetController* controller, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += std::get<std::string>(controller->putProperty(nullptr, target, settingsSig, 2, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controller->putProperty(nullptr, target, settingsSig, 3, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controller->putProperty(nullptr, target, settingsSig, 4, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controller->putProperty(nullptr, target, settingsSig, 5, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controller->putProperty(nullptr, target, settingsSig, 600, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(controller->putProperty(nullptr, target, settingsSig, 601, nullptr, true, false, true).get()->obj);
	controller->pressureGraph.updateControlPoints();
	out += makeSettingString(target, &controller->pressureGraph, settingsSig, -1, "PRESSURE");
	controller->directionGraph.updateControlPoints();
	out += makeSettingString(target, &controller->directionGraph, settingsSig, -1, "DIRECTION");
	controller->tiltGraph.updateControlPoints();
	out += makeSettingString(target, &controller->tiltGraph, settingsSig, -1, "TILT");
	controller->velocityGraph.updateControlPoints();
	out += makeSettingString(target, &controller->velocityGraph, settingsSig, -1, "VELOCITY");
	controller->rotationGraph.updateControlPoints();
	out += makeSettingString(target, &controller->rotationGraph, settingsSig, -1, "ROTATION");
	return out;
}
std::string makeSettingString(std::shared_ptr<Tool> target, TSetGraph* graph, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += std::get<std::string>(graph->putProperty(nullptr, target, settingsSig, 0, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(graph->putProperty(nullptr, target, settingsSig, 1, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(graph->putProperty(nullptr, target, settingsSig, 2, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(graph->putProperty(nullptr, target, settingsSig, 3, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(graph->putProperty(nullptr, target, settingsSig, 4, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(graph->putProperty(nullptr, target, settingsSig, 10, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(graph->putProperty(nullptr, target, settingsSig, 50, nullptr, true, false, true).get()->obj);
	return out;
}
std::string makeSettingString(std::shared_ptr<Tool> target, TSetNoise* noiseNode, int settingsSig, int subSig, std::string settingName)
{
	std::string out = "";
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 0, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 1, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 3, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 4, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 6, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 7, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 8, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 9, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 10, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 11, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 12, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 13, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 14, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 15, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 16, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 17, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 18, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 20, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 21, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 22, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 23, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 24, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 25, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 26, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 27, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 28, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 29, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 30, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 31, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 32, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 33, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 34, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 35, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 36, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 37, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 38, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 39, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 40, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 41, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 42, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 43, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 50, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 51, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 52, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 53, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 60, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 70, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 71, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 72, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 73, nullptr, true, false, true).get()->obj);
	out += std::get<std::string>(noiseNode->putProperty(nullptr, target, settingsSig, 74, nullptr, true, false, true).get()->obj);
	return out;
}

////////////////////////////////////////////////////////////////
// Graph Functions
////////////////////////////////////////////////////////////////

TSetGraph::TSetGraph(int subSigOffset)
{
	this->subSigOffset = subSigOffset;
}

std::shared_ptr<CObject> TSetGraph::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (subSig >= 0 && subSig < 10)
	{
		if (subSig == 0) {
			if (!isGet) { presetName = std::get<std::string>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(presetName, settingsSig, subSig + subSigOffset, "presetName")); }
				return std::make_shared<CObject>(presetName);
			}
		}
		else if (subSig == 1) {
			if (!isGet) { start = std::get<glm::dvec2>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(start, settingsSig, subSig + subSigOffset, "start")); }
				return std::make_shared<CObject>(glm::dvec2(start.x, start.y));
			}
		}
		else if (subSig == 2) {
			if (!isGet) { end = std::get<glm::dvec2>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(end, settingsSig, subSig + subSigOffset, "end")); }
				return std::make_shared<CObject>(glm::dvec2(end.x, end.y));
			}
		}
		else if (subSig == 3) {
			if (!isGet) { handleCount = (int)std::get<double>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(handleCount, settingsSig, subSig + subSigOffset, "handleCount")); }
				return std::make_shared<CObject>((double)handleCount);
			}
		}
		else if (subSig == 4) {
			if (!isGet) {
				if (asPercentage) {
					dampeningValue = percentRange_squared(
						(float)std::get<double>(object.get()->obj), 0.01f, 6.0f, false);
				}
				else { dampeningValue = clampf((float)std::get<double>(object.get()->obj), 0.01f, 6.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(dampeningValue, settingsSig, subSig + subSigOffset, "dampeningValue")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(dampeningValue, 0.01f, 6.0f, true)));
				}
				else { return std::make_shared<CObject>(double(dampeningValue)); }
			}
		}
	}
	else if (subSig >= 10 && subSig < 60)
	{
		if (subSig == 10) {
			if (!isGet) {
				if (object.get()->objType.type == CLiteralTypes::_CNumber_Array) {
					std::vector<std::shared_ptr<CObject>> numArr = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(object.get()->obj);
					if (numArr.size() == 0) { return std::shared_ptr<CObject>(nullptr); }
					for (int i = 0; i < 32; i++) {
						if (i < numArr.size()) {
							controlPoints[i] = (float)std::get<double>(numArr[i].get()->obj);
						}
					}
				}
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(controlPoints, 32, settingsSig, subSig + subSigOffset, "controlPoints")); }
				else {
					std::vector<std::shared_ptr<CObject>> numArr;
					for (int i = 0; i < 32; i++) {
						numArr.push_back(std::make_shared<CObject>(double(controlPoints[i])));
					}
					return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, std::make_shared<std::vector<std::shared_ptr<CObject>>>(numArr));
				}
			}
		}
		else if (subSig == 50) {
			if (!isGet) { flipOutput = (int)std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(flipOutput, settingsSig, subSig + subSigOffset, "flipOutput")); }
				return std::make_shared<CObject>(flipOutput);
			}
		}
	}
	
	return std::shared_ptr<CObject>(nullptr);
}
void TSetGraph::updateControlPoints()
{
	if (end.x < start.x) { flipOutput = true; }
	else { flipOutput = false; }
	for (int i = 1; i < 16; i++)
	{
		if (i > handleCount + 1) {
			controlPoints[i * 2] = controlPoints[(i * 2) + 1] = 0.0f;
		}
	}
	controlPoints[0] = start.x;
	controlPoints[1] = start.y;
	controlPoints[((handleCount + 1) * 2)] = end.x;
	controlPoints[((handleCount + 1) * 2) + 1] = end.y;
}

void TSetGraph::copyWidgetData(Graph_ToolControl* widget)
{
	start = widget->getStartPoint();
	end = widget->getEndPoint();
	if (end.x < start.x) { flipOutput = true; }
	else { flipOutput = false; }

	handleCount = widget->getHandleCount();
	for (int i = 1; i < 16; i++)
	{
		if (i < handleCount + 1)
		{
			controlPoints[i * 2] = widget->getCurvePoints()[i * 2];
			controlPoints[(i * 2) + 1] = widget->getCurvePoints()[(i * 2) + 1];
		}
		else
		{
			controlPoints[i * 2] = controlPoints[(i * 2) + 1] = 0.0f;
		}
	}
	controlPoints[0] = start.x;
	controlPoints[1] = start.y;
	controlPoints[((handleCount + 1) * 2)] = end.x;
	controlPoints[((handleCount + 1) * 2) + 1] = end.y;
}
float TSetGraph::modulateInput(float* input)
{
	int pointsCount = handleCount + 1;
	float returnValue = *input;
	// Start / End positions determine the floor and ceiling input range.
	// Input is re-mapped from the floor/ceil to 0 to 1, and flipped if the
	// end.x position is less than start.x
	float min = min(start.x, end.x);
	float max = max(start.x, end.x);
	returnValue =
		(*input < min) ? 1.0f - (!flipOutput * 1.0f) :
		(*input > max) ? 0.0f + (!flipOutput * 1.0f) :
		lerpf(1.0f - (!flipOutput * 1.0f), 0.0f + (!flipOutput * 1.0f), (*input - min) / (max - min));

	if (pointsCount > 1)
	{
		float i = returnValue;

		float h = 1.0f;
		float u = (1.0f - i);
		glm::vec2 point = glm::vec2(glm::vec2(controlPoints[(0 * 2)], controlPoints[(0 * 2) + 1]));

		if (i != 0.0) {
			if (i <= 0.5) {
				u = i / u;
				for (int k = 1; k <= pointsCount; ++k) {
					float prev_h = h;
					h = h * u * (float(pointsCount + 1) - float(k));
					h = h / (float(k) + h);
					float h1 = (h / prev_h) * ((float(k) * (1.0f - i)) / ((float)i * float(pointsCount - k + 1)));
					point = h1 * point + h * glm::vec2(glm::vec2(controlPoints[(k * 2)], controlPoints[(k * 2) + 1]));
				}
			}
			else {
				u = u / i;
				for (int k = 1; k <= pointsCount; ++k) {
					float prev_h = h;
					h = h * (float(pointsCount + 1) - float(k));
					h = h / (float(k) * u + h);
					float h1 = (h / prev_h) * ((float(k) * (1.0f - i)) / ((float)i * float(pointsCount - k + 1)));
					point = h1 * point + h * glm::vec2(glm::vec2(controlPoints[(k * 2)], controlPoints[(k * 2) + 1]));
				}
			}
		}
		returnValue = point.y;
	}
	// Applies dampening to the end result.
	returnValue = pow(returnValue, dampeningValue);
	return returnValue;
}

void TSetGraph::resetGraph()
{
	flipOutput = false;
	start = glm::vec2(0.0f);
	end = glm::vec2(1.0f);
	for (int i = 0; i < 32; i++)
	{
		controlPoints[i] = 0.0f;
	}
	controlPoints[2] = controlPoints[3] = 1.0f;
	handleCount = 0;
}

void TSetGraph::flipGraph()
{
	start = glm::vec2(1.0f - start.x, 0.0f);
	end = glm::vec2(1.0f - end.x, 1.0f);
	flipOutput = !flipOutput;
	for (int i = 1; i < handleCount + 1; i++)
	{
		controlPoints[i * 2] = 1.0f - controlPoints[i * 2];
	}
	controlPoints[(handleCount + 1) * 2] = end.x;
	controlPoints[((handleCount + 1) * 2) + 1] = end.y;
}

void TSetGraph::shiftGraph(float amountX, float amountY)
{
	for (int i = 1; i < handleCount + 1; i++)
	{
		controlPoints[i * 2] = clampf(controlPoints[i * 2] + amountX, 0.0f, 1.0f);
		controlPoints[(i * 2) + 1] = clampf(controlPoints[(i * 2) + 1] + amountY, 0.0f, 1.0f);
	}
}

void TSetGraph::scaleGraph(float amount)
{
	start = glm::vec2(clampf(start.x * (1.0f + (amount / 100.0f)), 0.0f, 1.0f), 0.0f);
	for (int i = 1; i < handleCount + 1; i++)
	{
		controlPoints[i * 2] = clampf(controlPoints[i * 2] * (1.0f + (amount / 100.0f)), 0.0f, 1.0f);
	}
	end = glm::vec2(clampf(controlPoints[((handleCount + 1) * 2)] * (1.0f + (amount / 100.0f)), 0.0f, 1.0f), 1.0f);
}

////////////////////////////////////////////////////////////////
// Control Node Functions
////////////////////////////////////////////////////////////////

TSetControl_Node::TSetControl_Node(float rangeMin, float rangeMax, bool centerRange, std::string nodeName)
{
	this->nodeName = nodeName;
	this->rangeMin = rangeMin;
	this->rangeMax = rangeMax;
	this->centerRange = centerRange;
	if (centerRange)
	{
		controlMin = controlMax = 0.0f;
	}
	else
	{
		controlMin = rangeMin;
		controlMax = rangeMax;
	}
}

std::shared_ptr<CObject> TSetControl_Node::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	// Node name, isEnabled, lockMode, Control Min/Max
	if (subSig >= 0 && subSig < 10)
	{
		if (subSig == 0) {
			if (!isGet) { /* CANNOT SET VALUE */ }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(nodeName, settingsSig, subSig, "nodeName")); }
				return std::make_shared<CObject>(nodeName);
			}
		}
		else if (subSig == 1) {
			if (!isGet) { isEnabled = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "isEnabled")); }
				return std::make_shared<CObject>(isEnabled);
			}
		}
		else if (subSig == 2) {
			if (!isGet) {
				lockingMode = clampi((int)std::get<double>(object.get()->obj), 0, 2);
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(lockingMode, settingsSig, subSig, "lockingMode")); }
				else { return std::make_shared<CObject>(double(lockingMode)); }
			}
		}
		else if (subSig == 3) {
			if (!isGet) {
				if (asPercentage) {
					if (centerRange) {
						controlMin = percentRange_squaredCenter(
							(float)std::get<double>(object.get()->obj), rangeMin, rangeMax, false);
					}
					else {
						controlMin = percentRange_squared(
							(float)std::get<double>(object.get()->obj), rangeMin, rangeMax, false);
					}
				}
				else { controlMin = clampf((float)std::get<double>(object.get()->obj), rangeMin, rangeMax); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(controlMin, settingsSig, subSig, "controlMin")); }
				if (asPercentage) {
					if (centerRange) {
						return std::make_shared<CObject>(double(
							percentRange_squaredCenter(controlMin, rangeMin, rangeMax, true)));
					}
					else {
						return std::make_shared<CObject>(double(
							percentRange_squared(controlMin, rangeMin, rangeMax, true)));
					}
				}
				else { return std::make_shared<CObject>(double(controlMin)); }
			}
		}
		else if (subSig == 4) {
			if (!isGet) {
				if (asPercentage) {
					if (centerRange) {
						controlMax = percentRange_squaredCenter(
							(float)std::get<double>(object.get()->obj), rangeMin, rangeMax, false);
					}
					else {
						controlMax = percentRange_squared(
							(float)std::get<double>(object.get()->obj), rangeMin, rangeMax, false);
					}
				}
				else { controlMax = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(controlMax, settingsSig, subSig, "controlMax")); }
				if (asPercentage) {
					if (centerRange) {
						return std::make_shared<CObject>(double(
							percentRange_squaredCenter(controlMax, rangeMin, rangeMax, true)));
					}
					else {
						return std::make_shared<CObject>(double(
							percentRange_squared(controlMax, rangeMin, rangeMax, true)));
					}
				}
				else { return std::make_shared<CObject>(double(controlMax)); }
			}
		}
	}
	// Enable/Disable UseGraph Toggles
	else if (subSig >= 10 && subSig <= 15)
	{
		if (subSig == 10) {
			if (!isGet) { usePressure = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(usePressure, settingsSig, subSig, "usePressure")); }
				return std::make_shared<CObject>(usePressure);
			}
		}
		else if (subSig == 11) {
			if (!isGet) { useDirection = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useDirection, settingsSig, subSig, "useDirection")); }
				return std::make_shared<CObject>(useDirection);
			}
		}
		else if (subSig == 12) {
			if (!isGet) { useTilt = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useTilt, settingsSig, subSig, "useTilt")); }
				return std::make_shared<CObject>(useTilt);
			}
		}
		else if (subSig == 13) {
			if (!isGet) { useVelocity = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useVelocity, settingsSig, subSig, "useVelocity")); }
				return std::make_shared<CObject>(useVelocity);
			}
		}
		else if (subSig == 14) {
			if (!isGet) { useRotation = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useRotation, settingsSig, subSig, "useRotation")); }
				return std::make_shared<CObject>(useRotation);
			}
		}
		else if (subSig == 15) {
			if (!isGet) { useFunction = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useFunction, settingsSig, subSig, "useFunction")); }
				return std::make_shared<CObject>(useFunction);
			}
		}
	}
	

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::shared_ptr<CObject>(nullptr);
}

bool TSetControl_Node::reportEnabled()
{
	if (!isEnabled) { return false; }
	if (usePressure || useTilt || useVelocity || useRotation || useDirection || useFunction) { return true; } return false;
}

// The Control Node takes in a set of stylus inputs, and produces a final 0-1 value
void TSetControl_Node::calculateControl(float& outValue,
	glm::vec3* pos, glm::vec3* dir,
	Input* input, TSetController* controller, int* anchorCount)
{
	std::vector<float> results;
	float pressureFinal = 1.0f; float dirFinal = 1.0f; float tiltFinal = 1.0f; float velFinal = 1.0f; float rotFinal = 1.0f;
	if (usePressure) { 
		pressureFinal = controller->pressureGraph.modulateInput(&input->pressure);  
		results.push_back(pressureFinal); 
	}
	if (useDirection) {
		if (controller->useInitialDir && !initialDirFound) {
			if (*anchorCount > watchCount && watchCount == -1) {
				initialDirFound = false;
				watchCount = *anchorCount;
				watchDir = *dir;
			}
			else if (watchCount != -1 && *anchorCount - watchCount > 1) {
				if (!compareVec3_isEqual(dir, &watchDir, 0.2f)) {
					initialDirFound = true;
					initialDir = *dir;
					watchCount = -1;
					watchDir = glm::vec3(1.0f);
				}
			}
		}
		float rad = (controller->useInitialDir) ?
			(initialDirFound) ? std::atan2f(initialDir.x, initialDir.y) : std::atan2(dir->x, dir->y)
			: std::atan2(dir->x, dir->y);
		dirFinal = (clampAngle_360((180.0f * (rad / MATH_PI)) + controller->dirRotationOffset + 90.0f) ) / 360.0f;
		if (loopDirValue) { dirFinal = abs(dirFinal - 0.5f) * 2.0f; }
		results.push_back(controller->directionGraph.modulateInput(&dirFinal)); }
	if (useTilt) { 
		tiltFinal = 1.0f - clampf(max(abs(input->tiltY) / 0.6667f, abs(input->tiltX) / 0.6667f), 0.0f, 1.0f);
		results.push_back(controller->tiltGraph.modulateInput(&tiltFinal)); }
	if (useVelocity) { 
		velFinal = clampf(clampf(input->velocity, 0.0f, 400.0f) / (controller->velocityCap * 400.0f), 0.0f, 1.0f);
		results.push_back(controller->pressureGraph.modulateInput(&velFinal)); }
	if (useRotation) { 
		// Splits the range into two portions that loop at their ends.
		rotFinal = abs(clampAngle_180(input->rotation + controller->rotationOffset) / 180.0f);
		results.push_back(controller->directionGraph.modulateInput(&rotFinal)); }
	// Average the inputs
	if (results.size() != 0)
	{
		outValue = sumVector_float(&results) / results.size();
	}
	if (useFunction && controller->function != nullptr)
	{
		std::vector<std::shared_ptr<CObject>> args;

		args.push_back(std::make_shared<CObject>(double(outValue)));
		args.push_back(std::make_shared<CObject>(double(pos->x)));
		args.push_back(std::make_shared<CObject>(double(-pos->y)));
		args.push_back(std::make_shared<CObject>(double(pressureFinal)));
		args.push_back(std::make_shared<CObject>(double(dirFinal)));
		args.push_back(std::make_shared<CObject>(double(tiltFinal)));
		args.push_back(std::make_shared<CObject>(double(velFinal)));
		args.push_back(std::make_shared<CObject>(double(rotFinal)));
		std::shared_ptr<CObject> returnObj = controller->function.get()->call(chromaIO.get()->scriptConsole.get()->getInterpreter(), &args);
		chromaIO.get()->scriptConsole.get()->getInterpreter()->resetInterpreter();
		if (returnObj != nullptr && returnObj.get()->objType.type == CLiteralTypes::_CNumber)
		{
			outValue = clampf((float)std::get<double>(returnObj.get()->obj), 0.0f, 1.0f);
		}
	}
}

////////////////////////////////////////////////////////////////
// Controller Functions
////////////////////////////////////////////////////////////////

std::shared_ptr<CObject> TSetController::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	// Non Graph Settings
	if (subSig >= 0 && subSig <= 99)
	{
		if (subSig == 2) {
			if (!isGet) {
				if (asPercentage) {
					dirRotationOffset = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -180.0f, 180.0f, false);
				}
				else { dirRotationOffset = clampf((float)std::get<double>(object.get()->obj), -180.0f, 180.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(dirRotationOffset, settingsSig, subSig, "dirRotationOffset")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(dirRotationOffset, -180.0f, 180.0f, true)));
				}
				else { return std::make_shared<CObject>(double(dirRotationOffset)); }
			}
		}
		else if (subSig == 3) {
			if (!isGet) { useInitialDir = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useInitialDir, settingsSig, subSig, "useInitialDir")); }
				return std::make_shared<CObject>(useInitialDir);
			}
		}
		else if (subSig == 4) {
			if (!isGet) {
				if (asPercentage) {
					velocityCap = percentRange_squared(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { velocityCap = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(velocityCap, settingsSig, subSig, "velocityCap")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(velocityCap, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(velocityCap)); }
			}
		}
		else if (subSig == 5) {
			if (!isGet) {
				if (asPercentage) {
					rotationOffset = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -180.0f, 180.0f, false);
				}
				else { rotationOffset = clampf((float)std::get<double>(object.get()->obj), -180.0f, 180.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(rotationOffset, settingsSig, subSig, "rotationOffset")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(rotationOffset, -180.0f, 180.0f, true)));
				}
				else { return std::make_shared<CObject>(double(rotationOffset)); }
			}
		}
	}
	// Pressure Graph
	else if (subSig >= 100 && subSig <= 199)
	{
		return pressureGraph.putProperty(interpreter, owner, settingsSig, subSig % 100, object, isGet, asPercentage, asString);
	}
	// Direction Graph
	else if (subSig >= 200 && subSig <= 299)
	{
		return directionGraph.putProperty(interpreter, owner, settingsSig, subSig % 200, object, isGet, asPercentage, asString);
	}
	// Tilt Graph
	else if (subSig >= 300 && subSig <= 399)
	{
		return tiltGraph.putProperty(interpreter, owner, settingsSig, subSig % 300, object, isGet, asPercentage, asString);
	}
	// Velocity Graph
	else if (subSig >= 400 && subSig <= 499)
	{
		return velocityGraph.putProperty(interpreter, owner, settingsSig, subSig % 400, object, isGet, asPercentage, asString);
	}
	//Rotation Graph
	else if (subSig >= 500 && subSig <= 599)
	{
		return rotationGraph.putProperty(interpreter, owner, settingsSig, subSig % 100, object, isGet, asPercentage, asString);
	}
	// Function
	else if (subSig >= 600 && subSig <= 699)
	{
		if (subSig == 600) {
			if (!isGet) {
				fieldValue = std::get<std::string>(object.get()->obj);
				if (interpreter != nullptr)
				{
					std::shared_ptr<CObject> funcObj = nullptr;
					if (fieldValue.find("::") != std::string::npos) {
						std::vector<std::string> scopeStack;
						std::string address = fieldValue;
						while (address.find("::") != std::string::npos)
						{
							scopeStack.push_back(splitRegularString(address, "::", true));
							address.erase(0, 1);
							std::cout << address << std::endl;
						}
						std::string lookup = address;
						funcObj = interpreter.get()->currentEnvironment.get()->get(scopeStack, lookup);
					}
					else {
						funcObj = interpreter.get()->currentEnvironment.get()->get(fieldValue);
					}
					if (funcObj != nullptr && funcObj.get()->objType.type == CLiteralTypes::_CFunction)
					{
						std::shared_ptr<CFunction> func = std::get<std::shared_ptr<CFunction>>(funcObj.get()->obj);
						if (func.get()->funcDeclaration.get()->returnType.get()->type == CTokenType::NUM)
						{
							std::vector<std::shared_ptr<CToken>> params = func.get()->funcDeclaration.get()->paramsTypes;
							if (params.size() == 8)
							{
								bool validParams = true;
								for (std::shared_ptr<CToken> param : params)
								{
									if (param.get()->type != CTokenType::NUM) { validParams = false; break; }
								}
								if (validParams)
								{
									function = func;
								}
							}
						}
					}
				}
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(fieldValue, settingsSig, subSig, "fieldValue")); }
				return std::make_shared<CObject>(fieldValue);
			}
		}
		if (subSig == 601) {
			if (!isGet) { functionString = std::get<std::string>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(functionString, settingsSig, subSig, "functionString")); }
				return std::make_shared<CObject>(functionString);
			}
		}
	}
	if (asString) { return std::make_shared<CObject>(makeSettingString(false, settingsSig, subSig, "NULL")); }
	return std::shared_ptr<CObject>(nullptr);
}


////////////////////////////////////////////////////////////////
// Noise Functions
////////////////////////////////////////////////////////////////

TSetNoise::TSetNoise(float finalRangeMin, float finalRangeMax, bool centerFinalRange, std::string noiseName)
{
	this->noiseName = noiseName;
	this->finalValMin = finalRangeMin;
	this->finalValMax = finalRangeMax;
	this->centerValRange = centerFinalRange;
	this->noiseMin = finalRangeMin;
	this->noiseMax = finalRangeMax;
	generateRandSeed();
}

std::shared_ptr<CObject> TSetNoise::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	// Generic Settings
	if (subSig >= 0 && subSig < 10)
	{
		if (subSig == 0) {
			if (!isGet) { presetName = std::get<std::string>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(presetName, settingsSig, subSig, "presetName")); }
				return std::make_shared<CObject>(presetName);
			}
		}
		else if (subSig == 1) {
			if (!isGet) { isEnabled = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "isEnabled")); }
				return std::make_shared<CObject>(isEnabled);
			}
		}
		else if (subSig == 3) {
			if (!isGet) { clampNoiseToRange = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(clampNoiseToRange, settingsSig, subSig, "clampNoiseToRange")); }
				return std::make_shared<CObject>(clampNoiseToRange);
			}
		}
		else if (subSig == 4) {
			if (!isGet) { seed = clampi((int)std::get<double>(object.get()->obj), -99999, 99999); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString((int)seed, settingsSig, subSig, "seed")); }
				return std::make_shared<CObject>((double)seed);
			}
		}
		else if (subSig == 6) {
			if (!isGet) { randomSeed = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(randomSeed, settingsSig, subSig, "randomSeed")); }
				return std::make_shared<CObject>(randomSeed);
			}
		}
		else if (subSig == 7) {
			if (!isGet) { totalMixMode = stringToBlendMode(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(totalMixMode, settingsSig, subSig, "totalMixMode")); }
				std::string tsetString = BlendModeToString(totalMixMode);
				return std::make_shared<CObject>(stringToUpper(tsetString));
			}
		}
		// Noise Min/Max
		else if (subSig == 8) {
			if (!isGet) {
				if (asPercentage) {
					if (centerValRange) {
						noiseMin = percentRange_squaredCenter(
							(float)std::get<double>(object.get()->obj), finalValMin, finalValMax, false);
					}
					else {
						noiseMin = percentRange_squared(
							(float)std::get<double>(object.get()->obj), finalValMin, finalValMax, false);
					}
				}
				else { noiseMin = clampf((float)std::get<double>(object.get()->obj), finalValMin, finalValMax); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(noiseMin, settingsSig, subSig, "noiseMin")); }
				if (asPercentage) {
					if (centerValRange) {
						return std::make_shared<CObject>(double(
							percentRange_squaredCenter(noiseMin, finalValMin, finalValMax, true)));
					}
					else {
						return std::make_shared<CObject>(double(
							percentRange_squared(noiseMin, finalValMin, finalValMax, true)));
					}
				}
				else { return std::make_shared<CObject>(double(noiseMin)); }
			}
		}
		else if (subSig == 9) {
			if (!isGet) {
				if (asPercentage) {
					if (centerValRange) {
						noiseMax = percentRange_squaredCenter(
							(float)std::get<double>(object.get()->obj), finalValMin, finalValMax, false);
					}
					else {
						noiseMax = percentRange_squared(
							(float)std::get<double>(object.get()->obj), finalValMin, finalValMax, false);
					}
				}
				else { noiseMax = clampf((float)std::get<double>(object.get()->obj), finalValMin, finalValMax); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(noiseMax, settingsSig, subSig, "noiseMax")); }
				if (asPercentage) {
					if (centerValRange) {
						return std::make_shared<CObject>(double(
							percentRange_squaredCenter(noiseMax, finalValMin, finalValMax, true)));
					}
					else {
						return std::make_shared<CObject>(double(
							percentRange_squared(noiseMax, finalValMin, finalValMax, true)));
					}
				}
				else { return std::make_shared<CObject>(double(noiseMax)); }
			}
		}
	}
	// 1D Noise Settings
	else if (subSig >= 10 && subSig < 20)
	{
		if (subSig == 10) {
			if (!isGet) { useNoise_1D = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useNoise_1D, settingsSig, subSig, "useNoise_1D")); }
				return std::make_shared<CObject>(useNoise_1D);
			}
		}
		else if (subSig == 11) {
			if (!isGet) { intervalMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(intervalMode, settingsSig, subSig, "intervalMode")); }
				return std::make_shared<CObject>(TSetPropToString(intervalMode));
			}
		}
		else if (subSig == 12) {
			if (!isGet) { intervalSmoothing = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(intervalSmoothing, settingsSig, subSig, "intervalSmoothing")); }
				return std::make_shared<CObject>(intervalSmoothing);
			}
		}
		// Weight / Intensity / Smoothing / MixAmount
		else if (subSig == 13) {
			if (!isGet) {
				if (asPercentage) {
					weight = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { weight = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(weight, settingsSig, subSig, "weight")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(weight, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(weight)); }
			}
		}
		else if (subSig == 14) {
			if (!isGet) {
				if (asPercentage) {
					intensity = percentRange_squared(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { intensity = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(intensity, settingsSig, subSig, "intensity")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(intensity, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(intensity)); }
			}
		}
		else if (subSig == 15) {
			if (!isGet) {
				if (asPercentage) {
					smoothing = percentRange_squared(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { smoothing = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(smoothing, settingsSig, subSig, "smoothing")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(smoothing, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(smoothing)); }
			}
		}
		else if (subSig == 16) {
			if (!isGet) {
				if (asPercentage) {
					mixAmount_1D = percentRange_squared(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { mixAmount_1D = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(mixAmount_1D, settingsSig, subSig, "mixAmount_1D")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(mixAmount_1D, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(mixAmount_1D)); }
			}
		}
		else if (subSig == 17) {
			if (!isGet) { noiseMixMode = stringToBlendMode(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(noiseMixMode, settingsSig, subSig, "noiseMixMode")); }
				std::string tsetString = BlendModeToString(noiseMixMode);
				return std::make_shared<CObject>(stringToUpper(tsetString));
			}
		}
		else if (subSig == 18) {
			if (!isGet) { apply1DFirst = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(apply1DFirst, settingsSig, subSig, "apply1DFirst")); }
				return std::make_shared<CObject>(apply1DFirst);
			}
		}
	}
	// 2D Noise Settings
	else if (subSig >= 20 && subSig < 50)
	{
		glm::ivec2 canvasDimensions = chromaIO.get()->getCanvasDimensions();
		if (subSig == 20) {
			if (!isGet) { useNoise_2D = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useNoise_2D, settingsSig, subSig, "useNoise_2D")); }
				return std::make_shared<CObject>(useNoise_2D);
			}
		}
		else if (subSig == 21) {
			if (!isGet) { noiseMode = stringToTSetProp(std::get<std::string>(object.get()->obj)); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(noiseMode, settingsSig, subSig, "noiseMode")); }
				std::string tsetString = TSetPropToString(noiseMode);
				return std::make_shared<CObject>(stringToUpper(tsetString));
			}
		}
		else if (subSig == 22) {
			if (!isGet) { useOriginOffset = std::get<bool>(object.get()->obj); calculateFinalScale(&canvasDimensions); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useOriginOffset, settingsSig, subSig, "useOriginOffset")); }
				return std::make_shared<CObject>(useOriginOffset);
			}
		}
		else if (subSig == 23) {
			if (!isGet) { scaleToCanvas = std::get<bool>(object.get()->obj); calculateFinalScale(&canvasDimensions); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(scaleToCanvas, settingsSig, subSig, "scaleToCanvas")); }
				return std::make_shared<CObject>(scaleToCanvas);
			}
		}
		else if (subSig == 24) {
			if (!isGet) { scaleToToolSize = std::get<bool>(object.get()->obj); calculateFinalScale(&canvasDimensions); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(scaleToToolSize, settingsSig, subSig, "scaleToToolSize")); }
				return std::make_shared<CObject>(scaleToToolSize);
			}
		}
		else if (subSig == 25) {
			if (!isGet) {
				if (asPercentage) {
					factorA = percentRange(
						(float)std::get<double>(object.get()->obj), 0.001f, 10.0f, false);
				}
				else { factorA = clampf((float)std::get<double>(object.get()->obj), 0.001f, 10.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(factorA, settingsSig, subSig, "factorA")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(factorA, 0.001f, 10.0f, true)));
				}
				else { return std::make_shared<CObject>(double(factorA)); }
			}
		}
		else if (subSig == 26) {
			if (!isGet) {
				if (asPercentage) {
					factorB = percentRange(
						(float)std::get<double>(object.get()->obj), 0.001f, 10.0f, false);
				}
				else { factorB = clampf((float)std::get<double>(object.get()->obj), 0.001f, 10.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(factorB, settingsSig, subSig, "factorB")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(factorB, 0.001f, 10.0f, true)));
				}
				else { return std::make_shared<CObject>(double(factorB)); }
			}
		}
		else if (subSig == 27) {
			if (!isGet) {
				if (asPercentage) {
					factorC = percentRange(
						(float)std::get<double>(object.get()->obj), 0.001f, 10.0f, false);
				}
				else { factorC = clampf((float)std::get<double>(object.get()->obj), 0.001f, 10.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(factorC, settingsSig, subSig, "factorC")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(factorC, 0.001f, 10.0f, true)));
				}
				else { return std::make_shared<CObject>(double(factorC)); }
			}
		}
		else if (subSig == 28) {
			if (!isGet) { rotateNoise = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(rotateNoise, settingsSig, subSig, "rotateNoise")); }
				return std::make_shared<CObject>(rotateNoise);
			}
		}
		else if (subSig == 29) {
			if (!isGet) {
				if (asPercentage) {
					rotation = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -180.0f, 180.0f, false);
				}
				else { rotation = clampf((float)std::get<double>(object.get()->obj), -180.0f, 180.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(rotation, settingsSig, subSig, "rotation")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(rotation, -180.0f, 180.0f, true)));
				}
				else { return std::make_shared<CObject>(double(rotation)); }
			}
		}
		else if (subSig == 30) {
			if (!isGet) {
				if (asPercentage) {
					blackValue = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { blackValue = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(blackValue, settingsSig, subSig, "blackValue")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(blackValue, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(blackValue)); }
			}
		}
		else if (subSig == 31) {
			if (!isGet) {
				if (asPercentage) {
					whiteValue = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { whiteValue = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(whiteValue, settingsSig, subSig, "whiteValue")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(whiteValue, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(whiteValue)); }
			}
		}
		else if (subSig == 32) {
			if (!isGet) {
				if (asPercentage) {
					brightness = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -1.0f, 1.0f, false);
				}
				else { brightness = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(brightness, settingsSig, subSig, "brightness")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(brightness, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(brightness)); }
			}
		}
		else if (subSig == 33) {
			if (!isGet) {
				if (asPercentage) {
					contrast = percentRange_squaredCenter(
						(float)std::get<double>(object.get()->obj), -1.0f, 1.0f, false);
				}
				else { contrast = clampf((float)std::get<double>(object.get()->obj), -1.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(contrast, settingsSig, subSig, "contrast")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredCenter(contrast, -1.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(contrast)); }
			}
		}
		else if (subSig == 34) {
			if (!isGet) {
				if (asPercentage) {
					valueRangeMin = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { valueRangeMin = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(valueRangeMin, settingsSig, subSig, "valueRangeMin")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(valueRangeMin, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(valueRangeMin)); }
			}
		}
		else if (subSig == 35) {
			if (!isGet) {
				if (asPercentage) {
					valueRangeMax = percentRange(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { valueRangeMax = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(valueRangeMax, settingsSig, subSig, "valueRangeMax")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange(valueRangeMax, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(valueRangeMax)); }
			}
		}
		else if (subSig == 36) {
			if (!isGet) {
				if (asPercentage) {
					valueRangeSmooth = percentRange_squaredInvert(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { valueRangeSmooth = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(valueRangeSmooth, settingsSig, subSig, "valueRangeSmooth")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squaredInvert(valueRangeSmooth, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(valueRangeSmooth)); }
			}
		}
		else if (subSig == 37) {
			if (!isGet) { invertValue = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(invertValue, settingsSig, subSig, "invertValue")); }
				return std::make_shared<CObject>(invertValue);
			}
		}
		else if (subSig == 38) {
			if (!isGet) { flipX = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(flipX, settingsSig, subSig, "flipX")); }
				return std::make_shared<CObject>(flipX);
			}
		}
		else if (subSig == 39) {
			if (!isGet) { flipY = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(flipY, settingsSig, subSig, "flipY")); }
				return std::make_shared<CObject>(flipY);
			}
		}
		else if (subSig == 40) {
			if (!isGet) {
				if (asPercentage) {
					mixAmount_2D = percentRange_squared(
						(float)std::get<double>(object.get()->obj), 0.0f, 1.0f, false);
				}
				else { mixAmount_2D = clampf((float)std::get<double>(object.get()->obj), 0.0f, 1.0f); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(mixAmount_2D, settingsSig, subSig, "mixAmount_2D")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(mixAmount_2D, 0.0f, 1.0f, true)));
				}
				else { return std::make_shared<CObject>(double(mixAmount_2D)); }
			}
		}
		else if (subSig == 41) {
			if (!isGet) {
				if (asPercentage) {
					scaleX = percentRange_squared(
						(float)std::get<double>(object.get()->obj), 0.0f, 10.0f, false);
				}
				else { scaleX = clampf((float)std::get<double>(object.get()->obj), 0.0f, 10.0f); }
				glm::ivec2 canvasDimensions = chromaIO.get()->getCanvasDimensions();
				calculateFinalScale(&canvasDimensions);
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(scaleX, settingsSig, subSig, "scaleX")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(scaleX, 0.0f, 10.0f, true)));
				}
				else { return std::make_shared<CObject>(double(scaleX)); }
			}
		}
		else if (subSig == 42) {
			if (!isGet) {
				if (asPercentage) {
					scaleY = percentRange_squared(
						(float)std::get<double>(object.get()->obj), 0.0f, 10.0f, false);
				}
				else { scaleY = clampf((float)std::get<double>(object.get()->obj), 0.0f, 10.0f); }
				glm::ivec2 canvasDimensions = chromaIO.get()->getCanvasDimensions();
				calculateFinalScale(&canvasDimensions);
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(scaleY, settingsSig, subSig, "scaleY")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(scaleY, 0.0f, 10.0f, true)));
				}
				else { return std::make_shared<CObject>(double(scaleY)); }
			}
		}
		else if (subSig == 43) {
			if (!isGet) {
				if (asPercentage) {
					skew = percentRange_squared(
						(float)std::get<double>(object.get()->obj), 0.0f, 10.0f, false);
				}
				else { skew = clampf((float)std::get<double>(object.get()->obj), 0.0f, 10.0f); }
				glm::ivec2 canvasDimensions = chromaIO.get()->getCanvasDimensions();
				calculateFinalScale(&canvasDimensions);
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(skew, settingsSig, subSig, "skew")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(skew, 0.0f, 10.0f, true)));
				}
				else { return std::make_shared<CObject>(double(skew)); }
			}
		}
	}
	// Function Settings
	else if (subSig >= 50 && subSig < 60)
	{
		if (subSig == 50) {
			if (!isGet) { 
				fieldValue = std::get<std::string>(object.get()->obj); 
				if (interpreter != nullptr)
				{
					std::shared_ptr<CObject> funcObj = nullptr;
					if (fieldValue.find("::") != std::string::npos) {
						std::vector<std::string> scopeStack;
						std::string address = fieldValue;
						while (address.find("::") != std::string::npos)
						{
							scopeStack.push_back(splitRegularString(address, "::", true));
							address.erase(0, 1);
							std::cout << address << std::endl;
						}
						std::string lookup = address;
						funcObj = interpreter.get()->currentEnvironment.get()->get(scopeStack, lookup);
					}
					else {
						funcObj = interpreter.get()->currentEnvironment.get()->get(fieldValue);
					}
					if (funcObj != nullptr && funcObj.get()->objType.type == CLiteralTypes::_CFunction)
					{
						std::shared_ptr<CFunction> func = std::get<std::shared_ptr<CFunction>>(funcObj.get()->obj);
						if (func.get()->funcDeclaration.get()->returnType.get()->type == CTokenType::NUM)
						{
							std::vector<std::shared_ptr<CToken>> params = func.get()->funcDeclaration.get()->paramsTypes;
							if (params.size() == 8)
							{
								bool validParams = true;
								for (std::shared_ptr<CToken> param : params)
								{
									if (param.get()->type != CTokenType::NUM) { validParams = false; break; }
								}
								if (validParams)
								{
									function = func;
								}
							}
						}
					}
				}
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(fieldValue, settingsSig, subSig, "fieldValue")); }
				return std::make_shared<CObject>(fieldValue);
			}
		}
		else if (subSig == 51) {
			if (!isGet) { functionString = std::get<std::string>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(functionString, settingsSig, subSig, "functionString")); }
				return std::make_shared<CObject>(functionString);
			}
		}
		else if (subSig == 52) {
			if (!isGet) { useFunc = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(useFunc, settingsSig, subSig, "useFunc")); }
				return std::make_shared<CObject>(useFunc);
			}
		}
		else if (subSig == 53) {
			if (!isGet) { runFuncFirst = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(runFuncFirst, settingsSig, subSig, "runFuncFirst")); }
				return std::make_shared<CObject>(runFuncFirst);
			}
		}
	}
	else if (subSig >= 60 && subSig < 70)
	{
		if (subSig == 60) {
			if (!isGet) { /* CANNOT EDIT SETTING */ }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(noiseName, settingsSig, subSig, "noiseName")); }
				return std::make_shared<CObject>(noiseName);
			}
		}
	}
	else if (subSig >= 70 && subSig < 80)
	{
		if (subSig == 70) {
			if (!isGet) {
				if (asPercentage) {
					shardInterval = (int)percentRange_cubed(
						(float)std::get<double>(object.get()->obj), 1.0f, 250.0f, false);
				}
				else { shardInterval = clampi((int)std::get<double>(object.get()->obj), 1, 250); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(shardInterval, settingsSig, subSig, "shardInterval")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed((float)shardInterval, 1.0f, 250.0f, true)));
				}
				else { return std::make_shared<CObject>(double(shardInterval)); }
			}
		}
		else if (subSig == 71) {
			if (!isGet) {
				if (asPercentage) {
					anchorInterval = (int)percentRange_cubed(
						(float)std::get<double>(object.get()->obj), 1.0f, 250.0f, false);
				}
				else { anchorInterval = clampi((int)std::get<double>(object.get()->obj), 1, 250); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(anchorInterval, settingsSig, subSig, "anchorInterval")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed((float)anchorInterval, 1.0f, 250.0f, true)));
				}
				else { return std::make_shared<CObject>(double(anchorInterval)); }
			}
		}
		else if (subSig == 72) {
			if (!isGet) {
				if (asPercentage) {
					splineInterval = (int)percentRange_cubed(
						(float)std::get<double>(object.get()->obj), 1.0f, 250.0f, false);
				}
				else { splineInterval = clampi((int)std::get<double>(object.get()->obj), 1, 250); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(splineInterval, settingsSig, subSig, "splineInterval")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed((float)splineInterval, 1.0f, 250.0f, true)));
				}
				else { return std::make_shared<CObject>(double(splineInterval)); }
			}
		}
		else if (subSig == 73) {
			if (!isGet) {
				if (asPercentage) {
					entityInterval = (int)percentRange_cubed(
						(float)std::get<double>(object.get()->obj), 1.0f, 250.0f, false);
				}
				else { entityInterval = clampi((int)std::get<double>(object.get()->obj), 1, 250); }
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(entityInterval, settingsSig, subSig, "entityInterval")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_cubed((float)entityInterval, 1.0f, 250.0f, true)));
				}
				else { return std::make_shared<CObject>(double(entityInterval)); }
			}
		}
		else if (subSig == 74) {
			if (!isGet) { centerFinalNoise = std::get<bool>(object.get()->obj); }
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(centerFinalNoise, settingsSig, subSig, "centerFinalNoise")); }
				return std::make_shared<CObject>(centerFinalNoise);
			}
		}
	}
	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::shared_ptr<CObject>(nullptr);
}

void TSetNoise::initialize(float* trueSpacing, int* entityCount, glm::ivec2* canvasDimensions)
{
	if (isEnabled) {
		if (randomSeed) { generateRandSeed(); }
		if (useNoise_1D) {
			calculateBufferSize(*trueSpacing);
			fillBuffer();
			if (intervalMode == TSetProp::perEntity && 
				(entityInterval == 1 || *entityCount == 1 || *entityCount % entityInterval == 0)) 
			{
				finalValStatic = buffer.back();
			}
			else {
				finalValStatic = buffer.back();
				lastInvervalValue = buffer.back();
				nextIntervalValue = buffer.front();
			}
		}
		if (useNoise_2D) {
			calculateFinalScale(canvasDimensions);
		}
	}
}

bool TSetNoise::reportEnabled()
{
	if (isEnabled && (useNoise_1D || useNoise_2D)) { return true; }
	return false;
}

void TSetNoise::generateRandSeed()
{
	std::random_device device;
	std::default_random_engine eng(device());
	std::uniform_real_distribution<float> distr(0, 1);
	seed = lerpi(1, 99999, distr(eng));
}
void TSetNoise::calculateBufferSize(float trueSpacing)
{
	float toolFactor = 2.0f;
	if (chromaIO.get()->toolbox.get()->checkActiveTool()) {
		toolFactor = chromaIO.get()->toolbox.get()->getActiveTool().get()->getTrueSpacing();
	}
	float bufferResolution = 3.0f;
	bufferSize = (int)clampf(toolFactor * bufferResolution, 5.0f, 50.0f);
}
void TSetNoise::fillBuffer()
{
	glm::vec3 pos = glm::vec3(0.0f);
	glm::vec3 origin = glm::vec3(0.0f);
	glm::vec3 dir = glm::vec3(1.0f);
	for (int i = 0; i < bufferSize; i++)
	{
		if (i >= buffer.size()) {
			pos = glm::vec3(srandNormalized((unsigned int)(seed * hash1(float(seed * i * 1)))));
			origin = glm::vec3(srandNormalized((unsigned int)(seed * hash1(float(seed * i * 2)))));
			float noise1 = 1.0f;
			generate1DNoise(noise1, &pos, &origin, &dir, &i, false);
			buffer.push_back(noise1);
		}
		else {
			pos = glm::vec3(srandNormalized((unsigned int)(seed * hash1(float(seed * i * 1)))));
			origin = glm::vec3(srandNormalized((unsigned int)(seed * hash1(float(seed * i * 2)))));
			float noise1 = 1.0f;
			generate1DNoise(noise1, &pos, &origin, &dir, &i, false);
			buffer[i] = noise1;
		}
	}
	if (buffer.size() > bufferSize) {
		buffer.erase(buffer.begin() + bufferSize, buffer.end());
		buffer.shrink_to_fit();
	}
}

void TSetNoise::calculateFinalScale(glm::ivec2* canvasDimensions)
{
	canvasCenter = *canvasDimensions;
	canvasCenter /= 2.0f;
	glm::vec2 canvasVec = canvasCenter / 50.0f;
	float canvasFactor = (canvasVec.x + canvasVec.y) / 2.0f;
	canvasFactor = (canvasFactor == 0.0f) ? 1.0f : canvasFactor;
	float toolFactor = 1.0f;
	if (chromaIO.get()->toolbox.get()->checkActiveTool()) {
		toolFactor = chromaIO.get()->toolbox.get()->getActiveTool().get()->getSize() / 50.0f;
	}
	toolFactor = (toolFactor == 0.0f) ? 1.0f : toolFactor;
	finalScale = 1.0f * 
		//(int(!scaleToCanvas) * 5.0f + (1 - int(!scaleToCanvas))) *
		(int(scaleToCanvas) * canvasFactor + (1 - int(scaleToCanvas))) *
		(int(scaleToToolSize) * toolFactor + (1 - int(scaleToToolSize)));
}
glm::vec2 TSetNoise::getFinalScale() { return glm::vec2(scaleX, scaleY) * finalScale; }

void TSetNoise::calculateNoise(float& outValue, glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, 
	Input* input, int* shardCount, int* anchorCount, int* splineCount, bool shuffleSeed)
{
	// Incoming outValue must be between 0 to 1 range
	float noise1 = 1.0f; float noise2 = 1.0f; float noiseFunc = 1.0f;
	if (!useNoise_1D && !useNoise_2D && !useFunc) { return; }
	// Run Function Noise if to run function first
	if (useFunc && runFuncFirst)
	{
		calculateFuncNoise(outValue, pos, input);
	}
	// Calculate noise values
	if (useNoise_1D)
	{
		noise1 = calculate1DNoise(pos, origin, dir, shardCount, anchorCount, splineCount, shuffleSeed);
		if (smoothing != 0.0f) {
			noise1 = smooth1DNoise(noise1); 
		}
	}
	if (useNoise_2D)
	{
		noise2 = calculate2DNoise(pos, origin, dir);
	}
	//if (useNoise_Tex) // Texture Noise?
	

	float outNoise = 1.0f;
	if (useNoise_1D && useNoise_2D)
	{
		float target = 1.0f;
		float blend = 1.0f;
		if (apply1DFirst) {
			target = (1.0f - (noise1 * mixAmount_1D));
			blend = (1.0f - (noise2 * mixAmount_2D));
		}
		else {
			target = (1.0f - (noise2 * mixAmount_2D));
			blend = (1.0f - (noise1 * mixAmount_1D));
		}
		outNoise = blend1(target, blend, noiseMixMode);
	}
	else
	{
		outNoise = (useNoise_1D) ? noise1 : (useNoise_2D) ? noise2 : outNoise;
	}
	// Function Noise calculations at end if not ran as first
	if (useFunc && !runFuncFirst)
	{
		calculateFuncNoise(outNoise, pos, input);
	}
	// Run final blend 
	outNoise = blend1(outValue, outNoise, totalMixMode);
	// Clamp noise to range
	if (clampNoiseToRange) {
		if (centerValRange) {
			outValue = lerpf(0.5f - ((noiseMin / finalValMin) * 0.5f), 0.5f + ((noiseMax / finalValMax) * 0.5f), outNoise);
		}
		else {
			outValue = lerpf(noiseMin, noiseMax, outNoise);
		}
	}
	else {
		outValue = clampf(outNoise, 0.0f, 1.0f);
	}
	// Apply center inversion
	if (centerFinalNoise)
	{
		outValue = (outValue > 0.5f) ? 1.0f - (outValue - 0.5f): 0.0f + (0.5f - outValue);
	}
}

float TSetNoise::calculate1DNoise(glm::vec3* pos, glm::vec3* origin, glm::vec3* dir,
	int* shardCount, int* anchorCount, int* splineCount, bool shuffleSeed)
{
	float outNoise = 1.0f;
	int* lastCount = &lastShardCount;
	int currentCount = 0;
	int interval = 0;
	switch (intervalMode)
	{
		case TSetProp::perShard: if (shardCount == nullptr) { return 1.0f; } lastCount = &lastShardCount; currentCount = *shardCount; interval = shardInterval; break;
		case TSetProp::perAnchor: if (anchorCount == nullptr) { return 1.0f; } lastCount = &lastAnchorCount; currentCount = *anchorCount; interval = anchorInterval; break;
		case TSetProp::perSpline: if (splineCount == nullptr) { return 1.0f; } lastCount = &lastSplineCount; currentCount = *splineCount; interval = splineInterval; break;
	}
	if (intervalMode != TSetProp::perEntity) {
		if (*lastCount != currentCount &&
				(currentCount == 0 || currentCount % interval == 0)) {
			lastInvervalValue = nextIntervalValue;
			float nextNoise = 1.0f;
			glm::vec2 posNext2 = glm::vec2(pos->x, pos->y);
			posNext2 = hash2(posNext2) * posNext2;
			glm::vec3 posNext3 = glm::vec3(posNext2.x, posNext2.y, 0.0f);
			nextIntervalValue = generate1DNoise(nextNoise, &posNext3, origin, dir, shardCount, shuffleSeed);
			*lastCount = currentCount;
		}
		if (intervalSmoothing) {
			outNoise = lerpf(lastInvervalValue, nextIntervalValue, (float(currentCount % interval) / (float)interval));
		}
		else {
			outNoise = lastInvervalValue;
		}
	}
	else if (intervalMode == TSetProp::perEntity)
	{
		outNoise = finalValStatic;
	}
	outNoise = clampf(outNoise, 0.0f, 1.0f);
	return outNoise;
}

float TSetNoise::generate1DNoise(float& outNoise, glm::vec3* pos, glm::vec3* origin, glm::vec3* dir, int* shuffle, bool shuffleSeed)
{
	glm::vec2 hashed = hash2(*pos + glm::vec3(float(shuffleSeed) * hash1(-*shuffle), float(shuffleSeed) * hash1(*shuffle), 0.0f));
	outNoise = srandNormalized(int((float)(seed + int(*shuffle * shuffleSeed)) * hashed.y));
	if (intensity > 0.00f)
	{
		float sel = (srandNormalized(int((float)(seed + int(shuffleSeed * hash1(*shuffle * 2.0f))) * hashed.x)) < intensity) ? 1.0f : 0.0f;
		outNoise = mix(outNoise, weight, intensity * sel);
	}
	return outNoise;
}

float TSetNoise::smooth1DNoise(float newNoise)
{
	float smoothed = newNoise;
	buffer.erase(buffer.begin());
	buffer.push_back(newNoise);
	float avg = sumVector_float(&buffer) / (float)bufferSize;
	smoothed = mix(newNoise, brightContrast(avg, 0.0f, 1.2f), smoothing);
	return smoothed;
}

glm::vec2 TSetNoise::adjustPos(glm::vec3* pos, glm::vec3* origin, glm::vec3* dir)
{
	glm::vec2 pivot = (!useOriginOffset) ? glm::vec2(origin->x, (canvasCenter.y * 2.0f) - (-1 * origin->y)) : canvasCenter;
	
	glm::vec2 loc = *pos;
	loc.y = (canvasCenter.y * 2.0f) - (-1 * loc.y);
	loc += glm::vec2((float)seed * hash1(-(float)seed), (float)seed * hash1((float)seed));
	// Flip the location if needed
	loc = glm::vec2((flipX == 1) ? 1.0 - loc.x : loc.x, (flipY == 1) ? 1.0 - loc.y : loc.y);

	// Apply factor B (Stretching)
	float radians = rotation * (MATH_PI / 180.0f);
	loc = mix(loc,
		glm::vec2(
			glm::dot(loc - pivot, glm::vec2(sin(radians), cos(radians))),
			glm::dot(loc - pivot, glm::vec2(cos(radians), -sin(radians)))) + pivot,
		fract(skew / 20.0f));

	glm::vec2 rotP = loc - pivot;

	float uR = glm::dot(rotP, glm::vec2(cos(radians), -sin(radians)));
	float vR = glm::dot(rotP, glm::vec2(sin(radians), cos(radians)));

	loc = mix(loc, glm::vec2(uR, vR) + pivot, rotateNoise);
	loc = ((loc - pivot) / getFinalScale()) + pivot;
	return loc;
}

float TSetNoise::calculate2DNoise(glm::vec3* pos, glm::vec3* origin, glm::vec3* dir)
{
	glm::vec2 loc = adjustPos(pos, origin, dir);
	
	float outNoise = 1.0f;
	switch (noiseMode)
	{
	case TSetProp::perlin: outNoise = perlin2D(loc, factorA, factorB, factorC); break;
	case TSetProp::voroni: outNoise = voroni2D(loc, factorA, factorB, factorC); break;
	case TSetProp::random: outNoise = random2D(loc, factorA, factorB, factorC); break;
	case TSetProp::vorotwist: outNoise = vorotwist2D(loc, factorA, factorB, factorC); break;
	case TSetProp::voroblur: outNoise = voroblur2D(loc, factorA, factorB, factorC); break;
	case TSetProp::canvas: outNoise = canvas_typeA_2D(loc, getFinalScale().x * getFinalScale().y, factorA, factorB, factorC); break;
	case TSetProp::cellularA: outNoise = cellularA_2D(loc, factorA, factorB, factorC); break;
	case TSetProp::cellularB: outNoise = cellularB_2D(loc, factorA, factorB, factorC).x; break;
	case TSetProp::halftoneDots: outNoise = halftoneDot2D(loc, factorA, factorB, factorC); break;
	case TSetProp::maze: outNoise = truchetPattern(loc, 0, factorA, factorB, factorC); break;
	case TSetProp::round: outNoise = truchetPattern(loc, 2, factorA, factorB, factorC); break;
	case TSetProp::triangles: outNoise = truchetPattern(loc, 1, factorA, factorB, factorC); break;
	default:
		outNoise = perlin2D(loc, factorA, factorB, factorC); break;
	}

	// Apply Image Adjustments
	outNoise = mix(outNoise, smoothstep(valueRangeMin, valueRangeMax, outNoise), 1.0f - valueRangeSmooth);
	outNoise = clampf(outNoise, valueRangeMin, valueRangeMax);
	outNoise = brightContrast(outNoise, brightness, contrast);
	outNoise = (outNoise < blackValue) ? 0.0f : (outNoise > whiteValue) ? 1.0f : outNoise;
	outNoise = mix(outNoise, 1.0f - outNoise, invertValue);
	// Return final clamped value
	return clampf(outNoise, 0.0f, 1.0f);
}

float TSetNoise::calculateFuncNoise(float& inValue, glm::vec3* pos, Input* input)
{
	if (function == nullptr) { return 1.0f; }
	std::vector<std::shared_ptr<CObject>> args;

	args.push_back(std::make_shared<CObject>(double(inValue)));
	args.push_back(std::make_shared<CObject>(double(pos->x)));
	args.push_back(std::make_shared<CObject>(double(-pos->y)));
	args.push_back(std::make_shared<CObject>(double(input->pressure)));
	args.push_back(std::make_shared<CObject>(double(input->rotation)));
	args.push_back(std::make_shared<CObject>(double(input->tiltX)));
	args.push_back(std::make_shared<CObject>(double(input->tiltY)));
	args.push_back(std::make_shared<CObject>(double(input->velocity)));
	std::shared_ptr<CObject> returnObj = function.get()->call(chromaIO.get()->scriptConsole.get()->getInterpreter(), &args);
	chromaIO.get()->scriptConsole.get()->getInterpreter()->resetInterpreter();
	if (returnObj != nullptr && returnObj.get()->objType.type == CLiteralTypes::_CNumber)
	{
		inValue = (float)std::get<double>(returnObj.get()->obj);
	}
	return 1.0f;
}