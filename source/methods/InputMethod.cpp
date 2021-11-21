#include "../include/methods/InputMethod.h"
#include "../include/tool/ToolSettings.h"
#include "../include/tool/toolSettings/ToolSettings_Forward.h"
#include "../include/tool/Tool.h"
#include "../include/math/math.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/rotate_vector.hpp>

#include <iostream>
#include <algorithm>
#include <iostream>

InputHandlerFlag InputMethod::continuousMove(Application* sender, Input dat, glm::vec3* pos, glm::vec3* dir)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (activeMode == TSetProp::line) {

		bool result = continuousHover(sender, dat, &continuous, &smoothing, &splineData, continuous.trueSpacing, *pos, *dir);
		glm::vec3 dirSpline = *dir;
		if (!result) {
			dirSpline = makeDir(splineData.anchors[splineData.anchors.size() - 2].pos , *pos);
			splineData.anchors[splineData.anchors.size() - 2].dir = dirSpline;
		}
		else {
			dirSpline = makeDir(splineData.anchors[splineData.anchors.size() - 2].pos, *pos);
		}
		splineData.anchors.back() = FragmentAnchor(anchorIDCount, *pos, dirSpline, 1.0f, dat);
		splineData.anchors.back().input.pressure = 1.0f;
		if (continuous.subdivide) {
			glm::vec3 pos1 = (splineData.anchors.size() >= 2) ? splineData.anchors[splineData.anchors.size() - 2].pos : splineData.transform.origin;
			glm::vec3 pos2 = splineData.anchors.back().pos;
			splineData.subdivCount = int(floor((glm::length(pos2 - pos1) / continuous.trueSpacing) * continuous.subdivideAmount));
		}
		else {
			splineData.subdivCount = 0;
		}
		wasHandled = InputHandlerFlag::previewLine;
	}
	// Continuous :: Lines Mode :: Curve
	else if (activeMode == TSetProp::curves &&
		dat.action == InputAction::press &&
		compareModKeyComponent(dat.modKey, continuous.alternateSubModeKey.modKey, false))
	{
		addInputData(dat);
		size_t size = splineData.anchors.size();
		// Update the start/end curve vertices metadata
		splineData.anchors[size - 2].headControl = splineData.anchors[size - 1].tailControl = true;
		splineData.anchors[size - 2].headType = splineData.anchors[size - 1].tailType = HandleType::manual;
		// Move the handle points for the adjacent points
		glm::vec3 lineDir = makeDir(splineData.anchors[size - 2].pos, splineData.anchors[size - 1].pos);
		float lineLen = glm::length(splineData.anchors[size - 1].pos - splineData.anchors[size - 2].pos) / 2.0f;
		float pressure = clampf(pow((sender->stylusInRange()) ? averageInputs(&data, 12, 0.15f, 0.6f, true).pressure : 0.8f, 3.2f) / 0.85f, 0.0f, 1.0f);
		lineLen = lerpf(0.0f, lineLen, pressure);
		// Update the positions/directions
		glm::vec3 leftPos = *pos - (lineDir * lineLen);
		glm::vec3 leftDir = makeDir(splineData.anchors[size - 2].pos, leftPos);
		glm::vec3 rightPos = *pos + (lineDir * lineLen);
		glm::vec3 rightDir = makeDir(rightPos, splineData.anchors[size - 1].pos);
		splineData.anchors[size - 2].headHandle = leftPos;
		splineData.anchors[size - 2].dir = leftDir;
		splineData.anchors[size - 1].tailHandle = rightPos;
		splineData.anchors[size - 1].dir = rightDir;
		wasHandled = InputHandlerFlag::previewCurves;
	}
	else if (activeMode == TSetProp::editHandles &&
		compareModKeyComponent(dat.modKey, continuous.alternateSubModeKey.modKey, false))
	{
		if (activePoint != nullptr) {
			*activePoint = *pos;
			if (activePointLinked != nullptr) {
				*activePointLinked = *pos + activeOrigin;
			}
		}
		wasHandled = InputHandlerFlag::editMode;
	}
	// Continuous :: Draw Mode
	else {
		if (!continuousDraw(sender, dat, &continuous, &smoothing, &splineData, continuous.trueSpacing, *pos, *dir)) {
			return InputHandlerFlag::wait;
		}
		splineIDCount++;
		splineData.anchors.push_back(FragmentAnchor(splineIDCount, *pos, *dir, 1.0f, dat));
		this->addVertices(pos, dir, &dat);
		wasHandled = InputHandlerFlag::allowPress;
	}
	return wasHandled;
}

