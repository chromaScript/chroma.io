#include "../../include/methods/inputMethods/In_ShapeDraw.h"
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
#include <gtc/quaternion.hpp>

#include <iostream>
#include <algorithm>
#include <iostream>

InputHandlerFlag In_ShapeDraw::move(Application* sender, Input dat)
{
	glm::vec3 splinePos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 splineDir = glm::normalize(glm::vec3(
		splinePos.x - splineData.anchors.back().pos.x,
		splinePos.y - splineData.anchors.back().pos.y,
		0.0f));

	//bool isNew = (splineData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
	if (controlScheme == TSetType::continuous)
	{
		if (!continuousDraw(sender, dat, &continuous, nullptr, &splineData, continuous.anchorSpacing, splinePos, splineDir)) { return InputHandlerFlag::wait; }
	}
	bool isNew = (splineData.anchors.front().input.flagPrimary == InputFlag::newInput);
	splineIDCount++;
	splineData.anchors.push_back(
		FragmentAnchor(anchorIDCount, splinePos, splineDir,
			1.0f,
			HandleType::linear, false, splinePos,
			HandleType::linear, false, splinePos,
			HandleRel::independent,
			data.inputEvents.back()));
	splineData.anchors.back().input.flagPrimary = InputFlag::null;
	splineData.anchors.back().input.flagSecondary = InputFlag::null;

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
				verts[1].y - verts[0].y,
				0.0f)),
			glm::normalize(glm::vec3(
				verts[2].x - verts[1].x,
				verts[2].y - verts[1].y,
				0.0f)),
			glm::normalize(glm::vec3(
				verts[3].x - verts[1].x,
				verts[3].y - verts[1].y,
				0.0f)),
			glm::normalize(glm::vec3(
				verts[0].x - verts[3].x,
				verts[0].y - verts[3].y,
				0.0f)),
			glm::normalize(glm::vec3(
				verts[1].x - verts[0].x,
				verts[1].y - verts[0].y,
				0.0f)),
		};

		for (int i = 0; i < verts.size(); i++)
		{
			anchorIDCount++;
			fragData.anchors.push_back(
				FragmentAnchor(anchorIDCount, verts[i], dirs[i],
					1.0f,
					HandleType::linear, false, verts[i],
					HandleType::linear, false, verts[i],
					HandleRel::independent,
					data.inputEvents.back()));
			fragData.anchors.back().input.flagPrimary = InputFlag::null;
			fragData.anchors.back().input.flagSecondary = InputFlag::null;
		}
		fragData.anchors.at(fragData.anchors.size() - verts.size()).input.flagSecondary = InputFlag::point;
	}

	if (isNew)
	{
		splineData.anchors.front().input.flagPrimary = InputFlag::null;
		fragData.anchors.front().input.flagPrimary = InputFlag::newInput;
	}
	else if (fragData.anchors.front().input.flagPrimary == InputFlag::newInput)
	{
		fragData.anchors.front().input.flagPrimary = InputFlag::null;
	}

	return InputHandlerFlag::allowPress_updateCursor;
}
InputHandlerFlag In_ShapeDraw::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
	{
		// Clear any constraint variables, these should not remain constant between different
		// press events of the same stroke
		//constraintDirty = false;
		//constraintAngle = AXIS_EMPTY;
		//constOriginX = fragData.transform.pos;
		//constOriginY = fragData.transform.pos;
		// Scenario in which 'Shift' is used to connect the end of the anchors to the next one
		// Keep previous anchor data and simply continue adding to it
		if (dat.modKey == InputModKey::shift && fragData.anchors.size() != 0)
		{
			// Reset the end-input && end-time
			data.end.resetAll();
			fragData.endTime = splineData.endTime = 0.0;
			// Push back a new data point
			data.inputEvents.push_back(dat);
			// Determine the anchor position and direction
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			glm::vec3 dir = glm::normalize(glm::vec3(
				pos.x - splineData.anchors.back().pos.x,
				pos.y - splineData.anchors.back().pos.y,
				0.0f));
			// Insert new anchor, set dirInterpFactor to 0.0f so that the stroke will hard-snap
			// the shards to the new direction rather than smoothly interpolate the change
			// Note: Must revisit this section later to inject adjustment of pressure via input settings. Interpolate along the pressure graph
			splineIDCount++;
			splineData.anchors.push_back(
				FragmentAnchor(anchorIDCount, pos, dir,
					0.0f,
					HandleType::linear, false, pos,
					HandleType::linear, false, pos,
					HandleRel::independent,
					data.inputEvents.back()));
			splineData.anchors.back().input.flagPrimary = InputFlag::null;
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
			splineData.anchors.push_back(
				FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
					0.0f,
					HandleType::linear, false, origin,
					HandleType::linear, false, origin,
					HandleRel::independent,
					data.inputEvents.back()));
			splineData.anchors.back().input.flagPrimary = InputFlag::newInput;
			splineData.anchors.back().wait = 1;
		}
		wasHandled = InputHandlerFlag::allowPress_updateCursor;
		//std::cout << "MethodType::in_vortex::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		data.end = dat;
		if (splineData.anchors.size() == 1) { splineData.anchors.back().input.flagPrimary = InputFlag::null; }
		fragData.endTime = splineData.endTime = (float)data.end.time;
		wasHandled = InputHandlerFlag::release;
		//std::cout << "MethodType::in_vortex::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
		//std::cout << "MethodType::in_vortex::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "MethodType::in_vortex::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}

InputHandlerFlag In_ShapeDraw::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	return InputHandlerFlag::noSignal;
}

void In_ShapeDraw::addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{

}

void In_ShapeDraw::generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{

}

void In_ShapeDraw::generateCurve()
{

}

void In_ShapeDraw::newInput(Application* sender, Input dat)
{

}