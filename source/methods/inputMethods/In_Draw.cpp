#include "../../include/methods/inputMethods/In_Draw.h"
#include "../../include/tool/ToolSettings.h"
#include "../../include/tool/toolSettings/ToolSettings_Forward.h"
#include "../../include/tool/Tool.h"

#ifndef APPLICATION_H
#include "../../include/Application.h"
#endif

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>

#include <iostream>
#include <algorithm>
#include <iostream>

////////////////////////////////////////////////////////////////
//
// Move Function
//
////////////////////////////////////////////////////////////////
InputHandlerFlag In_Draw::move(Application* sender, Input dat)
{
	//std::cout << "MethodType::in_draw::MOVE" << std::endl;
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 dir = makeDir(fragData.anchors.back().pos, pos);
	fragData.activeModKey = splineData.activeModKey = dat.modKey;

	// Continuous :: Lines Mode
	if (activeMode == TSetProp::line && dat.action == InputAction::move) {
		continuousHover(sender, dat, &continuous, &smoothing, &splineData, continuous.trueSpacing, pos, dir);
		glm::vec3 dirSpline = pos - splineData.anchors[splineData.anchors.size() - 2].pos;
		splineData.anchors[splineData.anchors.size() - 2].dir = dirSpline;
		splineData.anchors.back() = FragmentAnchor(anchorIDCount, pos, dirSpline,
			1.0f,
			HandleType::linear, false, pos,
			HandleType::linear, false, pos,
			HandleRel::independent,
			dat);
		splineData.anchors.back().input.pressure = 1.0f;
		if (continuous.subdivide) {
			glm::vec3 pos1 = (splineData.anchors.size() >= 2) ? splineData.anchors[splineData.anchors.size() - 2].pos : splineData.transform.origin;
			glm::vec3 pos2 = splineData.anchors.back().pos;
			splineData.subdivCount = int(floor((glm::length(pos2 - pos1) / continuous.trueSpacing) * continuous.subdivideAmount));
		}
		else {
			splineData.subdivCount = 0;
		}
		return InputHandlerFlag::previewLine;
	}
	// Continuous :: Lines Mode :: Curve
	else if (activeMode == TSetProp::curves && 
		dat.action == InputAction::press && 
		compareModKeyComponent(dat.modKey, continuous.alternateSubModeKey.modKey, false))
	{
		addInputData(dat);
		size_t size = splineData.anchors.size();
		// Update the start/end curve vertices metadata
		splineData.anchors[size - 3].headControl = splineData.anchors[size - 1].tailControl = true;
		splineData.anchors[size - 3].headType = splineData.anchors[size - 1].tailType = HandleType::manual;
		// Move the point for the cursor position
		splineData.anchors[size - 2] = FragmentAnchor(splineIDCount, pos, dir,
			1.0f,
			HandleType::manual, false, pos,
			HandleType::manual, false, pos,
			HandleRel::independent,
			dat);
		// Move the handle points for the adjacent points
		glm::vec3 lineDir = makeDir(splineData.anchors[size - 3].pos, splineData.anchors[size - 1].pos);
		float lineLen = glm::length(splineData.anchors[size - 1].pos - splineData.anchors[size - 3].pos) / 2.0f;
		float pressure = clampf(pow((sender->stylusInRange()) ? averageInputs(&data, 12, 0.15f, 0.6f, true).pressure : 0.8f, 3.2f) / 0.85f, 0.0f, 1.0f);
		lineLen = lerpf(0.0f, lineLen, pressure);
		// Update the positions/directions
		glm::vec3 leftPos = pos - (lineDir * lineLen);
		glm::vec3 leftDir = makeDir(splineData.anchors[size - 3].pos, leftPos);
		glm::vec3 rightPos = pos + (lineDir * lineLen);
		glm::vec3 rightDir = makeDir(rightPos, splineData.anchors[size - 1].pos);
		splineData.anchors[size - 3].headHandle = leftPos;
		splineData.anchors[size - 3].dir = leftDir;
		splineData.anchors[size - 1].tailHandle = rightPos;
		splineData.anchors[size - 1].dir = rightDir;
		return InputHandlerFlag::previewCurves;
	}
	else if (activeMode == TSetProp::editHandles &&
		compareModKeyComponent(dat.modKey, continuous.alternateSubModeKey.modKey, false))
	{
		if (activePoint != nullptr) {
			*activePoint = pos;
		}
		return InputHandlerFlag::editMode;
	}
	// Continuous :: Draw Mode
	else {
		bool isNew = (splineData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
		if (controlScheme == TSetType::continuous)
		{
			if (!continuousMove(sender, dat, &continuous, &smoothing, &splineData, continuous.trueSpacing, pos, dir)) { return InputHandlerFlag::wait; }
			splineIDCount++;
			splineData.anchors.push_back(FragmentAnchor(splineIDCount, pos, dir,
				1.0f,
				HandleType::linear, false, pos,
				HandleType::linear, false, pos,
				HandleRel::independent,
				dat));

			int size = (int)splineData.anchors.size();
			int count = 1;
			count = continuous.modulateCount(count, &pos, &splineData.transform.origin, &dir, &dat, &size, &size, &size);
			glm::vec3 splinePos = pos;
			for (int i = 0; i < count; i++) {
				anchorIDCount++;
				splinePos = pos;
				continuous.modulatePosition(splinePos, &image.tipSize, &pos, &splineData.transform.origin, &dir, &dat, &anchorIDCount, &splineIDCount, &splineIDCount);
				dir = makeDir(fragData.anchors.back().pos, splinePos);
				fragData.anchors.push_back(FragmentAnchor(anchorIDCount, splinePos, dir,
					1.0f,
					HandleType::linear, false, splinePos,
					HandleType::linear, false, splinePos,
					HandleRel::independent,
					dat));
				if (continuous.connectInterval > 1 && anchorIDCount % continuous.connectInterval == 0) {
					fragData.anchors.back().input.flagSecondary = InputFlag::point;
				}
			}
		}
		
		if (isNew)
		{
			splineData.anchors.front().input.flagPrimary = InputFlag::null;
			fragData.anchors.front().input.flagPrimary = InputFlag::null;
		}
		return InputHandlerFlag::allowPress;
	}
}

////////////////////////////////////////////////////////////////
//
// Click Function
//
////////////////////////////////////////////////////////////////
InputHandlerFlag In_Draw::click(Application* sender, Input dat)
{
	std::cout << "CLICK::" << static_cast<int>(dat.action) << " :: " << TSetPropToString(activeMode) << std::endl;
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
		// Scenario in which 'Shift' is used to connect the end of the anchors to the next one
		// Keep previous anchor data and simply continue adding to it
		else if (fragData.anchors.size() != 0 && compareModKey(dat.modKey, continuous.connectLastStrokeKey.modKey, false))
		{
			// Reset the end-input && end-time
			data.end.resetAll();
			fragData.endTime = 0.0;
			// Push back a new data point
			addInputData(dat);
			// Determine the anchor position and direction
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			glm::vec3 dir = makeDir(splineData.anchors.back().pos, pos);
			// Insert new anchor, set dirInterpFactor to 0.0f so that the stroke will hard-snap
			// the shards to the new direction rather than smoothly interpolate the change
			splineData.anchors.back().headType = splineData.anchors.back().tailType = HandleType::manual;
			splineData.anchors.back().handleRelationship = HandleRel::independent;
			splineIDCount++;
			splineData.anchors.push_back(FragmentAnchor(splineIDCount, pos, dir,
				0.0f,
				HandleType::linear, false, pos,
				HandleType::linear, false, pos,
				HandleRel::independent,
				dat));
			generateVertices(&pos, &dir, &dat);
			wasHandled = InputHandlerFlag::continueInput;
		}
		else if (compareModKeyComponent(dat.modKey, continuous.alternateSubModeKey.modKey, false))
		{
			activeMode = TSetProp::curves;
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			glm::vec3 dir = makeDir(splineData.anchors.back().pos, pos);
			splineIDCount++;
			splineData.anchors.push_back(FragmentAnchor(splineIDCount, pos, dir,
				0.0f,
				HandleType::linear, false, pos,
				HandleType::linear, false, pos,
				HandleRel::independent,
				dat));
			wasHandled = InputHandlerFlag::allowPress;
		}
		else if (activeMode == TSetProp::line)
		{
			activeMode = TSetProp::draw;
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			glm::vec3 dir = makeDir(fragData.anchors.back().pos, pos);
			generateVertices(&pos, &dir, &dat);
			wasHandled = InputHandlerFlag::allowPress;
		}
		else if (activeMode == TSetProp::curves)
		{
			activeMode = TSetProp::draw;
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			glm::vec3 dir = makeDir(fragData.anchors.back().pos, pos);
			generateVertices(&pos, &dir, &dat);
			wasHandled = InputHandlerFlag::allowPress;
		}
		// A new set of anchors is started
		else
		{
			// Get new copies of the tool settings
			// Once a new input has begun, the order has been 'sent' and whatever
			// settings were in place at that time is what the output will
			// use to render the input. After this point, the verticies and tool settings
			// for the output fragment may only be changed using vertex/anchor manipulation tools
			if (owner.get()->checkInterestMask(TSetType::image)) { image = *owner.get()->getImage(); }
			else { image.isEnabled = false; }
			if (owner.get()->checkInterestMask(TSetType::smoothing)) { smoothing = *owner.get()->getSmoothing(); }
			else { smoothing.isEnabled = false; }

			switch (controlScheme)
			{
			case TSetType::continuous:
			default:
				glm::ivec2 canvasDimensions = sender->getCanvasDimensions();
				owner.get()->getContinuousControl()->updateTrueSpacing(owner, canvasDimensions.x, canvasDimensions.y);
				continuous = *owner.get()->getContinuousControl();
				continuous.initialize(&image.trueSpacing, &storedEntityCount, &image.tipSize, &canvasDimensions);
				activeMode = TSetProp::draw;
			}
			// Reset the data
			data.reset();
			data.start = dat;
			data.inputModKey = dat.modKey;
			// Push back a new data point
			addInputData(dat);
			// Reset the fragData
			fragData.reset();
			splineData.reset();
			fragData.centerAboutOrigin = fragData.connectEnds = fragData.constantSize = false;
			splineData.centerAboutOrigin = splineData.connectEnds = splineData.constantSize = false;
			fragData.startTime = splineData.startTime = (float)data.start.time;
			// Set the upper left corner for the entity 
			glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
			fragData.transform.origin = splineData.transform.origin = origin;
			// Add the first anchor point, there is no direction yet, so a manual wait will be placed on the
			// anchor to count off 1 more anchor before rendering.
			anchorIDCount = splineIDCount = 0;
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR, 
				1.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				dat));
			splineData.anchors.push_back(FragmentAnchor(splineIDCount, origin, DEFAULT_DIR,
				1.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				dat));
			fragData.anchors.back().wait = 1;
			splineData.anchors.back().wait = 1;
			fragData.anchors.back().input.flagPrimary = InputFlag::newInput;
			splineData.anchors.back().input.flagPrimary = InputFlag::newInput;
			wasHandled = InputHandlerFlag::allowPress_updateCursor;
		}
		//std::cout << "MethodType::in_draw::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		data.end = dat;
		if (fragData.anchors.size() == 1) { fragData.anchors.back().input.flagPrimary = InputFlag::null; }
		fragData.activeModKey = splineData.activeModKey = dat.modKey;
		fragData.endTime = splineData.endTime = (float)data.end.time;
		if (activeMode == TSetProp::curves || activeMode == TSetProp::editHandles) {
			bool subModeKey = compareModKeyComponent(dat.modKey, continuous.alternateSubModeKey.modKey, false);
			bool altModeKey = compareModKeyComponent(dat.modKey, continuous.alternateModeKey.modKey, false);
			if (subModeKey && altModeKey) {
				activeMode = TSetProp::editHandles;
				activePoint = nullptr;
				wasHandled = InputHandlerFlag::editMode;
			}
			else {
				activeMode = TSetProp::draw;
				fragData.anchors.back() = splineData.anchors[splineData.anchors.size() - 3];
				fragData.anchors.push_back(splineData.anchors.back());
				fragData.depth = 1;
				fragData.anchors.back().input.flagSecondary = InputFlag::updateData;
				wasHandled = InputHandlerFlag::releaseCurve;
			}
		}
		else if (compareModKeyComponent(dat.modKey, continuous.alternateModeKey, false) && fragData.anchors.size() != 0) {
			activeMode = TSetProp::line;
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			glm::vec3 dir = makeDir(fragData.anchors.back().pos, pos);
			splineData.anchors.push_back(FragmentAnchor(splineIDCount, pos, dir,
				1.0f,
				HandleType::linear, false, pos,
				HandleType::linear, false, pos,
				HandleRel::independent,
				dat));
			wasHandled = InputHandlerFlag::release_continueInput;
		}
		else {
			if (continuous.closeShape) {
				glm::vec3 pos = splineData.anchors.front().pos;
				glm::vec3 dir = makeDir(splineData.anchors.back().pos, pos);
				splineIDCount++;
				splineData.anchors.push_back(FragmentAnchor(splineIDCount, pos, dir,
					1.0f,
					HandleType::linear, false, pos,
					HandleType::linear, false, pos,
					HandleRel::independent,
					dat));
				generateVertices(&pos, &dir, &dat);
				wasHandled = InputHandlerFlag::releaseConnect;
			}
			else { wasHandled = InputHandlerFlag::release; }
		}
		//std::cout << "MethodType::in_draw::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
		//std::cout << "MethodType::in_draw::CLICK::NOSIGNAL" << std::endl;
	}
	std::cout << "END-CLICK::" << static_cast<int>(dat.action) << " :: " << TSetPropToString(activeMode) << std::endl;
	//std::cout << "MethodType::in_draw::CLICK::TIME=" << dat.time << "::TYPE=" << keybindToString(Keybind(InputKey::unknown, data.inputModKey)) << std::endl;
	return wasHandled;
}

