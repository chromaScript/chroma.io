#include "../../include/methods/inputMethods/In_Pan.h"
#include "../../include/methods/InputMethod.h"
#include "../../include/tool/ToolSettings.h"
#include "../../include/tool/toolSettings/ToolSettings_Forward.h"
#include "../../include/tool/Tool.h"

#ifndef APPLICATION_H
#include "../../include/Application.h"
#endif

#include <iostream>

InputHandlerFlag In_Pan::move(Application* sender, Input dat)
{
	// If the array is empty, insert the first element, otherwise overwrite it with the passed Input
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }
	// Test that the cursor moved a significant amount
	glm::vec3 pos = glm::vec3(dat.x, dat.y, 0.0f);
	glm::vec3 dir = glm::normalize(pos - fragData.anchors.back().pos);
	float length = glm::length(pos - fragData.anchors.back().pos);
	if (length < ROOT2 * 1.2f) { return InputHandlerFlag::wait; }

	bool isNew = (fragData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
	// Adjust the anchor points
	fragData.anchors.front().input.flagPrimary = InputFlag::null;
	// Add the anchor
	int i = (int)fragData.anchors.size() - 1;
	while (i >= 0 && (std::isnan(dir.x) || std::isnan(dir.y) || std::isnan(dir.z))) {
		dir = fragData.anchors[(size_t)i].dir;
		i--;
	}
	float angle = glm::degrees(std::atan2f(dir.y, dir.x));

	if (!lockModeActivated && length > ROOT2 * 1.5f)
	{
		std::vector<float> constraintAngles;
		std::vector<float> constraintDifferences;
		std::vector<Keybind> constraintKeys;
		bool doConstraintA = pan.lockToX && compareModKey(pan.lockXModKey, dat.modKey, false);
		if (doConstraintA)
		{
			constraintAngles.push_back(clampAngle_360(360.0f));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360.0f))));
			constraintAngles.push_back(clampAngle_360(180.0f));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360.0f))));
			constraintKeys.push_back(pan.lockXModKey);
			constraintKeys.push_back(pan.lockXModKey);
		}
		bool doConstraintB = pan.lockToY && compareModKey(pan.lockYModKey, dat.modKey, false);
		if (doConstraintB)
		{
			constraintAngles.push_back(clampAngle_360(90.0f));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360.0f))));
			constraintAngles.push_back(clampAngle_360(270.0f));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360.0f))));
			constraintKeys.push_back(pan.lockYModKey);
			constraintKeys.push_back(pan.lockYModKey);
		}
		bool doConstraintC = pan.lockCustomAngle && compareModKey(pan.lockCustomModKey, dat.modKey, false);
		if (doConstraintC)
		{
			constraintAngles.push_back(clampAngle_360(pan.customAngle * -1.0f));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360.0f))));
			constraintAngles.push_back(clampAngle_360((pan.customAngle * -1.0f) + 180.0f));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360.0f))));
			constraintKeys.push_back(pan.lockCustomModKey);
			constraintKeys.push_back(pan.lockCustomModKey);
		}
		if (constraintAngles.size() != 0)
		{
			// Use an iterator max_element operation to get the index (distance) to get the desired constraint angle
			std::vector<float>::iterator lowIndex = std::min_element(constraintDifferences.begin(), constraintDifferences.end());
			float lowValue = constraintDifferences[std::distance(constraintDifferences.begin(), lowIndex)];
			std::vector<float>::iterator highIndex = std::max_element(constraintDifferences.begin(), constraintDifferences.end());
			float highValue = 360 - constraintDifferences[std::distance(constraintDifferences.begin(), highIndex)];
			float selectAngle = constraintAngles[std::distance(
				constraintDifferences.begin(), (lowValue <= highValue) ? lowIndex : highIndex)];
			activeKey = constraintKeys[std::distance(
				constraintDifferences.begin(), (lowValue <= highValue) ? lowIndex : highIndex)];
			activeAngle = clampAngle_180(selectAngle);
			constraintOrigin = pos;
			constraintDir =
				//glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(continuousControl->activeAngle + 90))) * DEFAULT_DIR;
				glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(activeAngle + 0.0f))) * DEFAULT_DIR;
			constraintDirPerpendicular = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(90.0f))) * constraintDir;
			glm::vec3 constrainP2 = constraintOrigin + (constraintDir * 100.0f);
			constraintLine = glm::vec4(constraintOrigin.x, constraintOrigin.y, constrainP2.x, constrainP2.y);
			lockModeActivated = true;
			//std::cout << activeAngle << std::endl;
		}
	}

	if (lockModeActivated)
	{
		glm::vec3 perpLineP2 = pos + (constraintDirPerpendicular * 100.0f);
		glm::vec4 perpLine = glm::vec4(pos.x, pos.y, perpLineP2.x, perpLineP2.y);
		pos = glm::vec3(lineIntersect2D(constraintLine, perpLine), 0.0f);
		dir = glm::vec3(
			cos(activeAngle * (MATH_PI / 180.0f)),
			sin(activeAngle * (MATH_PI / 180.0f)),
			0.0f);
		if (!compareModKey(activeKey, dat.modKey, false))
		{
			clearConstraint();
		}
	}

	if (fragData.anchors.size() >= 3) { fragData.anchors.erase(fragData.anchors.begin()); }
	fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pos, dir,
		1.0f,
		HandleType::linear, false, pos,
		HandleType::linear, false, pos,
		HandleRel::independent,
		data.inputEvents.back()));
	if (pan.clickTargetPanning && compareModKey(pan.clickTarModKey, dat.modKey, false)) {
		fragData.anchors.back().input.flagSecondary = InputFlag::connect;
	}

	if (isNew)
	{
		fragData.anchors.front().input.flagPrimary = InputFlag::null;
	}


	return InputHandlerFlag::allowPress_updateCursor;
}