InputHandlerFlag InputMethod::continuousClick(Application* sender, Input dat, 
	int waitCountVertex, int waitCountSpline, InputFlag vertexFlagSecondary, InputFlag splineFlagSecondary)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
	{
		// Clear any constraint variables, these should not remain constant between different
		// press events of the same stroke
		continuous.clearConstraint();
		fragData.activeModKey = dat.modKey;
		splineData.activeModKey = dat.modKey;

		if (activeMode == TSetProp::editHandles) {
			bool result = continuousEditHandles(wasHandled, sender, dat);
		}
		else if (activeMode == TSetProp::line && !forceNewInput)
		{
			if (compareModKeyComponent(dat.modKey, continuous.alternateSubModeKey.modKey, false))
			{
				activeMode = TSetProp::curves;
				wasHandled = InputHandlerFlag::allowPress;
			}
			else {
				glm::vec3 pos = splineData.anchors.back().pos;
				glm::vec3 dir = makeDir(splineData.anchors.at(splineData.anchors.size() - 2).pos, pos);
				bool altMode = compareModKey(dat.modKey, continuous.alternateModeKey.modKey, false);
				if (!altMode) { activeMode = continuous.defaultMode; }
				else if (altMode && continuous.defaultMode == TSetProp::line) { activeMode = TSetProp::draw; }
				else if (altMode && continuous.defaultMode == TSetProp::draw) { activeMode = TSetProp::draw; }
				if (activeMode == TSetProp::draw) {
					this->generateVertices(&pos, &dir, &dat);
					wasHandled = InputHandlerFlag::allowPress;
				}
				else {
					this->generateVertices(&pos, &dir, &dat);
					splineIDCount++;
					splineData.anchors.push_back(FragmentAnchor(splineIDCount, pos, dir, 1.0f, dat));
					wasHandled = InputHandlerFlag::allowPress;
				}
			}
		}
		// Connection of previous input to next input
		else if (fragData.anchors.size() != 0 && compareModKey(dat.modKey, continuous.connectLastStrokeKey.modKey, false))
		{
			wasHandled = connectInput_continuous(sender, &dat);
		}
		// A new set of anchors is started
		else
		{
			newInput(sender, dat);
			wasHandled = newInput_continuous(sender, dat, waitCountVertex, waitCountSpline, vertexFlagSecondary, splineFlagSecondary);
		}
	}
	// Handle Click Release
	else if (dat.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		data.end = dat;
		if (fragData.anchors.size() == 1) { fragData.anchors.back().input.flagPrimary = InputFlag::null; }
		fragData.activeModKey = splineData.activeModKey = dat.modKey;
		fragData.endTime = splineData.endTime = (float)data.end.time;

		if (activeMode == TSetProp::curves || activeMode == TSetProp::editHandles) {
			bool subModeKey = compareModKeyComponent(dat.modKey, continuous.alternateSubModeKey.modKey, false);
			bool altModeKey = (continuous.defaultMode == TSetProp::line) ?
				true : compareModKeyComponent(dat.modKey, continuous.alternateModeKey.modKey, false);
			if (subModeKey && altModeKey) {
				activeMode = TSetProp::editHandles;
				activePoint = nullptr;
				wasHandled = InputHandlerFlag::editMode;
			}
			else {
				activeMode = TSetProp::draw;
				generateCurve();
				wasHandled = InputHandlerFlag::releaseCurve;
			}
		}
		else if (activeMode != TSetProp::line &&
			compareModKeyComponent(dat.modKey, continuous.alternateModeKey, false) &&
			fragData.anchors.size() != 0)
		{
			activeMode = TSetProp::line;
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			glm::vec3 dir = makeDir(splineData.anchors.back().pos, pos);
			splineData.anchors.push_back(FragmentAnchor(splineIDCount, pos, dir, 1.0f, dat));
			wasHandled = InputHandlerFlag::release_continueInput;
		}
		else if (activeMode == TSetProp::line) {
			wasHandled = InputHandlerFlag::previewLine;
		}
		else {
			if (continuous.closeShape) {
				glm::vec3 pos = splineData.anchors.front().pos;
				glm::vec3 dir = makeDir(splineData.anchors.back().pos, pos);
				splineIDCount++;
				splineData.anchors.push_back(FragmentAnchor(splineIDCount, pos, dir, 1.0f, dat));
				generateVertices(&pos, &dir, &dat);
				wasHandled = InputHandlerFlag::releaseConnect;
			}
			else { wasHandled = InputHandlerFlag::release; }
		}
	}
	else { wasHandled = InputHandlerFlag::noSignal; }
	return wasHandled;
}


