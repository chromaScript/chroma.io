#include "../include/methods/In_Sampler.h"
#include "../include/methods/InputMethod.h"
#include "../include/ToolSettings.h"
#include "../include/Tool.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>

#include <iostream>
#include <algorithm>
#include <iostream>

int In_Sampler::move(Application* sender, MouseEvent dat)
{
	// If the array is empty, insert the first element, otherwise overwrite it with the passed MouseEvent
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }
	// Adjust the anchor points
	fragData.anchors.front().flag = FLAG_NULL;
	// Update the Anchor
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 screenPos = glm::vec3((float)dat.x, (float)dat.y, 0.0f);
	// Only update the anchors if there was a significant movement
	if (glm::length(fragData.anchors.back().pos - screenPos) > ROOT2 * 1.2f)
	{
		fragData.anchors = {
		FragmentAnchor(FLAG_NULL, anchorIDCount, pos, glm::vec3(0, 0, 0), 1.0f,
			FHANDLE_LINEAR, false, pos,
			FHANDLE_LINEAR, false, pos,
			FHANDLE_REL_INDEPENDENT,
			data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
			data.inputEvents.back().tilty, data.inputEvents.back().rotation,
			(float)data.start.time, sender->getMouseVelocity(), 1.0f),
		FragmentAnchor(FLAG_NULL, anchorIDCount, screenPos, glm::vec3(0, 0, 0), 1.0f,
				FHANDLE_LINEAR, false, screenPos,
				FHANDLE_LINEAR, false, screenPos,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f) };
		anchorIDCount++;
		return INPUT_ALLOW_PRESS_UPDATECURSOR;
	}
	else
	{
		return INPUT_WAIT;
	}
}

int In_Sampler::click(Application* sender, MouseEvent dat)
{
	int wasHandled;
	MouseEvent inClick = dat;
	// Warning: This logic branch has no fail-case
	if (inClick.action == INPUT_PRESS && dat.button == UI_MOUSE_LEFT)
	{
		// Get the settings
		sampler = *owner.get()->getSampler();
		// For new inputs, clear the current array, and then set start to the passed MouseEvent
		anchorIDCount = 0;
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
		// Add the first and second anchor points
		// The second anchor point is used to hold the raw screen-coordinates
		fragData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, anchorIDCount, origin, glm::vec3(0, 0, 0), 1.0f,
			FHANDLE_LINEAR, false, origin,
			FHANDLE_LINEAR, false, origin,
			FHANDLE_REL_INDEPENDENT,
			data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
			data.inputEvents.back().tilty, data.inputEvents.back().rotation,
			(float)data.start.time, sender->getMouseVelocity(), 1.0f));
		glm::vec3 screenPos = glm::vec3((float)dat.x, (float)dat.y, 0.0f);
		fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, anchorIDCount, screenPos, glm::vec3(0, 0, 0), 1.0f,
			FHANDLE_LINEAR, false, screenPos,
			FHANDLE_LINEAR, false, screenPos,
			FHANDLE_REL_INDEPENDENT,
			data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
			data.inputEvents.back().tilty, data.inputEvents.back().rotation,
			(float)data.start.time, sender->getMouseVelocity(), 1.0f));
		anchorIDCount++;
		wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
		//std::cout << "IN_POINT::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (inClick.action == INPUT_RELEASE && dat.button == UI_MOUSE_LEFT)
	{
		// For Drag, Only interested in the start, last mouseEvent, & end events
		data.end = dat;
		fragData.endTime = (float)data.end.time;
		wasHandled = INPUT_ALLOW_RELEASE;
		//std::cout << "IN_POINT::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = INPUT_NOSIGNAL;
		//std::cout << "IN_POINT::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "IN_POINT::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}