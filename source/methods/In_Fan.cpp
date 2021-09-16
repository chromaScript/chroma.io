#include "../include/methods/In_Fan.h"
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

int In_Fan::move(Application* sender, MouseEvent dat)
{
	glm::vec3 splinePos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 splineDir = glm::normalize(glm::vec3(
		splinePos.x - splineData.anchors.back().pos.x,
		splineData.anchors.back().pos.y - splinePos.y,
		0.0f));

	bool isNew = (splineData.anchors.front().flag == FLAG_NEW_INPUT) ? true : false;
	if (controlScheme == TSetType::continuous)
	{
		if (!continuousMove(sender, dat, &continuous, &smoothing, true, continuous.anchorSpacing, splinePos, splineDir)) { return INPUT_WAIT; }
	}
	
	if (!fan.stopWait)
	{
		std::cout << glm::length(splinePos - splineData.transform.origin) << std::endl;
		if (glm::length(splinePos - splineData.transform.origin) < fan.waitAmount) { return INPUT_WAIT; }
		else { fan.stopWait = true; }
	}
	
	splineIDCount++;
	splineData.anchors.push_back(FragmentAnchor(FLAG_NULL, splineIDCount, splinePos, splineDir, 1.0f,
		FHANDLE_LINEAR, false, splinePos,
		FHANDLE_LINEAR, false, splinePos,
		FHANDLE_REL_INDEPENDENT,
		data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
		data.inputEvents.back().tilty, data.inputEvents.back().rotation,
		(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));

	glm::vec3 fragDir = glm::normalize(glm::vec3(
		splinePos.x - fragData.transform.origin.x,
		fragData.transform.origin.y - splinePos.y,
		0.0f));

	anchorIDCount++;
	fragData.anchors.push_back(FragmentAnchor((fan.airbrushMode) ? FLAG_CONORIGIN_SWAPCLEARMETA : FLAG_CONORIGIN, anchorIDCount, splinePos, fragDir, 1.0f,
		FHANDLE_LINEAR, false, splinePos,
		FHANDLE_LINEAR, false, splinePos,
		FHANDLE_REL_INDEPENDENT,
		data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
		data.inputEvents.back().tilty, data.inputEvents.back().rotation,
		(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));

	if (isNew)
	{
		splineData.anchors.front().flag = FLAG_NULL;
		fragData.anchors.front().flag = FLAG_NEW_INPUT;
	}
	else if (fragData.anchors.front().flag == FLAG_NEW_INPUT)
	{
		fragData.anchors.front().flag = FLAG_NULL;
	}
	return INPUT_ALLOW_PRESS_UPDATECURSOR;
}
int In_Fan::click(Application* sender, MouseEvent dat)
{
	int wasHandled;
	if (dat.action == INPUT_PRESS && dat.button == UI_MOUSE_LEFT)
	{
		// Clear any constraint variables, these should not remain constant between different
		// press events of the same stroke
		continuous.clearConstraint();
		// Scenario in which 'Shift' is used to connect the end of the anchors to the next one
		// Keep previous anchor data and simply continue adding to it
		if (dat.modKey == INPUT_MOD_SHIFT)
		{
			// Clear the wait amount as override
			fan.stopWait = true;
			// Reset the end-input && end-time
			data.end.reset();
			fragData.endTime = 0.0;
			// Push back a new data point
			data.inputEvents.push_back(dat);
			// Determine the anchor position and direction
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			
			int lastSplineIndex = splineData.anchors.size() - 1;
			if (continuous.fillOnShiftClick)
			{
				glm::vec3 connectPoint = splineData.anchors.back().pos;
				glm::vec3 connectLineDir = glm::normalize(glm::vec3(
					connectPoint.x - pos.x,
					connectPoint.y - pos.y,
					0.0f));
				glm::vec3 splinePos = glm::vec3(0), splineDir = glm::vec3(0);
				float splinePressure = lerpf(data.inputEvents.back().pressure, splineData.anchors.back().pressure, continuous.connectPropertiesWeighting);
				float splineTiltx = lerpf(data.inputEvents.back().tiltx, splineData.anchors.back().tiltx, continuous.connectPropertiesWeighting);
				float splineTilty = lerpf(data.inputEvents.back().tilty, splineData.anchors.back().tilty, continuous.connectPropertiesWeighting);
				float splineRotation = lerpf(data.inputEvents.back().rotation, splineData.anchors.back().rotation, continuous.connectPropertiesWeighting);
				float splineVelocity = lerpf(sender->getMouseVelocity(), splineData.anchors.back().velocity, continuous.connectPropertiesWeighting);
				float distance = glm::length(pos - splineData.anchors.back().pos);
				float usedDistance = continuous.anchorSpacing;
				while (usedDistance < distance)
				{
					splinePos = connectPoint + (connectLineDir * -usedDistance);
					splineDir = glm::normalize(glm::vec3(
						fragData.transform.origin.x - splinePos.x,
						splinePos.y - fragData.transform.origin.y,
						0.0f));
					usedDistance += continuous.anchorSpacing;
					splineIDCount++;
					splineData.anchors.push_back(FragmentAnchor(FLAG_NULL, splineIDCount, splinePos, splineDir, 1.0f,
						FHANDLE_LINEAR, false, splinePos,
						FHANDLE_LINEAR, false, splinePos,
						FHANDLE_REL_INDEPENDENT,
						splinePressure, splineTiltx,
						splineTilty, splineRotation,
						(float)data.inputEvents.back().time, splineVelocity, 1.0f));
				}
			}
			// Insert new anchor, set dirInterpFactor to 0.0f so that the stroke will hard-snap
			// the shards to the new direction rather than smoothly interpolate the change
			// Note: Must revisit this section later to inject adjustment of pressure via input settings. Interpolate along the pressure graph

			glm::vec3 dir = glm::normalize(glm::vec3(
				fragData.transform.origin.x - pos.x,
				pos.y - fragData.transform.origin.y,
				0.0f));
			splineIDCount++;
			splineData.anchors.push_back(FragmentAnchor(FLAG_NULL, splineIDCount, pos, dir, 1.0f,
				FHANDLE_LINEAR, false, pos,
				FHANDLE_LINEAR, false, pos,
				FHANDLE_REL_INDEPENDENT,
				splineData.anchors.back().pressure, splineData.anchors.back().tiltx,
				splineData.anchors.back().tilty, splineData.anchors.back().rotation,
				(float)data.inputEvents.back().time, splineData.anchors.back().velocity, 1.0f));

			for (int i = lastSplineIndex; i < splineData.anchors.size(); i++)
			{
				FragmentAnchor* spline = &splineData.anchors[i];
				glm::vec3 fragPos = spline->pos;
				glm::vec3 fragDir = glm::normalize(glm::vec3(
					fragData.transform.origin.x - fragPos.x,
					fragPos.y - fragData.transform.origin.y,
					0.0f));
				anchorIDCount++;
				fragData.anchors.push_back(FragmentAnchor((fan.airbrushMode) ? FLAG_CONORIGIN_SWAPCLEARMETA : FLAG_CONORIGIN, anchorIDCount, fragPos, fragDir, 0.0f,
					FHANDLE_LINEAR, false, fragPos,
					FHANDLE_LINEAR, false, fragPos,
					FHANDLE_REL_INDEPENDENT,
					spline->pressure, spline->tiltx,
					spline->tilty, spline->rotation,
					(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));
			}
			
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
			smoothing = *owner.get()->getSmoothing();
			fan = *owner.get()->getFan();
			// Reset the data
			data.reset();
			data.start = dat;
			data.inputModKey = dat.modKey;
			// Push back a new data point
			data.inputEvents.push_back(dat);
			// Reset the fragData
			fragData.reset(); splineData.reset();
			fragData.startTime = splineData.startTime = (float)data.start.time;
			// Set the upper left corner for the entity 
			glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
			fragData.transform.origin = splineData.transform.origin = origin;
			// Add the first anchor point, there is no direction yet, so a manual wait will be placed on the
			// anchor to count off 1 more anchor before rendering.
			anchorIDCount = 0;
			splineIDCount = 0;
			splineData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, anchorIDCount, origin, glm::vec3(0, 0, 0), 1.0f,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
			// Place the first anchor as the origin-point
			anchorIDCount++;
			fragData.anchors.push_back(FragmentAnchor(FLAG_ORIGIN_POINT, anchorIDCount, origin, glm::vec3(0, 0, 0), 1.0f,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
				

		}
		wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
		//std::cout << "IN_DRAW::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == INPUT_RELEASE && dat.button == UI_MOUSE_LEFT)
	{
		data.end = dat;
		if (fragData.anchors.size() == 1) { fragData.anchors.back().flag = FLAG_NULL; }
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