InputHandlerFlag InputMethod::continuousKey(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (action == InputAction::release) {
		if (activeMode == TSetProp::curves || activeMode == TSetProp::editHandles) {
			// If the key released is the sub-mode key
			bool subModeKey = compareModKey(continuous.alternateSubModeKey, convertKeybind_modKey(key), false);
			bool altModeKey = compareModKeyComponent(continuous.alternateModeKey, modKeys, false);
			if (subModeKey) {
				// If the alt mode key is currently held
				if ((continuous.defaultMode == TSetProp::line && !altModeKey) ||
					(continuous.defaultMode == TSetProp::line && altModeKey) ||
					(continuous.defaultMode == TSetProp::draw && altModeKey))
				{
					// Terminate current curve
					activeMode = TSetProp::line;
					generateCurve();
					// Begin next line
					glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
					glm::vec3 dir = makeDir(fragData.anchors.back().pos, pos);
					splineData.anchors.push_back(FragmentAnchor(splineIDCount, pos, dir, 1.0f, dat));
					wasHandled = InputHandlerFlag::releaseCurve;
				}
				else {
					// If now neither key is held, when in default-draw mode, finalize the curve and end input
					activeMode = TSetProp::draw;
					generateCurve();
					forceNewInput = true;
					wasHandled = InputHandlerFlag::finalizeCurve;
				}
			}
		}
		if (compareKey(key, InputKey::escape, false)) {
			activeMode = continuous.defaultMode;
			forceNewInput = true;
			wasHandled = InputHandlerFlag::terminateInput;
		}
	}
	return wasHandled;
}

bool InputMethod::continuousDraw(Application* sender, Input dat,
	TSet_ContinuousControl* continuousControl, TSet_Smoothing* smoothing,
	VertexData* target, float vertexSpacing, glm::vec3& outPos, glm::vec3 &outDir)
{
	// Kick bad-calls
	if (target->anchors.size() == 0) { return false; }
	
	
	// Add the input data
	addInputData(dat);

	int i;
	float length, angle;

	// Do setup of variables that all logic branches need
	i = (int)target->anchors.size() - 1;
	while (i >= 0 && (std::isnan(outDir.x) || std::isnan(outDir.y) || std::isnan(outDir.z)))
	{
		outDir = target->anchors[(size_t)i].dir;
		i--;
	}
	length = glm::length(outPos - target->anchors.back().pos);
	angle = glm::degrees(std::atan2f(outDir.y, outDir.x));

	// Determine if to set a constraint angle

	// Add both the constraint angle and it's opposite angle in order to snap to either direction
	// along the constraint axis when it's enabled
	if (continuousControl->isEnabled && !continuousControl->activated && length > ROOT2 * continuousControl->constraintLengthThreshold)
	{
		std::vector<float> constraintAngles;
		std::vector<float> constraintDifferences;
		std::vector<Keybind> constraintKeys;
		bool doConstraintA = continuousControl->enableConstrainA && compareModKey(continuousControl->constrainA_modKey, dat.modKey, false);
		if (doConstraintA)
		{
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainA_angle + 360));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainA_angle + 180));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintKeys.push_back(continuousControl->constrainA_modKey);
			constraintKeys.push_back(continuousControl->constrainA_modKey);
		}
		bool doConstraintB = continuousControl->enableConstrainB && compareModKey(continuousControl->constrainB_modKey, dat.modKey, false);
		if (doConstraintB)
		{
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainB_angle + 360));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainB_angle + 180));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintKeys.push_back(continuousControl->constrainB_modKey);
			constraintKeys.push_back(continuousControl->constrainB_modKey);
		}
		bool doConstraintC = continuousControl->enableConstrainC && compareModKey(continuousControl->constrainC_modKey, dat.modKey, false);
		if (doConstraintC)
		{
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainC_angle + 360));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainC_angle + 180));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintKeys.push_back(continuousControl->constrainC_modKey);
			constraintKeys.push_back(continuousControl->constrainC_modKey);
		}
		bool doConstraintD = continuousControl->enableConstrainD && compareModKey(continuousControl->constrainD_modKey, dat.modKey, false);
		if (doConstraintD)
		{
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainD_angle + 360));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainD_angle + 180));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintKeys.push_back(continuousControl->constrainD_modKey);
			constraintKeys.push_back(continuousControl->constrainD_modKey);
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
			continuousControl->activeKey = constraintKeys[std::distance(
				constraintDifferences.begin(), (lowValue <= highValue) ? lowIndex : highIndex)];
			continuousControl->activeAngle = clampAngle_180(selectAngle);
			continuousControl->origin = outPos;
			continuousControl->dir = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(continuousControl->activeAngle))) * DEFAULT_DIR;
			continuousControl->perpendicular = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(90.0f))) * continuousControl->dir;
			glm::vec3 constrainP2 = continuousControl->origin + (continuousControl->dir * 100.0f);
			continuousControl->line = glm::vec4(continuousControl->origin.x, continuousControl->origin.y, constrainP2.x, constrainP2.y);
			continuousControl->activated = true;
		}
	}


	// Return and do not add an anchor if the length is less than the diagonal of 1 pixel * vertexSpacing
	if (length < vertexSpacing) { return false; }
	if (continuousControl->activated)
	{
		glm::vec3 perpLineP2 = outPos + (continuousControl->perpendicular * 100.0f);
		glm::vec4 perpLine = glm::vec4(outPos.x, outPos.y, perpLineP2.x, perpLineP2.y);
		outPos = glm::vec3(lineIntersect2D(continuousControl->line, perpLine), 0.0f);
		outDir = glm::vec3(
			cos(continuousControl->activeAngle * (MATH_PI / 180.0f)), 
			sin(continuousControl->activeAngle * (MATH_PI / 180.0f)), 
			0.0f);
		if (!compareModKey(continuousControl->activeKey, dat.modKey, false))
		{
			continuousControl->clearConstraint();
		}
		// Must recheck the length again after adjustment, otherwise the input gets sluggish as it piles
		// up anchors on top of eachother
		length = glm::length(outPos - target->anchors.back().pos);
		if (length < vertexSpacing) { return false; }
	}
	else
	{
		// Adjust the direction on a wait-flag
		if (target->anchors.size() == 1 && target->anchors.front().wait > 0)
		{
			target->anchors.front().dir = outDir;
			target->anchors.front().wait = 0;
		}
		else
		{
			// Apply smoothing to the direction value as applicable
			glm::vec3 lastDir = target->anchors.back().dir;
			float angleChange = angle - glm::degrees(atan2(lastDir.x, lastDir.y));
			if (smoothing != nullptr && smoothing->isEnabled && smoothing->smoothDirection && angleChange >= smoothing->directionThreshold) {
				outDir = lerpDir(outDir, lastDir, smoothing->directionFactor);
			}
		}
	}
	return true;
}

