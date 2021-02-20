#include "../include/methods/In_Zoom.h"
#include "../include/methods/InputMethod.h"
#include "../include/ToolSettings.h"
#include "../include/Tool.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif

#include <iostream>

int In_Zoom::move(Application* sender, MouseEvent dat)
{
	// If the array is empty, insert the first element, otherwise overwrite it with the passed MouseEvent
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }
	// Adjust the anchor points
	fragData.anchors.front().flag = FLAG_NULL;
	// Add the anchor
	if (fragData.anchors.size() >= maxBufferLength) { fragData.anchors.erase(fragData.anchors.begin()); }
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	float zoomInterp = (dat.modKey == INPUT_MOD_ALT) ? -1.0f : 1.0f;
	fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, 0, pos, glm::vec3(0, 0, 0), zoomInterp,
		FHANDLE_LINEAR, false, pos,
		FHANDLE_LINEAR, false, pos,
		FHANDLE_REL_INDEPENDENT,
		data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
		data.inputEvents.back().tilty, data.inputEvents.back().rotation,
		(float)data.start.time, sender->getMouseVelocity(), 1.0f));
	// Set to scrub-mode if the distance has shifted far enough
	if ((fragData.anchors.back().pos.y - fragData.transform.origin.y) >= zoom.scrubThreshold) { fragData.anchors.back().flag = FLAG_SCRUB; }
	return INPUT_ALLOW_PRESS;
}

int In_Zoom::click(Application* sender, MouseEvent dat)
{
	int wasHandled;
	// Warning: This logic branch has no fail-case
	if (dat.action == GLFW_PRESS && dat.button == UI_MOUSE_LEFT)
	{
		// Get the settings
		zoom = *owner.get()->getZoom();
		// For new inputs, clear the current array, and then set start to the passed MouseEvent
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
		float zoomInterp = (dat.modKey == INPUT_MOD_ALT) ? -1.0f : 1.0f;
		fragData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, 0, origin, glm::vec3(0, 0, 0), zoomInterp,
			FHANDLE_LINEAR, false, origin,
			FHANDLE_LINEAR, false, origin,
			FHANDLE_REL_INDEPENDENT,
			data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
			data.inputEvents.back().tilty, data.inputEvents.back().rotation,
			(float)data.start.time, sender->getMouseVelocity(), 1.0f));
		wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
		//std::cout << "IN_PAN::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == GLFW_RELEASE && dat.button == UI_MOUSE_LEFT)
	{
		// For Drag, Only interested in the start, last mouseEvent, & end events
		data.end = dat;
		fragData.endTime = (float)data.end.time;
		wasHandled = INPUT_ALLOW_RELEASE;
		//std::cout << "IN_ZOOM::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = INPUT_NOSIGNAL;
		//std::cout << "IN_ZOOM::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "IN_ZOOM::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}