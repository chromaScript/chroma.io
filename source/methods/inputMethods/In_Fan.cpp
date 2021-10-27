#include "../../include/methods/inputMethods/In_Fan.h"
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

InputHandlerFlag In_Fan::move(Application* sender, Input dat)
{
	glm::vec3 splinePos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 splineDir = glm::normalize(glm::vec3(
		splinePos.x - splineData.anchors.back().pos.x,
		splinePos.y - splineData.anchors.back().pos.y,
		0.0f));

	bool isNew = (splineData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
	if (controlScheme == TSetType::continuous)
	{
		if (!continuousMove(sender, dat, &continuous, &smoothing, &splineData, continuous.anchorSpacing, splinePos, splineDir)) { return InputHandlerFlag::wait; }
	}
	
	if (!fan.stopWait)
	{
		std::cout << glm::length(splinePos - splineData.transform.origin) << std::endl;
		if (glm::length(splinePos - splineData.transform.origin) < fan.waitAmount) { return InputHandlerFlag::wait; }
		else { fan.stopWait = true; }
	}
	
	splineIDCount++;
	splineData.anchors.push_back(FragmentAnchor(anchorIDCount, splinePos, splineDir,
		1.0f,
		HandleType::linear, false, splinePos,
		HandleType::linear, false, splinePos,
		HandleRel::independent,
		dat));
	splineData.anchors.back().input.flagPrimary = InputFlag::null;

	glm::vec3 fragDir = glm::normalize(glm::vec3(
		splinePos.x - fragData.transform.origin.x,
		splinePos.y - fragData.transform.origin.y,
		0.0f));

	anchorIDCount++;
	fragData.anchors.push_back(FragmentAnchor(anchorIDCount, splinePos, fragDir,
		1.0f,
		HandleType::linear, false, splinePos,
		HandleType::linear, false, splinePos,
		HandleRel::independent,
		dat));
	fragData.anchors.back().input.flagPrimary = (fan.airbrushMode) ? InputFlag::connectOriginSwapClearMetadata : InputFlag::connectOrigin;

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
InputHandlerFlag In_Fan::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
	{
		// Clear any constraint variables, these should not remain constant between different
		// press events of the same stroke
		continuous.clearConstraint();
		// Scenario in which 'Shift' is used to connect the end of the anchors to the next one
		// Keep previous anchor data and simply continue adding to it
		if (dat.modKey == InputModKey::shift && fragData.anchors.size() != 0)
		{
			// Clear the wait amount as override
			fan.stopWait = true;
			// Reset the end-input && end-time
			data.end.resetAll();
			fragData.endTime = 0.0;
			// Push back a new data point
			data.inputEvents.push_back(dat);
			// Determine the anchor position and direction
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			
			int lastSplineIndex = (int)splineData.anchors.size() - 1;
			glm::vec3 connectPoint = splineData.anchors.back().pos;
			glm::vec3 connectLineDir = glm::normalize(glm::vec3(
				pos.x - connectPoint.x,
				pos.y - connectPoint.y,
				0.0f));
			glm::vec3 splinePos = glm::vec3(0), splineDir = glm::vec3(0);
			float splinePressure = data.inputEvents.back().pressure;
			float splineTiltx = data.inputEvents.back().tiltX;
			float splineTilty = data.inputEvents.back().tiltY;
			float splineRotation = data.inputEvents.back().rotation;
			float splineVelocity = sender->getMouseVelocity(0.4f);
			float distance = glm::length(pos - splineData.anchors.back().pos);
			float usedDistance = continuous.anchorSpacing;
			while (usedDistance < distance)
			{
				float splinePressure = lerpf(data.inputEvents.back().pressure, splineData.anchors.back().input.pressure, usedDistance);
				float splineTiltx = lerpf(data.inputEvents.back().tiltX, splineData.anchors.back().input.tiltX, usedDistance);
				float splineTilty = lerpf(data.inputEvents.back().tiltY, splineData.anchors.back().input.tiltY, usedDistance);
				float splineRotation = lerpf(data.inputEvents.back().rotation, splineData.anchors.back().input.rotation, usedDistance);
				float splineVelocity = lerpf(sender->getMouseVelocity(0.4f), splineData.anchors.back().input.velocity, usedDistance);
				splinePos = connectPoint + (connectLineDir * -usedDistance);
				splineDir = glm::normalize(glm::vec3(
					splinePos.x - fragData.transform.origin.x,
					splinePos.y - fragData.transform.origin.y,
					0.0f));
				usedDistance += continuous.anchorSpacing;
				splineIDCount++;
				splineData.anchors.push_back(FragmentAnchor(anchorIDCount, splinePos, splineDir,
					1.0f,
					HandleType::linear, false, splinePos,
					HandleType::linear, false, splinePos,
					HandleRel::independent,
					Input(splinePressure, splineTiltx,
						splineTilty, splineRotation, splineVelocity)));
			}
			// Insert new anchor, set dirInterpFactor to 0.0f so that the stroke will hard-snap
			// the shards to the new direction rather than smoothly interpolate the change
			// Note: Must revisit this section later to inject adjustment of pressure via input settings. Interpolate along the pressure graph

			glm::vec3 dir = glm::normalize(glm::vec3(
				pos.x - fragData.transform.origin.x,
				pos.y - fragData.transform.origin.y,
				0.0f));
			splineIDCount++;
			splineData.anchors.push_back(FragmentAnchor(anchorIDCount, pos, dir,
				1.0f,
				HandleType::linear, false, pos,
				HandleType::linear, false, pos,
				HandleRel::independent,
				splineData.anchors.back().input));

			for (int i = lastSplineIndex; i < splineData.anchors.size(); i++)
			{
				FragmentAnchor* spline = &splineData.anchors[i];
				glm::vec3 fragPos = spline->pos;
				glm::vec3 fragDir = glm::normalize(glm::vec3(
					fragPos.x - fragData.transform.origin.x,
					fragPos.y - fragData.transform.origin.y,
					0.0f));
				anchorIDCount++;
				fragData.anchors.push_back(FragmentAnchor(anchorIDCount, fragPos, fragDir,
					0.0f,
					HandleType::linear, false, pos,
					HandleType::linear, false, pos,
					HandleRel::independent,
					spline->input));
				fragData.anchors.back().input.flagPrimary = (fan.airbrushMode) ? InputFlag::connectOriginSwapClearMetadata : InputFlag::connectOrigin;
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
			splineData.anchors.push_back(FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				0.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				data.inputEvents.back()));
			splineData.anchors.back().input.flagPrimary = InputFlag::newInput;
			// Place the first anchor as the origin-point
			anchorIDCount++;
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				1.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				data.inputEvents.back()));
			fragData.anchors.back().input.flagPrimary = InputFlag::originPoint;
		}
		wasHandled = InputHandlerFlag::allowPress_updateCursor;
		//std::cout << "MethodType::in_draw::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		data.end = dat;
		if (fragData.anchors.size() == 1) { fragData.anchors.back().input.flagPrimary = InputFlag::null; }
		fragData.endTime = (float)data.end.time;
		wasHandled = InputHandlerFlag::release;
		//std::cout << "MethodType::in_draw::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
		//std::cout << "MethodType::in_draw::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "MethodType::in_draw::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}