bool InputMethod::continuousHover(Application* sender, Input dat,
	TSet_ContinuousControl* continuousControl, TSet_Smoothing* smoothing,
	VertexData* target, float vertexSpacing, glm::vec3& outPos, glm::vec3& outDir)
{
	// Kick bad-calls
	if (target->anchors.size() == 0) { return false; }


	// Add the input data
	addInputData(dat);

	int i;
	float length, angle;

	// Do setup of variables that all logic branches need
	i = (int)target->anchors.size() - 1;
	while (i >= 0 && (std::isnan(outDir.x) || std::isnan(outDir.y) || std::isnan(outDir.z)))
	{
		outDir = target->anchors[(size_t)i].dir;
		i--;
	}
	length = glm::length(outPos - target->anchors.back().pos);
	angle = glm::degrees(std::atan2f(outDir.y, outDir.x));

	// Determine if to set a constraint angle

	// Adjust the position and direction if angle snapping is enabled.
	if (activeMode == TSetProp::line && target->anchors.size() >= 2)
	{
		bool useAngleA = false; bool useAngleB = false;
		if (compareModKeyComponent(dat.modKey, continuousControl->altAngleSnapKeyA, false)) { useAngleA = true; }
		if (compareModKeyComponent(dat.modKey, continuousControl->altAngleSnapKeyB, false)) { useAngleB = true; }
		float angleA_dif = (useAngleA) ? abs(angle) - abs(floor(abs(angle / continuousControl->altAngleSnapA_angle)) * continuousControl->altAngleSnapA_angle) : -10.0f;
		float angleB_dif = (useAngleB) ? abs(angle) - abs(floor(abs(angle / continuousControl->altAngleSnapB_angle)) * continuousControl->altAngleSnapB_angle) : -10.0f;
		if (useAngleA || useAngleB) { 
			float outAngle = 0.0f;
			if (useAngleA && useAngleB) {
				outAngle = (angleA_dif < angleB_dif) ? continuousControl->altAngleSnapA_angle : continuousControl->altAngleSnapB_angle;
			}
			else if (useAngleA && !useAngleB) { outAngle = continuousControl->altAngleSnapA_angle; }
			else if (!useAngleA && useAngleB) { outAngle = continuousControl->altAngleSnapB_angle; }
			float angle1 = ceil(angle / outAngle) * outAngle;
			float angle2 = floor(angle / outAngle) * outAngle;
			outAngle = (abs(angle - angle1) < abs(angle - angle2)) ? glm::radians(angle1) : glm::radians(angle2);
			glm::vec3 snapDir = glm::vec3(cos(outAngle), sin(outAngle), 0.0f);
			glm::vec3 snapPos = target->anchors.at(target->anchors.size() - 2).pos + (snapDir * 100.0f);
			glm::vec4 line1 = glm::vec4(
				target->anchors.at(target->anchors.size() - 2).pos.x,
				target->anchors.at(target->anchors.size() - 2).pos.y,
				snapPos.x,
				snapPos.y
			);
			glm::vec3 perpendicularPoint = outPos + ((snapDir * glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)))) * 100.0f);
			glm::vec4 line2 = glm::vec4(
				outPos.x,
				outPos.y,
				perpendicularPoint.x,
				perpendicularPoint.y
			);
			outPos = glm::vec3(lineIntersect2D(line1, line2), 0.0f);
			outDir = makeDir(target->anchors.at(target->anchors.size() - 2).pos, outPos);
		}
		return true;
	}
	return false;
}

