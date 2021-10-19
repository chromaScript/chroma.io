#include "../../include/methods/inputMethods/In_Rotate.h"
#include "../../include/methods/InputMethod.h"
#include "../../include/tool/ToolSettings.h"
#include "../../include/tool/toolSettings/ToolSettings_Forward.h"
#include "../../include/tool/Tool.h"

#ifndef APPLICATION_H
#include "../../include/Application.h"
#endif

#include <iostream>

InputHandlerFlag In_Rotate::move(Application* sender, Input dat)
{
	bool isNew = (fragData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
	// If the array is empty, insert the first element, otherwise overwrite it with the passed Input
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }
	// Adjust the anchor points
	fragData.anchors.front().input.flagPrimary = InputFlag::null;
	// Add the anchor
	if (fragData.anchors.size() >= 2) { fragData.anchors.erase(fragData.anchors.begin()); }
	glm::vec3 pos = glm::vec3(dat.x, dat.y, 0.0f);
	// Special scenario for Rotate where FLAG after the first is set as the modKeys, use INPUT_MOD_[key]
	fragData.anchors.push_back(
		FragmentAnchor(anchorIDCount, pos, DEFAULT_DIR,
			1.0f,
			HandleType::linear, false, pos,
			HandleType::linear, false, pos,
			HandleRel::independent,
			data.inputEvents.back()));
	if (isNew) { fragData.anchors.front().input.flagPrimary = InputFlag::null; }

	return InputHandlerFlag::allowPress;
}

InputHandlerFlag In_Rotate::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	// Warning: This logic branch has no fail-case
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
	{
		// Get the settings
		rotate = *owner.get()->getRotate();
		// For new inputs, clear the current array, and then set start to the passed Input
		data.reset();
		data.start = dat;
		data.inputModKey = dat.modKey;
		// Push back a new data point
		data.inputEvents.push_back(dat);
		// Reset the fragData
		fragData.reset();
		fragData.constantSize = true;
		fragData.centerAboutOrigin = fragData.connectEnds = false;
		fragData.startTime = (float)data.start.time;
		// Set the upper left corner for the entity 
		glm::vec3 pos = glm::vec3(dat.x, dat.y, 0.0f);
		glm::vec3 origin = glm::vec3(0.0f);
		if (rotate.rotateAboutCursor) {
			origin = glm::vec3(dat.x, dat.y, 0.0f);
		}
		else {
			origin = glm::vec3(sender->getWindowWidth() / 2, sender->getWindowHeight() / 2, 0.0f);
		}
		fragData.transform.origin = origin;
		// Add the first anchor point
		fragData.anchors.push_back(
			FragmentAnchor(anchorIDCount, pos, DEFAULT_DIR,
				1.0f,
				HandleType::linear, false, pos,
				HandleType::linear, false, pos,
				HandleRel::independent,
				data.inputEvents.back()));
		fragData.anchors.back().input.flagPrimary = InputFlag::newInput;
		wasHandled = InputHandlerFlag::allowPress_updateCursor;
		//std::cout << "MethodType::in_rotate::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		// For Drag, Only interested in the start, last inputEvent, & end events
		data.end = dat;
		fragData.endTime = (float)data.end.time;
		wasHandled = InputHandlerFlag::release;
		//std::cout << "MethodType::in_rotate::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
		//std::cout << "MethodType::in_rotate::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "MethodType::in_rotate::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}