#include "../../include/methods/inputMethods/In_Zoom.h"
#include "../../include/methods/InputMethod.h"
#include "../../include/tool/ToolSettings.h"
#include "../../include/tool/toolSettings/ToolSettings_Forward.h"
#include "../../include/tool/Tool.h"

#ifndef APPLICATION_H
#include "../../include/Application.h"
#endif

#include <iostream>

InputHandlerFlag In_Zoom::move(Application* sender, Input dat)
{
	bool isNew = (fragData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
	// If the array is empty, insert the first element, otherwise overwrite it with the passed Input
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }

	// Add the anchor
	if (fragData.anchors.size() >= maxBufferLength) { fragData.anchors.erase(fragData.anchors.begin()); }
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	float zoomInterp = (dat.modKey == InputModKey::alt) ? -1.0f : 1.0f;
	fragData.anchors.push_back(
		FragmentAnchor(anchorIDCount, pos, DEFAULT_DIR,
			zoomInterp,
			HandleType::linear, false, pos,
			HandleType::linear, false, pos,
			HandleRel::independent,
			data.inputEvents.back()));
	// Set to scrub-mode if the distance has shifted far enough
	if ((fragData.anchors.back().pos.y - fragData.transform.origin.y) >= zoom.scrubThreshold) { fragData.anchors.back().input.flagSecondary = InputFlag::scrub; }
	// Clear New Flag
	if (isNew) { fragData.anchors.front().input.flagPrimary = InputFlag::null; }

	return InputHandlerFlag::allowPress;
}

InputHandlerFlag In_Zoom::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	// Warning: This logic branch has no fail-case
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
	{
		// Get the settings
		zoom = *owner.get()->getZoom();
		// For new inputs, clear the current array, and then set start to the passed Input
		data.inputEvents.clear();
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
		glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
		fragData.transform.origin = origin;
		// Add the first anchor point
		// Use zoomInterp to capture when 'Alt' is used with a click-input
		float zoomInterp = (dat.modKey == InputModKey::alt) ? -1.0f : 1.0f;
		fragData.anchors.push_back(
			FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				zoomInterp,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				data.inputEvents.back()));
		fragData.anchors.back().input.flagPrimary = InputFlag::newInput;
		wasHandled = InputHandlerFlag::allowPress_updateCursor;
		//std::cout << "MethodType::in_pan::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		// For Drag, Only interested in the start, last inputEvent, & end events
		data.end = dat;
		fragData.endTime = (float)data.end.time;
		wasHandled = InputHandlerFlag::release;
		//std::cout << "MethodType::in_zoom::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
		//std::cout << "MethodType::in_zoom::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "MethodType::in_zoom::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}

InputHandlerFlag In_Zoom::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	return InputHandlerFlag::noSignal;
}