bool InputMethod::continuousEditHandles(InputHandlerFlag& result, Application* sender, Input dat)
{
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 dir = makeDir(splineData.anchors.back().pos, pos);
	size_t size = splineData.anchors.size();
	glm::vec3* points[3] = { &splineData.anchors.at(size - 2).headHandle, &splineData.anchors.at(size - 1).tailHandle, &splineData.anchors.at(size - 1).pos };
	float distances[3] = { glm::length(pos - *points[0]), glm::length(pos - *points[1]), glm::length(pos - *points[2]) };
	float minDist = 1e8;
	size_t minIndex = 0;
	for (size_t i = 0; i < 3; i++) {
		if (distances[i] < minDist) {
			minDist = distances[i];
			minIndex = i;
		}
	}
	if (minDist <= 25.0f) {
		activePoint = points[minIndex];
		result = InputHandlerFlag::editMode;
		if (activePoint == &splineData.anchors.at(size - 1).pos) {
			activePointLinked = &splineData.anchors.at(size - 1).tailHandle;
			activeOrigin = splineData.anchors.at(size - 1).tailHandle - splineData.anchors.at(size - 1).pos;
		}
		else {
			activePointLinked = nullptr;
		}
		return true;
	}
	else {
		activePoint = activePointLinked = nullptr;
		result = InputHandlerFlag::editMode;
		return false;
	}
}

