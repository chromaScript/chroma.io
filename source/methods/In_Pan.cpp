#include "../include/methods/In_Pan.h"
#include "../include/methods/InputMethod.h"
#include "../include/ToolSettings.h"
#include "../include/toolSettings/ToolSettings_Forward.h"
#include "../include/Tool.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif

#include <iostream>

int In_Pan::move(Application* sender, MouseEvent dat)
{
	// If the array is empty, insert the first element, otherwise overwrite it with the passed MouseEvent
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }
	// Test that the cursor moved a significant amount
	glm::vec3 pos = glm::vec3(dat.x, dat.y, 0.0f);
	glm::vec3 dir = glm::normalize(pos - fragData.anchors.back().pos);
	float length = glm::length(pos - fragData.anchors.back().pos);
	if (length < ROOT2 * 1.2f) { return INPUT_WAIT; }
	// Adjust the anchor points
	fragData.anchors.front().flag = FLAG_NULL;
	// Add the anchor
	if (fragData.anchors.size() >= 3) { fragData.anchors.erase(fragData.anchors.begin()); }
	fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, 0, pos, dir, 1.0f,
		FHANDLE_LINEAR, false, pos,
		FHANDLE_LINEAR, false, pos,
		FHANDLE_REL_INDEPENDENT,
		data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
		data.inputEvents.back().tilty, data.inputEvents.back().rotation,
		(float)data.start.time, sender->getMouseVelocity(), 1.0f));

	return INPUT_ALLOW_PRESS_UPDATECURSOR;
}

int In_Pan::click(Application* sender, MouseEvent dat)
{
	int wasHandled;
	// Warning: This logic branch has no fail-case
	if (dat.action == INPUT_PRESS && dat.button == UI_MOUSE_LEFT)
	{
		// Get the settings
		pan = *owner.get()->getPan();
		// For new inputs, clear the current array, and then set start to the passed MouseEvent
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
		fragData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, 0, pos, glm::vec3(0, 0, 0), 1.0f,
			FHANDLE_LINEAR, false, pos,
			FHANDLE_LINEAR, false, pos,
			FHANDLE_REL_INDEPENDENT,
			data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
			data.inputEvents.back().tilty, data.inputEvents.back().rotation,
			(float)data.start.time, sender->getMouseVelocity(), 1.0f));
		fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, 0, pos, glm::vec3(0, 0, 0), 1.0f,
			FHANDLE_LINEAR, false, pos,
			FHANDLE_LINEAR, false, pos,
			FHANDLE_REL_INDEPENDENT,
			data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
			data.inputEvents.back().tilty, data.inputEvents.back().rotation,
			(float)data.start.time, sender->getMouseVelocity(), 1.0f));
		fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, 0, pos, glm::vec3(0, 0, 0), 1.0f,
			FHANDLE_LINEAR, false, pos,
			FHANDLE_LINEAR, false, pos,
			FHANDLE_REL_INDEPENDENT,
			data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
			data.inputEvents.back().tilty, data.inputEvents.back().rotation,
			(float)data.start.time, sender->getMouseVelocity(), 1.0f));
		wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
		//std::cout << "IN_PAN::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == INPUT_RELEASE && dat.button == UI_MOUSE_LEFT)
	{
		// For Drag, Only interested in the start, last mouseEvent, & end events
		data.end = dat;
		fragData.endTime = (float)data.end.time;
		wasHandled = INPUT_ALLOW_RELEASE;
		std::cout << "IN_PAN::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = INPUT_NOSIGNAL;
		std::cout << "IN_PAN::CLICK::NOSIGNAL" << std::endl;
	}
	std::cout << "IN_PAN::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}