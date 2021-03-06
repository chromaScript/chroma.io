#include "../include/methods/In_ShapeDraw.h"
#include "../include/methods/InputMethod.h"
#include "../include/ToolSettings.h"
#include "../include/Tool.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/rotate_vector.hpp>
#include <gtc/quaternion.hpp>

#include <iostream>
#include <algorithm>
#include <iostream>

int In_ShapeDraw::move(Application* sender, MouseEvent dat)
{
	glm::vec3 splinePos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 splineDir = glm::normalize(glm::vec3(
		splinePos.x - splineData.anchors.back().pos.x,
		splineData.anchors.back().pos.y - splinePos.y,
		0.0f));

	bool isNew = (splineData.anchors.front().flag == FLAG_NEW_INPUT) ? true : false;
	if (controlScheme == TSetType::continuous)
	{
		if (!continuousMove(sender, dat, &continuous, nullptr, true, splinePos, splineDir)) { return INPUT_WAIT; }
	}

	splineIDCount++;
	splineData.anchors.push_back(FragmentAnchor(data.start.flag, splineIDCount, splinePos, splineDir, 1.0f,
		FHANDLE_LINEAR, false, splinePos,
		FHANDLE_LINEAR, false, splinePos,
		FHANDLE_REL_INDEPENDENT,
		data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
		data.inputEvents.back().tilty, data.inputEvents.back().rotation,
		(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));

	// Verticies are counted clockwise from the upper-left corner (origin-point)
	if (polygon.sides == 4)
	{
		float halfSize = polygon.size / 2.0f;
		glm::vec3 shapeXYDir = glm::quat(glm::vec3(0, 0, glm::radians(45.0f))) * splineDir;
		glm::vec3 shapeYXDir = glm::quat(glm::vec3(0, 0, glm::radians(-45.0f))) * splineDir;
		// Set the shape verticies based on bounding box
		std::vector<glm::vec3> verts = {
			splinePos + (shapeXYDir * halfSize),
			splinePos + (shapeYXDir * halfSize),
			splinePos - (shapeXYDir * halfSize),
			splinePos - (shapeYXDir * halfSize),
			splinePos + (shapeXYDir * halfSize)
		};

		std::vector<glm::vec3> dirs = {
			glm::normalize(glm::vec3(
				verts[1].x - verts[0].x,
				verts[0].y - verts[1].y,
				0.0f)),
			glm::normalize(glm::vec3(
				verts[2].x - verts[1].x,
				verts[1].y - verts[2].y,
				0.0f)),
			glm::normalize(glm::vec3(
				verts[3].x - verts[1].x,
				verts[1].y - verts[3].y,
				0.0f)),
			glm::normalize(glm::vec3(
				verts[0].x - verts[3].x,
				verts[3].y - verts[0].y,
				0.0f)),
			glm::normalize(glm::vec3(
				verts[1].x - verts[0].x,
				verts[0].y - verts[1].y,
				0.0f)),
		};

		for (int i = 0; i < verts.size(); i++)
		{
			anchorIDCount++;
			fragData.anchors.push_back(FragmentAnchor(data.start.flag, anchorIDCount, verts[i], dirs[i], 1.0f,
				FHANDLE_LINEAR, false, verts[i],
				FHANDLE_LINEAR, false, verts[i],
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));
		}
		
	}

	if (isNew)
	{
		splineData.anchors.front().flag = FLAG_NEW_INPUT;
		fragData.anchors.front().flag = FLAG_NEW_INPUT;
	}
	else if (fragData.anchors.front().flag == FLAG_NEW_INPUT)
	{
		fragData.anchors.front().flag = FLAG_NULL;
	}

	return INPUT_ALLOW_PRESS_UPDATECURSOR;
}
int In_ShapeDraw::click(Application* sender, MouseEvent dat)
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
			fragData.endTime = splineData.endTime = 0.0;
			// Push back a new data point
			data.inputEvents.push_back(dat);
			// Determine the anchor position and direction
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			glm::vec3 dir = glm::normalize(glm::vec3(
				splineData.anchors.back().pos.x - pos.x,
				splineData.anchors.back().pos.y - pos.y,
				0.0f));
			// Insert new anchor, set dirInterpFactor to 0.0f so that the stroke will hard-snap
			// the shards to the new direction rather than smoothly interpolate the change
			// Note: Must revisit this section later to inject adjustment of pressure via input settings. Interpolate along the pressure graph
			splineIDCount++;
			splineData.anchors.push_back(FragmentAnchor(FLAG_NULL, splineIDCount, pos, dir, 0.0f,
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
				continuous.anchorSpacing = 30.0f;
			}
			polygon = *owner.get()->getPolygon();
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
			splineData.transform.origin = origin;
			// Add the first anchor point, there is no direction yet, so a manual wait will be placed on the
			// anchor to count off 1 more anchor before rendering.
			anchorIDCount = 0;
			splineIDCount = 0;
			splineData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, splineIDCount, origin, glm::vec3(0, 0, 0), 1.0f,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
			splineData.anchors.back().wait = 1;
		}
		wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
		//std::cout << "IN_VORTEX::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == INPUT_RELEASE && dat.button == UI_MOUSE_LEFT)
	{
		data.end = dat;
		if (splineData.anchors.size() == 1) { splineData.anchors.back().flag = FLAG_NULL; }
		fragData.endTime = splineData.endTime = (float)data.end.time;
		wasHandled = INPUT_ALLOW_RELEASE;
		//std::cout << "IN_VORTEX::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = INPUT_NOSIGNAL;
		//std::cout << "IN_VORTEX::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "IN_VORTEX::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}