bool InputMethod::dragMove(Application* sender, Input dat, 
	TSet_DragControl* dragControl, glm::vec3& cursorPos)
{
	// Pause the input if there hasn't been input movement
	glm::dvec2 lastInput = glm::dvec2(data.inputEvents.back().x, data.inputEvents.back().y);
	float moveTest = (float)glm::length(glm::dvec2(dat.x, dat.y) - lastInput);
	if (moveTest < ROOT2) { return false; }
	// Add the input data
	addInputData(dat);
	
	// Clear InputFlag::newInput
	if (fragData.anchors.size() != 0) { fragData.anchors.front().input.flagPrimary = InputFlag::null; }
	
	// Determine the input mode and settings
	fragData.centerAboutOrigin =
		(sender->getKeyState(dragControl->sizeAboutOriginKey.modKey)) ? true : false;
	bool extrudeMode = (sender->getKeyState(dragControl->extrudeModeKey.modKey) && dragControl->enableExtrusion) ? true : false;
	bool moveMode = (sender->getKeyState(dragControl->moveOriginModeKey.glfwKey)) ? true : false;
	bool snapMode = (sender->getKeyState(dragControl->snapModeKey.modKey)) ? true : false;
	// Set up the data
	glm::vec3 lastPos = sender->pickScreenCoord(lastInput.x, lastInput.y);
	glm::vec3 dir = glm::normalize(glm::vec3(
		fragData.transform.origin.x - cursorPos.x,
		fragData.transform.origin.y - cursorPos.y,
		0.0f));

	if (moveMode && !extrudeMode)
	{
		glm::vec3 movement = cursorPos - lastPos;
		fragData.transform.origin += movement;
		// Calculate for if centerAboutOrigin is on
		fragData.transform.bounds.p1 = fragData.transform.origin;
		fragData.transform.bounds.p4 = lastCursor = cursorPos;
	}
	else if (extrudeMode && !moveMode)
	{
		dir = glm::normalize(glm::vec3(
			fragData.transform.origin.x - lastCursor.x,
			fragData.transform.origin.y - lastCursor.y,
			0.0f));
		fragData.transform.bounds.p4 = lastCursor;
		startExtrude = distancePointLine2D(glm::dvec2(cursorPos), glm::dvec4(
			fragData.transform.origin.x,
			fragData.transform.origin.y,
			fragData.transform.bounds.p4.x,
			fragData.transform.bounds.p4.y));
	}
	// Otherwise apply length/rotation changes
	else
	{
		//cursorPos = fragData.transform.origin + (rotation * (cursorPos - fragData.transform.origin));
		if (snapMode)
		{
			// Warning: Unfinished area of program!
		}
		fragData.transform.bounds.p4 = lastCursor = cursorPos;
	}

	// Calculate for if centerAboutOrigin is on
	fragData.transform.bounds.p1 = (fragData.centerAboutOrigin) ?
		fragData.transform.origin + (dir * glm::length(fragData.transform.bounds.p4 - fragData.transform.origin)) :
		fragData.transform.origin;
	

	// Calculate the remaining bounds box points p2 and p3 and then recalculate p1 and p4 for extrusion
	// The points do not receive rotation transformation here. The transform.roll is bundled with the box so that
	// any rasterizer receiving the box can apply the transform virtually itself before rendering
	glm::vec3 normal = glm::quat(glm::vec3(0, 0, glm::radians(-90.0f))) * dir;
	fragData.transform.bounds.p2 = fragData.transform.bounds.p1 + (normal * startExtrude);
	fragData.transform.bounds.p3 = fragData.transform.bounds.p4 + (normal * startExtrude);
	fragData.transform.bounds.p1 = fragData.transform.bounds.p1 + (normal * -startExtrude);
	fragData.transform.bounds.p4 = fragData.transform.bounds.p4 + (normal * -startExtrude);
	// Unrotate all the verts and then set the roll value before exiting
	glm::quat unRotate = glm::quat(glm::vec3(0, 0, atan2(dir.x, dir.y)));
	fragData.transform.bounds.p1 = fragData.transform.origin + (unRotate * (fragData.transform.bounds.p1 - fragData.transform.origin));
	fragData.transform.bounds.p2 = fragData.transform.origin + (unRotate * (fragData.transform.bounds.p2 - fragData.transform.origin));
	fragData.transform.bounds.p3 = fragData.transform.origin + (unRotate * (fragData.transform.bounds.p3 - fragData.transform.origin));
	fragData.transform.bounds.p4 = fragData.transform.origin + (unRotate * (fragData.transform.bounds.p4 - fragData.transform.origin));
	fragData.transform.roll = -glm::degrees(atan2(dir.x, dir.y));
	return true;
}