void In_Pan::clearConstraint()
{
	lockModeActivated = false;
	activeAngle = -720.0f;
	constraintOrigin = glm::vec3(0.0f);
	constraintDir = DEFAULT_DIR;
	constraintDirPerpendicular = DEFAULT_DIR;
	constraintLine = glm::vec4(0.0f);
	activeKey = Keybind();
}

InputHandlerFlag In_Pan::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	// Warning: This logic branch has no fail-case
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
	{
		// Get the settings
		pan = *owner.get()->getPan();
		lockModeActivated = false;
		activeKey = Keybind();
		// For new inputs, clear the current array, and then set start to the passed Input
		data.reset();
		data.start = dat;
		data.inputModKey = dat.modKey;
		// Push back a new data point
		data.inputEvents.push_back(dat);
		// Reset the fragData
		fragData.reset();
		fragData.startTime = (float)data.start.time;
		// Set the upper left corner for the entity 
		glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
		glm::vec3 pos = glm::vec3(dat.x, dat.y, 0.0f);
		fragData.transform.origin = origin;
		// Add the first anchor point
		fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pos, DEFAULT_DIR,
			1.0f,
			HandleType::linear, false, pos,
			HandleType::linear, false, pos,
			HandleRel::independent,
			data.inputEvents.back()));
		fragData.anchors.front().input.flagPrimary = InputFlag::newInput;
		fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pos, DEFAULT_DIR,
			1.0f,
			HandleType::linear, false, pos,
			HandleType::linear, false, pos,
			HandleRel::independent,
			data.inputEvents.back()));
		fragData.anchors.back().input.flagPrimary = InputFlag::null;
		fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pos, DEFAULT_DIR,
			1.0f,
			HandleType::linear, false, pos,
			HandleType::linear, false, pos,
			HandleRel::independent,
			data.inputEvents.back()));
		fragData.anchors.back().input.flagPrimary = InputFlag::null;
		wasHandled = InputHandlerFlag::allowPress_updateCursor;
		//std::cout << "MethodType::in_pan::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		// For Drag, Only interested in the start, last inputEvent, & end events
		data.end = dat;
		fragData.endTime = (float)data.end.time;
		if (pan.clickTargetPanning && compareModKey(pan.clickTarModKey, dat.modKey, false)) {
			fragData.anchors.back().input.flagSecondary = InputFlag::connect;
		}
		wasHandled = InputHandlerFlag::release;
		std::cout << "MethodType::in_pan::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
		std::cout << "MethodType::in_pan::CLICK::NOSIGNAL" << std::endl;
	}
	std::cout << "MethodType::in_pan::CLICK::TIME=" << dat.time << "::TYPE=" << keybindToString(Keybind(InputKey::unknown, data.inputModKey)) << std::endl;
	return wasHandled;
}

InputHandlerFlag In_Pan::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	return InputHandlerFlag::noSignal;
}