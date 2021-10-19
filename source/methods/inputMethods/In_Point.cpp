#include "../../include/methods/inputMethods/In_Point.h"
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

InputHandlerFlag In_Point::move(Application* sender, Input dat)
{
	/*
	// If the array is empty, insert the first element, otherwise overwrite it with the passed Input
	if (data.inputEvents.size() < 4) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }
	// Adjust the anchor points
	fragData.anchors.front().flagPrimary = InputFlag::null;
	// Update the Anchor
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	fragData.anchors = { FragmentAnchor(InputFlag::null, 0, pos, glm::vec3(0, 0, 0), 1.0f,
		FHANDLE_LINEAR, false, pos,
		FHANDLE_LINEAR, false, pos,
		FHANDLE_REL_INDEPENDENT,
		data.inputEvents.back().pressure, data.inputEvents.back().tiltX,
		data.inputEvents.back().tiltY, data.inputEvents.back().rotation,
		(float)data.start.time, sender->getMouseVelocity(), 1.0f) };
		*/
	return InputHandlerFlag::allowPress_updateCursor;
}

InputHandlerFlag In_Point::click(Application* sender, Input dat)
{
	/*
	int wasHandled;
	Input inClick = dat;
	// Warning: This logic branch has no fail-case
	if (inClick.action == InputAction::press)
	{
		// Get the settings
		sampler = *owner.get()->getSampler();
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
		glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
		fragData.transform.origin = origin;
		// Add the first and only anchor point
		fragData.anchors.push_back(FragmentAnchor(InputFlag::newInput, 0, origin, glm::vec3(0, 0, 0), 1.0f,
			FHANDLE_LINEAR, false, origin,
			FHANDLE_LINEAR, false, origin,
			FHANDLE_REL_INDEPENDENT,
			data.inputEvents.back().pressure, data.inputEvents.back().tiltX,
			data.inputEvents.back().tiltY, data.inputEvents.back().rotation,
			(float)data.start.time, sender->getMouseVelocity(), 1.0f));
		wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
		std::cout << "MethodType::in_point::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (inClick.action == InputAction::release)
	{
		// For Drag, Only interested in the start, last inputEvent, & end events
		data.end = dat;
		fragData.endTime = (float)data.end.time;
		wasHandled = INPUT_ALLOW_RELEASE;
		std::cout << "MethodType::in_point::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = INPUT_NOSIGNAL;
		std::cout << "MethodType::in_point::CLICK::NOSIGNAL" << std::endl;
	}
	std::cout << "MethodType::in_point::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;

	return wasHandled;
	*/
	return InputHandlerFlag::noSignal;
}