bool InputMethod::onePointMove(Application* sender, Input dat, 
	TSet_OnePointControl* onePointControl, glm::vec3 &cursorPos)
{
	// Pause the input if there hasn't been input movement
	glm::dvec2 lastInput = glm::dvec2(data.inputEvents.back().x, data.inputEvents.back().y);
	float moveTest = (float)glm::length(glm::dvec2(dat.x, dat.y) - lastInput);
	if (moveTest < ROOT2) { return false; }
	// Add the input data
	addInputData(dat);

	// Clear InputFlag::newInput
	if (fragData.anchors.size() != 0) { fragData.anchors.front().input.flagPrimary = InputFlag::null; }

	// Determine the input mode and settings
	fragData.centerAboutOrigin =
		(sender->getKeyState(onePointControl->sizeAboutOriginKey.modKey)) ? true : false;
	bool rotateMode = (sender->getKeyState(onePointControl->rotateModeKey.modKey)) ? true : false;
	bool moveMode = (sender->getKeyState(onePointControl->moveOriginModeKey.glfwKey)) ? true : false;
	bool constrainRatio = (sender->getKeyState(onePointControl->constrainRatioKey.modKey)) ? true : false;
	// Set up the data
	glm::vec3 lastPos = sender->pickScreenCoord(lastInput.x, lastInput.y);
	glm::vec3 dir = glm::normalize(glm::vec3(
		fragData.transform.origin.x - cursorPos.x,
		fragData.transform.origin.y - cursorPos.y,
		0.0f));

	// Apply Movement if enabled. Cannot apply move and rotate at the same time
	if (moveMode && !rotateMode)
	{
		glm::vec3 movement = cursorPos - lastPos;
		fragData.transform.bounds.p1 += movement;
		fragData.transform.bounds.p3 += movement;
		fragData.transform.origin += movement;
	}
	// Apply rotation if enabled.
	else if (rotateMode && !moveMode)
	{
		glm::vec2 dir = glm::normalize(fragData.transform.origin - cursorPos);
		float angleIn = glm::degrees(atan2(dir.x, dir.y));
		float angleOut = 0.0f;
		if (startAngle <= -359.0f)
		{
			startAngle = clampAngle_180(angleIn + onePointControl->initialRotation);
			angleOut = 0.0f;
		}
		else
		{
			angleOut = startAngle - angleIn;
		}
		fragData.transform.roll = clampAngle_180(angleOut);
		if (constrainRatio)
		{
			float count = floor(fragData.transform.roll / onePointControl->constrainAngleIncrement);
			float remainder = fragData.transform.roll - (count * onePointControl->constrainAngleIncrement);
			fragData.transform.roll =
				(remainder < onePointControl->constrainAngleIncrement / 2.0f) ?
				count * onePointControl->constrainAngleIncrement : (count + 1) * onePointControl->constrainAngleIncrement;
		}
	}
	// Otherwise apply size changes
	else
	{
		glm::quat rotation = glm::quat(glm::vec3(0, 0, glm::radians(-fragData.transform.roll)));
		cursorPos = fragData.transform.origin + (rotation * (cursorPos - fragData.transform.origin));
		if (constrainRatio)
		{
			float x = cursorPos.x - fragData.transform.origin.x;
			float y = cursorPos.y - fragData.transform.origin.y;
			float s = (x * y < 0.0f) ? -1.0f : 1.0f;
			float a = (abs(x) > abs(y)) ? abs(y) : abs(x);
			a *= x / abs(x);
			fragData.transform.bounds.p3 = fragData.transform.origin + glm::vec3(a, a * s, 0);
		}
		else
		{
			fragData.transform.bounds.p3 = cursorPos;
		}
		// Calculate for if centerAboutOrigin is on
		fragData.transform.bounds.p1 = (fragData.centerAboutOrigin) ?
			fragData.transform.origin - (fragData.transform.bounds.p3 - fragData.transform.origin) :
			fragData.transform.origin;
	}
	// Calculate the remaining bounds box points p2 and p4
	// The points do not receive rotation transformation here. The transform.roll is bundled with the box so that
	// any rasterizer receiving the box can apply the transform virtually itself before rendering
	fragData.transform.bounds.p2 = fragData.transform.bounds.p1 + glm::vec3(fragData.transform.bounds.p3.x - fragData.transform.bounds.p1.x, 0.0f, 0.0f);
	fragData.transform.bounds.p4 = fragData.transform.bounds.p1 + glm::vec3(0.0f, fragData.transform.bounds.p3.y - fragData.transform.bounds.p1.y, 0.0f);
	return true;
}

void InputMethod::resetInput(Application* sender, Input dat, glm::vec3& point, glm::vec3& dir)
{
	// Reset measurements
	startAngle = -360.0f;
	startExtrude = 0.0f;
	// Get new copies of the control scheme settings
	switch (controlScheme)
	{
	case TSetType::continuous:
		continuous = *owner.get()->getContinuousControl();
		fragData.transform.roll = 0.0f;
		startExtrude = 0.0f;
		break;
	case TSetType::drag:
		drag = *owner.get()->getDragControl();
		fragData.centerAboutOrigin =
			(dat.modKey != InputModKey::none && compareModKey(drag.sizeAboutOriginKey, dat.modKey, false)) ? true : false;
		fragData.transform.roll = 0.0f;
		startExtrude = drag.initialExtrusion;
		break;
	case TSetType::onepoint:
		continuous = *owner.get()->getContinuousControl();
		fragData.centerAboutOrigin =
			(dat.modKey != InputModKey::none && compareModKey(drag.sizeAboutOriginKey, dat.modKey, false)) ? true : false;
		fragData.transform.roll = onePoint.initialRotation;
		startExtrude = 0.0f;
		break;
	}
	// Reset the data
	data.reset();
	data.start = dat;
	data.inputModKey = dat.modKey;
	// Push back a new data point
	addInputData(dat);
	// Reset the fragData
	fragData.reset(); splineData.reset();
	fragData.startTime = splineData.startTime = (float)data.start.time;
	// Set the upper left corner for the entity 
	point = sender->pickScreenCoord(dat.x, dat.y);
	dir = glm::vec3(0.0f);
	// Set Basic Transform Data
	fragData.transform.roll = startAngle;
	fragData.transform.origin = point;
	fragData.transform.bounds = RectBounds(point);
	// Reset the ID Counters
	anchorIDCount = 0;
	splineIDCount = 0;
	inputCount = 0;
}

