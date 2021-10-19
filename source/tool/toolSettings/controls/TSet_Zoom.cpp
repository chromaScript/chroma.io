#include "../../../include/tool/ToolSettings.h"
#include "../../../include/input/keys.h"
#include "../../../include/math/Color.h"

class CFunction;
#include "../../../include/cscript/CInterpreter.h"
#include "../../../include/cscript/CObject.h"
#include "../../../include/tool/Tool.h"
#include "../../../include/Application.h"

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>
#include <string>
#include <vector>
#include <memory>
#include <random>

#include "../../../include/tool/toolSettings/controls/TSet_Zoom.h"

extern std::shared_ptr<Application> chromaIO;

////////////////////////////////////////////////////////////////
// Zoom
////////////////////////////////////////////////////////////////

TSet_Zoom::TSet_Zoom()
{
	this->type = TSetType::zoom;
	this->isEnabled = true;
	this->scrubThreshold = 16; // 200
	this->accelerationIn = 0.8f; // 201
	this->accelerationOut = 0.25f; // 202
	this->zoomSpeed = 0.45f; // 203
	this->clickTarZoom = false; // 204
	this->animatedZoom = false; // 205
	this->animSpeed = 0.5f; // 206
	this->zoomStepField = "global::zoomStepsA"; // 210
	this->zoomSteps = { 1.0f, 2.5f, 5.0f, 10.0f, 15.0f, 20.0f, 30.0f, 40.0f, 50.0f, 75.0f, 100.0f, 125.0f, 150.0f, 175.0f, 200.0f, 250.0f, 300.0f };
	this->putProperty(
		chromaIO.get()->scriptConsole.get()->getInterpreter(), nullptr, 
		210, -1, std::make_shared<CObject>(zoomStepField), 
		false, false, false);
}
std::shared_ptr<CObject> TSet_Zoom::putProperty(
	std::shared_ptr<CInterpreter> interpreter, std::shared_ptr<Tool> owner,
	int settingsSig, int subSig, std::shared_ptr<CObject> object, bool isGet, bool asPercentage, bool asString)
{
	if (settingsSig >= 200 && settingsSig < 210)
	{
		if (settingsSig == 200)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(scrubThreshold, settingsSig, subSig, "scrubThreshold")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared((float)scrubThreshold, 4, 64, true)));
				}
				else { return std::make_shared<CObject>(double(scrubThreshold)); }
			}
			else
			{
				if (asPercentage) {
					scrubThreshold = (int)std::round(
						percentRange_squared((float)std::get<double>(object.get()->obj), 4, 64, false));
				}
				else {
					scrubThreshold = clampi(
						(int)std::round(std::get<double>(object.get()->obj)), 4, 64);
				}
			}
		}
		else if (settingsSig == 201)
		{
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(accelerationIn, settingsSig, subSig, "accelerationIn")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(accelerationIn, 0.1f, 2.0f, true)));
				}
				else { return std::make_shared<CObject>(double(accelerationIn)); }
			}
			else {
				if (asPercentage) {
					accelerationIn = percentRange_squared((float)std::get<double>(object.get()->obj), 0.1f, 2.0f, false);
				}
				else { accelerationIn = clampf((float)std::get<double>(object.get()->obj), 0.1f, 2.0f); }
			}
		}
		else if (settingsSig == 202)
		{
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(accelerationOut, settingsSig, subSig, "accelerationOut")); }
				if (asPercentage) {
					return std::make_shared<CObject>(double(
						percentRange_squared(accelerationOut, 0.1f, 2.0f, true)));
				}
				else { return std::make_shared<CObject>(double(accelerationOut)); }

			}
			else {
				if (asPercentage) {
					accelerationOut = percentRange_squared((float)std::get<double>(object.get()->obj), 0.1f, 2.0f, false);
				}
				else { accelerationOut = clampf((float)std::get<double>(object.get()->obj), 0.01, 2.0f); }
			}
		}
		else if (settingsSig == 203)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(zoomSpeed, settingsSig, subSig, "zoomSpeed")); }
				if (asPercentage) { return std::make_shared<CObject>(double(percentRange_cubed(zoomSpeed, 0.01f, 1.0f, true))); }
				else { return std::make_shared<CObject>(double(zoomSpeed)); }
			}
			else
			{
				if (asPercentage) { zoomSpeed = percentRange_cubed((float)std::get<double>(object.get()->obj), 0.01f, 1.0f, false); }
				else { zoomSpeed = clampf((float)std::get<double>(object.get()->obj), 0.01f, 1.0f); }
			}
		}
		else if (settingsSig == 204)
		{
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(clickTarZoom, settingsSig, subSig, "clickTarZoom")); }
				return std::make_shared<CObject>(clickTarZoom);
			}
			else {
				clickTarZoom = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 205)
		{
			if (isGet) {
				if (asString) { return std::make_shared<CObject>(makeSettingString(animatedZoom, settingsSig, subSig, "animatedZoom")); }
				return std::make_shared<CObject>(animatedZoom);
			}
			else {
				animatedZoom = std::get<bool>(object.get()->obj);
			}
		}
		else if (settingsSig == 206)
		{
			if (isGet)
			{
				if (asString) { return std::make_shared<CObject>(makeSettingString(animSpeed, settingsSig, subSig, "animSpeed")); }
				if (asPercentage) { return std::make_shared<CObject>(double(percentRange_cubed(animSpeed, 0.01f, 1.0f, true))); }
				else { return std::make_shared<CObject>(double(animSpeed)); }
			}
			else
			{
				if (asPercentage) { animSpeed = percentRange_cubed((float)std::get<double>(object.get()->obj), 0.01f, 1.0f, false); }
				else { animSpeed = clampf((float)std::get<double>(object.get()->obj), 0.01f, 1.0f); }
			}
		}
	}
	else if (settingsSig >= 210 && settingsSig < 220)
	{
		if (settingsSig == 210)
		{
			if (!isGet) {
				std::string zoomStepStr = std::get<std::string>(object.get()->obj);
				zoomStepField = zoomStepStr;
				if (interpreter != nullptr)
				{
					std::shared_ptr<CObject> numArrayObj = nullptr;
					if (zoomStepStr.find("::") != std::string::npos) {
						std::vector<std::string> scopeStack;
						std::string address = zoomStepStr;
						while (address.find("::") != std::string::npos)
						{
							scopeStack.push_back(splitRegularString(address, "::", true));
							address.erase(0, 1);
							std::cout << address << std::endl;
						}
						std::string lookup = address;
						std::shared_ptr<CEnvironment> previous = interpreter.get()->getEnvironment();
						interpreter.get()->setEnvironment(interpreter.get()->getEnvironment("global"));
						numArrayObj = interpreter.get()->currentEnvironment.get()->get(scopeStack, lookup);
						interpreter.get()->setEnvironment(previous);
					}
					else {
						std::shared_ptr<CEnvironment> previous = interpreter.get()->getEnvironment();
						interpreter.get()->setEnvironment(interpreter.get()->getEnvironment("global"));
						numArrayObj = interpreter.get()->currentEnvironment.get()->get(zoomStepStr);
						interpreter.get()->setEnvironment(previous);
					}
					// Get the array
					if (numArrayObj != nullptr && numArrayObj.get()->objType.type == CLiteralTypes::_CNumber_Array)
					{
						std::vector<std::shared_ptr<CObject>> numArr =
							*std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(numArrayObj.get()->obj);
						std::vector<float> newStepArray;
						for (std::shared_ptr<CObject> obj : numArr)
						{
							if (obj.get()->objType.type == CLiteralTypes::_CNumber) {
								newStepArray.push_back((float)std::get<double>(obj.get()->obj));
							}
						}
						if (newStepArray.size() != 0) {
							zoomSteps = newStepArray;
						}
					}
				}
			}
			else {
				if (asString) { return std::make_shared<CObject>(makeSettingString(zoomStepField, settingsSig, subSig, "zoomStepField")); }
				return std::make_shared<CObject>(zoomStepField);
			}
		}
	}

	if (asString) { return std::make_shared<CObject>(makeSettingString(isEnabled, settingsSig, subSig, "NULL")); }
	return std::make_shared<CObject>(nullptr);
}

TSetControl_Node* TSet_Zoom::getControlNode(int settingSig, int subSig)
{
	return nullptr;
}
TSetController* TSet_Zoom::getController(int settingSig, int subSig)
{
	return nullptr;
}
TSetGraph* TSet_Zoom::getGraph(int settingSig, int subSig)
{
	return nullptr;
}
TSetNoise* TSet_Zoom::getNoise(int settingSig, int subSig)
{
	return nullptr;
}