InputHandlerFlag In_Draw::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	std::cout << "KEY::" << static_cast<int>(key.glfwKey) << " ACTION::" << static_cast<int>(action) << " :: " << TSetPropToString(activeMode) << std::endl;
	if (static_cast<int>(dat.action) == 1 && activeMode == TSetProp::editHandles) {
		int k = 5;
	}
	if (static_cast<int>(key.glfwKey) == 3410 && action == InputAction::release && activeMode == TSetProp::editHandles) {
		int k = 5;
	}
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (action == InputAction::release) {
		if (activeMode == TSetProp::curves || activeMode == TSetProp::editHandles) {
			// If the key released is the sub-mode key
			if (compareModKey(continuous.alternateSubModeKey, convertKeybind_modKey(key), false)) {
				// If the alt mode key is currently held
				if (compareModKeyComponent(continuous.alternateModeKey, modKeys, false)) {
					// Terminate current curve
					activeMode = TSetProp::line;
					fragData.anchors.back() = splineData.anchors[splineData.anchors.size() - 3];
					fragData.anchors.push_back(splineData.anchors.back());
					fragData.depth = 1;
					fragData.anchors.back().input.flagSecondary = InputFlag::updateData;
					// Begin next line
					glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
					glm::vec3 dir = makeDir(fragData.anchors.back().pos, pos);
					splineData.anchors.push_back(FragmentAnchor(splineIDCount, pos, dir,
						1.0f,
						HandleType::linear, false, pos,
						HandleType::linear, false, pos,
						HandleRel::independent,
						dat));
					wasHandled = InputHandlerFlag::releaseCurve;
				}
				else if (!compareModKeyComponent(continuous.alternateModeKey, modKeys, false)) {
					// If now neither key is held, finalize the curve and end input
					activeMode = TSetProp::draw;
					fragData.anchors.back() = splineData.anchors[splineData.anchors.size() - 3];
					fragData.anchors.push_back(splineData.anchors.back());
					fragData.depth = 1;
					fragData.anchors.back().input.flagSecondary = InputFlag::updateData;
					wasHandled = InputHandlerFlag::finalizeCurve;
				}
			}
		}
		if (compareKey(key, InputKey::escape, false)) {
			activeMode = TSetProp::draw;
			wasHandled = InputHandlerFlag::terminateInput;
		}
	}
	std::cout << "END-KEY::" << static_cast<int>(key.glfwKey) << " ACTION::" << static_cast<int>(action) << " :: " << TSetPropToString(activeMode) << std::endl;
	
	return wasHandled;
}