InputHandlerFlag InputMethod::newInput_continuous(Application* sender, Input dat, 
	int waitCountVertex, int waitCountSpline, InputFlag vertexFlagSecondary, InputFlag splineFlagSecondary)
{
	InputHandlerFlag result = InputHandlerFlag::noSignal;
	if (owner.get()->checkInterestMask(TSetType::image)) { image = *owner.get()->getImage(); }
	else { image.isEnabled = false; }
	if (owner.get()->checkInterestMask(TSetType::smoothing)) { smoothing = *owner.get()->getSmoothing(); }
	else { smoothing.isEnabled = false; }

	glm::ivec2 canvasDimensions = sender->getCanvasDimensions();
	owner.get()->getContinuousControl()->updateTrueSpacing(owner, canvasDimensions.x, canvasDimensions.y);
	continuous = *owner.get()->getContinuousControl();
	continuous.initialize(&image.trueSpacing, &storedEntityCount, &image.tipSize, &canvasDimensions);
	bool altMode = compareModKey(dat.modKey, continuous.alternateModeKey.modKey, false);
	if (!altMode) { activeMode = continuous.defaultMode; }
	else if (altMode && continuous.defaultMode == TSetProp::line) { activeMode = TSetProp::draw; }
	else if (altMode && continuous.defaultMode == TSetProp::draw) { activeMode = TSetProp::line; }

	resetData(dat, true, false, false, false);
	// Set the upper left corner for the entity 
	glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
	fragData.transform.origin = splineData.transform.origin = origin;
	glm::vec3 dir = sender->getMouseDirection(0.9f);

	this->initializeVertices(&origin, &dir, &dat, waitCountVertex, waitCountSpline, vertexFlagSecondary, splineFlagSecondary);
	result = InputHandlerFlag::allowPress_updateCursor;

	return result;
}

InputHandlerFlag InputMethod::connectInput_continuous(Application* sender, Input* dat)
{
	InputHandlerFlag result = InputHandlerFlag::noSignal;
	// Reset the end-input && end-time
	data.end.resetAll();
	fragData.endTime = 0.0;
	// Push back a new data point
	addInputData(*dat);
	// Determine the anchor position and direction
	glm::vec3 pos = sender->pickScreenCoord(dat->x, dat->y);
	glm::vec3 dir = makeDir(splineData.anchors.back().pos, pos);

	splineData.anchors.back().headType = splineData.anchors.back().tailType = HandleType::manual;
	splineData.anchors.back().handleRelationship = HandleRel::independent;
	splineIDCount++;
	splineData.anchors.push_back(FragmentAnchor(anchorIDCount, pos, dir, 1.0f, data.inputEvents.back()));
	generateVertices(&pos, &dir, dat);

	result = InputHandlerFlag::continueInput;
	return result;
}

void InputMethod::addInputData(Input dat)
{
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }
}

void InputMethod::resetData(Input dat, bool linearStream, bool centerAboutOrigin, bool connectEnds, bool constantSize)
{
	anchorIDCount = splineIDCount = 0;
	// Reset the data
	data.reset();
	data.start = dat;
	data.inputModKey = dat.modKey;
	// Push back a new data point
	addInputData(dat);
	// Reset the fragData
	fragData.reset();
	splineData.reset();
	fragData.linearStream = splineData.linearStream = linearStream;
	fragData.centerAboutOrigin = splineData.centerAboutOrigin = centerAboutOrigin;
	fragData.connectEnds = splineData.connectEnds = connectEnds;
	fragData.constantSize = splineData.constantSize = constantSize;
	fragData.startTime = splineData.startTime = (float)data.start.time;
	forceNewInput = false;
}

void InputMethod::clearFlagNew(bool clear)
{
	if (clear) { splineData.anchors.front().input.flagPrimary = InputFlag::null; }
}