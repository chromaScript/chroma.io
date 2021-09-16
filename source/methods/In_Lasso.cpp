#include "../include/methods/In_Lasso.h"
#include "../include/methods/InputMethod.h"
#include "../include/ToolSettings.h"
#include "../include/toolSettings/ToolSettings_Forward.h"
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

int In_Lasso::move(Application* sender, MouseEvent dat)
{
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 dir = glm::normalize(glm::vec3(
		pos.x - fragData.anchors.back().pos.x,
		fragData.anchors.back().pos.y - pos.y,
		0.0f));

	bool isNew = (fragData.anchors.front().flag == FLAG_NEW_INPUT) ? true : false;
	if (controlScheme == TSetType::continuous)
	{
		if (!continuousMove(sender, dat, &continuous, nullptr, false, continuous.anchorSpacing, pos, dir)) { return INPUT_WAIT; }
	}

	anchorIDCount++;
	fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, anchorIDCount, pos, dir, 1.0f,
		FHANDLE_LINEAR, false, pos,
		FHANDLE_LINEAR, false, pos,
		FHANDLE_REL_INDEPENDENT,
		data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
		data.inputEvents.back().tilty, data.inputEvents.back().rotation,
		(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));

	if (isNew)
	{
		fragData.anchors.front().flag = FLAG_NULL;
	}

	return INPUT_ALLOW_PRESS_UPDATECURSOR;
}
int In_Lasso::click(Application* sender, MouseEvent dat)
{
	int wasHandled;
	if (dat.action == INPUT_PRESS && dat.button == UI_MOUSE_LEFT)
	{
		// Clear any constraint variables, these should not remain constant between different
		// press events of the same stroke
		//constraintDirty = false;
		//constraintAngle = AXIS_EMPTY;
		//constOriginX = fragData.transform.pos;
		//constOriginY = fragData.transform.pos;
		// Scenario in which 'Shift' is used to connect the end of the anchors to the next one
		// Keep previous anchor data and simply continue adding to it
		if (dat.modKey == INPUT_MOD_SHIFT)
		{
			// Reset the end-input && end-time
			data.end.reset();
			fragData.endTime = 0.0;
			// Push back a new data point
			data.inputEvents.push_back(dat);
			// Determine the anchor position and direction
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			glm::vec3 dir = glm::normalize(glm::vec3(
				fragData.anchors.back().pos.x - pos.x,
				fragData.anchors.back().pos.y - pos.y,
				0.0f));
			// Insert new anchor, set dirInterpFactor to 0.0f so that the stroke will hard-snap
			// the shards to the new direction rather than smoothly interpolate the change
			// Note: Must revisit this section later to inject adjustment of pressure via input settings. Interpolate along the pressure graph
			anchorIDCount++;
			fragData.anchors.push_back(FragmentAnchor(data.start.flag, anchorIDCount, pos, dir, 0.0f,
				FHANDLE_LINEAR, false, pos,
				FHANDLE_LINEAR, false, pos,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));
		}
		// A new set of anchors is started
		else
		{
			// Get new copies of the tool settings
			// Once a new input has begun, the order has been 'sent' and whatever
			// settings were in place at that time is what the output will
			// use to render the input. After this point, the verticies and tool settings
			// for the output fragment may only be changed using vertex/anchor manipulation tools
			switch (controlScheme)
			{
			case TSetType::continuous:
			default:
				continuous = *owner.get()->getContinuousControl();
			}
			// Reset the data
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
			fragData.transform.origin = origin;
			// Add the first anchor point, there is no direction yet, so a manual wait will be placed on the
			// anchor to count off 1 more anchor before rendering.
			anchorIDCount = 0;
			fragData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, anchorIDCount, origin, glm::vec3(0, 0, 0), 1.0f,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
			fragData.anchors.back().wait = 1;
		}
		wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
		//std::cout << "IN_DRAW::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == INPUT_RELEASE && dat.button == UI_MOUSE_LEFT)
	{
		data.end = dat;
		if (fragData.anchors.size() == 1) { fragData.anchors.back().flag = FLAG_NULL; }
		if (fragData.connectEnds)
		{
			fragData.anchors.push_back(fragData.anchors.front());
		}
		fragData.endTime = (float)data.end.time;
		wasHandled = INPUT_ALLOW_RELEASE;
		//std::cout << "IN_DRAW::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = INPUT_NOSIGNAL;
		//std::cout << "IN_DRAW::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "IN_DRAW::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}