#include "../include/methods/In_Rake.h"
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
#include <map>
#include <deque>

int In_Rake::move(Application* sender, MouseEvent dat)
{
	glm::vec3 splinePos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 splineDir = glm::normalize(glm::vec3(
		splinePos.x - splineData.anchors.back().pos.x,
		splinePos.y - splineData.anchors.back().pos.y,
		0.0f));

	bool isNew = (splineData.anchors.front().flag == FLAG_NEW_INPUT) ? true : false;
	if (controlScheme == TSetType::continuous)
	{
		if (!continuousMove(sender, dat, &continuous, &smoothing, true, splinePos, splineDir)) { return INPUT_WAIT; }
	}

	splineIDCount++;
	splineData.anchors.push_back(FragmentAnchor(data.start.flag, splineIDCount, splinePos, splineDir, 1.0f,
		FHANDLE_LINEAR, false, splinePos,
		FHANDLE_LINEAR, false, splinePos,
		FHANDLE_REL_INDEPENDENT,
		data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
		data.inputEvents.back().tilty, data.inputEvents.back().rotation,
		(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));

	glm::vec3 fragDir = splineDir * glm::quat(glm::vec3(0, 0, glm::radians(90.f)));
	glm::vec3 fragPos = glm::vec3(0);

	if (rake.pointsShapeMode == TSetProp::linear)
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
			std::cout << vec2VecToString({ fragPointA, fragPointB }) << std::endl;

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
			lengthIncrement = -glm::length(pointA - pointB) / (rake.points * 2);
			startPos = (pointA.y > pointB.y) ? pointB : pointA;
			
			/*
			for (int i = 0; i < (rake.points * 2) + 3; i++)
			{
				anchorIDCount++;
				fragPos = startPos - (fragDir * (lengthIncrement * i));
				fragData.anchors.push_back(FragmentAnchor(data.start.flag, anchorIDCount, fragPos, glm::vec3(fragDir.x, fragDir.y * -1, fragDir.z), 1.0f,
					FHANDLE_LINEAR, false, fragPos,
					FHANDLE_LINEAR, false, fragPos,
					FHANDLE_REL_INDEPENDENT,
					data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
					data.inputEvents.back().tilty, data.inputEvents.back().rotation,
					(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));
			}
			*/

			anchorIDCount++;
			fragData.anchors.push_back(FragmentAnchor(data.start.flag, anchorIDCount, pointA, glm::vec3(fragDir.x, fragDir.y * -1, fragDir.z), 1.0f,
				FHANDLE_LINEAR, false, pointA,
				FHANDLE_LINEAR, false, pointA,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));

			anchorIDCount++;
			fragData.anchors.push_back(FragmentAnchor(data.start.flag, anchorIDCount, pointB, glm::vec3(fragDir.x, fragDir.y * -1, fragDir.z), 1.0f,
				FHANDLE_LINEAR, false, pointB,
				FHANDLE_LINEAR, false, pointB,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));
		}
		else
		{
			lengthIncrement = rake.size / (rake.points * 2);
			startPos = splinePos + (fragDir * (rake.size / 2.0f));
			for (int i = 0; i < (rake.points * 2) + 3; i++)
			{
				anchorIDCount++;
				fragPos = startPos - (fragDir * (lengthIncrement * i));
				fragData.anchors.push_back(FragmentAnchor(data.start.flag, anchorIDCount, fragPos, glm::vec3(fragDir.x, fragDir.y * -1, fragDir.z), 1.0f,
					FHANDLE_LINEAR, false, fragPos,
					FHANDLE_LINEAR, false, fragPos,
					FHANDLE_REL_INDEPENDENT,
					data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
					data.inputEvents.back().tilty, data.inputEvents.back().rotation,
					(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));
			}
		}

		
		
	}

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
int In_Rake::click(Application* sender, MouseEvent dat)
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