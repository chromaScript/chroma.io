#include "../../include/methods/inputMethods/In_Sampler.h"
#include "../../include/methods/InputMethod.h"
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

InputHandlerFlag In_Sampler::move(Application* sender, Input dat)
{
	fragData.activeModKey = dat.modKey;

	// If the array is empty, insert the first element, otherwise overwrite it with the passed Input
	bool isNew = (fragData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }
	// Update the Anchor
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 dir = glm::normalize(pos - fragData.anchors.back().pos);
	glm::vec3 screenPos = glm::vec3((float)dat.x, (float)dat.y, 0.0f);
	// Only update the anchors if there was a significant movement
	InputHandlerFlag result = InputHandlerFlag::noSignal;
	if (glm::length(fragData.anchors.back().pos - screenPos) > ROOT2 * 1.2f)
	{
		fragData.anchors = {
			FragmentAnchor(anchorIDCount, pos, dir,
				1.0f,
				HandleType::linear, false, pos,
				HandleType::linear, false, pos,
				HandleRel::independent,
				data.inputEvents.back()),
			FragmentAnchor(anchorIDCount, screenPos, dir,
				1.0f,
				HandleType::linear, false, screenPos,
				HandleType::linear, false, screenPos,
				HandleRel::independent,
				data.inputEvents.back()) };
		anchorIDCount++;
		result = InputHandlerFlag::allowPress_updateCursor;
	}
	else
	{
		result = InputHandlerFlag::wait;
	}
	if (isNew) { fragData.anchors.front().input.flagPrimary = InputFlag::null; }
	return result;
}

InputHandlerFlag In_Sampler::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	Input inClick = dat;
	// Warning: This logic branch has no fail-case
	if (inClick.action == InputAction::press && dat.button == InputMouseButton::left)
	{
		// Get the settings
		sampler = *owner.get()->getSampler();
		float trueSpacing = 1.0f; 
		glm::ivec2 canvasDimensions = sender->getCanvasDimensions();
		sampler.initialize(&trueSpacing, &sampleCount, &canvasDimensions);
		sampleCount++;
		// For new inputs, clear the current array, and then set start to the passed Input
		anchorIDCount = 0;
		data.reset();
		data.start = dat;
		data.inputModKey = dat.modKey;
		fragData.activeModKey = dat.modKey;
		// Push back a new data point
		data.inputEvents.push_back(dat);
		// Reset the fragData
		fragData.reset();
		fragData.constantSize = true;
		fragData.centerAboutOrigin = fragData.connectEnds = false;
		fragData.startTime = (float)data.start.time;
		// Set the upper left corner for the entity 
		glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
		fragData.transform.origin = origin;
		// Add the first and second anchor points
		// The second anchor point is used to hold the raw screen-coordinates
		fragData.anchors.push_back(
			FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				1.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				data.inputEvents.back()));
		glm::vec3 screenPos = glm::vec3((float)dat.x, (float)dat.y, 0.0f);
		fragData.anchors.push_back(
			FragmentAnchor(anchorIDCount, screenPos, DEFAULT_DIR,
				1.0f,
				HandleType::linear, false, screenPos,
				HandleType::linear, false, screenPos,
				HandleRel::independent,
				data.inputEvents.back()));
		fragData.anchors.front().input.flagPrimary = InputFlag::newInput;
		anchorIDCount++;
		wasHandled = InputHandlerFlag::allowPress_updateCursor;
		//std::cout << "MethodType::in_point::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (inClick.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		// For Drag, Only interested in the start, last inputEvent, & end events
		data.end = dat;
		fragData.activeModKey = dat.modKey;
		fragData.endTime = (float)data.end.time;
		wasHandled = InputHandlerFlag::release;
		//std::cout << "MethodType::in_point::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
		//std::cout << "MethodType::in_point::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "MethodType::in_point::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}

InputHandlerFlag In_Sampler::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	return InputHandlerFlag::noSignal;
}