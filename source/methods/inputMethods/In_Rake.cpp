#include "../../include/methods/inputMethods/In_Rake.h"
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
#include <map>
#include <deque>

InputHandlerFlag In_Rake::move(Application* sender, Input dat)
{
	glm::vec3 splinePos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 splineDir = glm::normalize(glm::vec3(
		splinePos.x - splineData.anchors.back().pos.x,
		splinePos.y - splineData.anchors.back().pos.y,
		0.0f));

	bool isNew = (splineData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
	if (controlScheme == TSetType::continuous)
	{
		if (!continuousDraw(sender, dat, &continuous, &smoothing, &splineData, continuous.anchorSpacing, splinePos, splineDir)) 
		{ 
			return InputHandlerFlag::wait;
		}
	}

	splineIDCount++;
	splineData.anchors.push_back(FragmentAnchor(anchorIDCount, splinePos, splineDir,
		1.0f,
		HandleType::linear, false, splinePos,
		HandleType::linear, false, splinePos,
		HandleRel::independent,
		data.inputEvents.back()));

	glm::vec3 fragDir = splineDir * glm::quat(glm::vec3(0, 0, glm::radians(90.f)));
	glm::vec3 fragPos = glm::vec3(0);

	if (rake.rakeShape == TSetProp::linear)
	{
		float lengthIncrement = 0.0f;
		glm::vec3 startPos = glm::vec3(0.0f);
		int pointCount = 0;

		if (rake.pushToCanvasEdge)
		{
			glm::vec2 canvasSize = sender->getCanvasDimensions(); canvasSize.y *= -1;
			glm::vec2 canvasV1 = glm::vec2(0);
			glm::vec2 canvasV2 = glm::vec2(canvasSize.x, 0);
			glm::vec2 canvasV3 = glm::vec2(canvasSize.x, canvasSize.y);
			glm::vec2 canvasV4 = glm::vec2(0, canvasSize.y);
			glm::vec4 canvasTop = glm::vec4(canvasV1, canvasV2);
			glm::vec4 canvasBottom = glm::vec4(canvasV4, canvasV3);
			glm::vec4 canvasLeft = glm::vec4(canvasV1, canvasV4);
			glm::vec4 canvasRight = glm::vec4(canvasV2, canvasV3);
			//
			glm::vec2 fragPointA = glm::vec2(splinePos);
			glm::vec2 fragPointB = glm::vec2(splinePos + (fragDir * 100.0f));
			glm::vec4 fragLine = glm::vec4(fragPointA.x, fragPointA.y,
				fragPointB.x, fragPointB.y);
			//std::cout << vec2VecToString({ fragPointA, fragPointB }) << std::endl;

			// Use a deque as the list
			std::vector<glm::vec2> pointSelector;
			std::vector<float> lenSelector;

			// Set the topIntersect and set as first deque element
			glm::vec2 topIntersect = lineIntersect2D(fragLine, canvasTop);
			float topLen = glm::length(topIntersect - fragPointA);
			pointSelector.push_back(topIntersect);
			lenSelector.push_back(topLen);

			// Set the bottomIntersect and sort into deque
			glm::vec2 bottomIntersect = lineIntersect2D(fragLine, canvasBottom);
			float bottomLen = glm::length(bottomIntersect - fragPointA);
			pointSelector.push_back(bottomIntersect);
			lenSelector.push_back(bottomLen);
		
			// Set the leftIntersect and sort into deque
			glm::vec2 leftIntersect = lineIntersect2D(fragLine, canvasLeft);
			float leftLen = glm::length(leftIntersect - fragPointA);
			pointSelector.push_back(leftIntersect);
			lenSelector.push_back(leftLen);

			// Set the rightIntersect and sort into deque
			glm::vec2 rightIntersect = lineIntersect2D(fragLine, canvasRight);
			float rightLen = glm::length(rightIntersect - fragPointA);
			pointSelector.push_back(rightIntersect);
			lenSelector.push_back(rightLen);

			int indexA = 0, indexB = 0;
			float selectA = (float)INT_MAX, selectB = (float)INT_MAX;
			for (int i = 0; i < 4; i++)
			{
				if (lenSelector[i] < selectA) 
				{ 
					selectB = selectA;
					selectA = lenSelector[i];
					indexB = indexA;
					indexA = i;
				}
				else if (lenSelector[i] < selectB && lenSelector[i] != selectA) 
				{ 
					selectB = lenSelector[i];
					indexB = i;
				}
			}
			glm::vec3 pointA = glm::vec3(pointSelector[indexA], 0.0f);
			glm::vec3 pointB = glm::vec3(pointSelector[indexB], 0.0f);
			//std::cout << indexA << ", " << indexB << std::endl;
			//std::cout << vec2VecToString({ topIntersect, bottomIntersect, leftIntersect, rightIntersect }) << std::endl;
			lengthIncrement = -glm::length(pointA - pointB) / (rake.pointsCount * 2);
			startPos = (pointA.y > pointB.y) ? pointB : pointA;
			
			/*
			for (int i = 0; i < (rake.points * 2) + 3; i++)
			{
				anchorIDCount++;
				fragPos = startPos - (fragDir * (lengthIncrement * i));
				fragData.anchors.push_back(FragmentAnchor(data.start.flagPrimary, anchorIDCount, fragPos, glm::vec3(fragDir.x, fragDir.y * -1, fragDir.z), 1.0f,
					FHANDLE_LINEAR, false, fragPos,
					FHANDLE_LINEAR, false, fragPos,
					FHANDLE_REL_INDEPENDENT,
					data.inputEvents.back().pressure, data.inputEvents.back().tiltX,
					data.inputEvents.back().tiltY, data.inputEvents.back().rotation,
					(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));
			}
			*/

			anchorIDCount++;
			fragData.anchors.push_back(
				//FragmentAnchor(anchorIDCount, pointA, glm::vec3(fragDir.x, fragDir.y * -1, fragDir.z),
				FragmentAnchor(anchorIDCount, pointA, glm::vec3(fragDir.x, fragDir.y, fragDir.z),
					1.0f,
					HandleType::linear, false, pointA,
					HandleType::linear, false, pointA,
					HandleRel::independent,
					data.inputEvents.back()));

			anchorIDCount++;
			fragData.anchors.push_back(
				//FragmentAnchor(anchorIDCount, pointB, glm::vec3(fragDir.x, fragDir.y * -1, fragDir.z),
				FragmentAnchor(anchorIDCount, pointB, glm::vec3(fragDir.x, fragDir.y, fragDir.z),
					1.0f,
					HandleType::linear, false, pointB,
					HandleType::linear, false, pointB,
					HandleRel::independent,
					data.inputEvents.back()));
			fragData.anchors.at(fragData.anchors.size() - 2).input.flagSecondary = InputFlag::point;
		}
		else
		{
			lengthIncrement = rake.size / (rake.pointsCount * 2);
			startPos = splinePos + (fragDir * (rake.size / 2.0f));
			for (int i = 0; i < (rake.pointsCount * 2) + 3; i++)
			{
				anchorIDCount++;
				fragPos = startPos - (fragDir * (lengthIncrement * i));
				fragData.anchors.push_back(
					//FragmentAnchor(anchorIDCount, fragPos, glm::vec3(fragDir.x, fragDir.y * -1, fragDir.z),
					FragmentAnchor(anchorIDCount, fragPos, glm::vec3(fragDir.x, fragDir.y, fragDir.z),
						1.0f,
						HandleType::linear, false, fragPos,
						HandleType::linear, false, fragPos,
						HandleRel::independent,
						data.inputEvents.back()));
			}
			fragData.anchors.at(fragData.anchors.size() - size_t((rake.pointsCount * 2) + 3)).input.flagSecondary = InputFlag::point;
		}
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
InputHandlerFlag In_Rake::click(Application* sender, Input dat)
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
					1.0f,
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
				owner.get()->getContinuousControl()->updateTrueSpacing(owner, sender->getCanvasDimensions().x, sender->getCanvasDimensions().y);
				continuous = *owner.get()->getContinuousControl();
				continuous.clearConstraint();
			}
			rake = *owner.get()->getRake();
			smoothing = *owner.get()->getSmoothing();
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
					1.0f,
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

InputHandlerFlag In_Rake::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	return InputHandlerFlag::noSignal;
}

void In_Rake::addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{

}

void In_Rake::generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{

}

void In_Rake::generateCurve()
{

}

void In_Rake::newInput(Application* sender, Input dat)
{

}