void In_Draw::addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{
	int count = 1;
	if (continuous.scatterConnections) { 
		count = continuous.modulateCount(count, pos, &splineData.transform.origin, dir, dat, &anchorIDCount, &splineIDCount, &splineIDCount); 
	}
	glm::vec3 splinePos = *pos;
	glm::vec3 splineDir = *dir;
	for (int i = 0; i < count; i++) {
		anchorIDCount++;
		splinePos = *pos;
		if (continuous.scatterConnections) { 
			continuous.modulatePosition(splinePos, &image.tipSize, pos, &splineData.transform.origin, dir, dat, &anchorIDCount, &splineIDCount, &splineIDCount);
		}
		splineDir = makeDir(fragData.anchors.back().pos, splinePos);
		fragData.anchors.push_back(FragmentAnchor(anchorIDCount, splinePos, splineDir,
			1.0f,
			HandleType::linear, false, splinePos,
			HandleType::linear, false, splinePos,
			HandleRel::independent,
			*dat));
		if (continuous.connectInterval > 1 && anchorIDCount % continuous.connectInterval == 0) {
			fragData.anchors.back().input.flagSecondary = InputFlag::point;
		}
	}
}

void In_Draw::generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{
	glm::vec3 pos1 = splineData.anchors[splineData.anchors.size() - 2].pos;
	glm::vec3 dir1 = splineData.anchors[splineData.anchors.size() - 2].dir;
	glm::vec3 pos2 = *pos;
	glm::vec3 dir2 = *dir;
	float pressure1 = splineData.anchors[splineData.anchors.size() - 2].input.pressure;
	float pressure2 = dat->pressure;
	glm::vec3 subPos = pos2; glm::vec3 subDir = dir2;
	int subdivCount = (continuous.subdivide) ? int(floor((glm::length(pos2 - pos1) / continuous.trueSpacing) * continuous.subdivideAmount)) : 0;
	if (subdivCount == 0) {
		addVertices(&subPos, &subDir, dat);
	}
	else {
		for (int i = 1; i < subdivCount + 1; i++) {
			float t = float(i) / float(subdivCount);
			subPos = lerpPos(pos1, pos2, t);
			subDir = makeDir(fragData.anchors.back().pos, subPos);
			Input lerpDat = *dat;
			lerpDat.pressure = lerpf(pressure1, pressure2, t);
			addVertices(&subPos, &subDir, &lerpDat);
		}
	}
}