#include "../../include/math/curve.h"
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
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 dir = makeDir(splineData.anchors.back().pos, pos);
	fragData.activeModKey = splineData.activeModKey = dat.modKey;
	bool isNew = (splineData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;

	if (controlScheme == TSetType::continuous)
	{
		wasHandled = continuousMove(sender, dat, &pos, &dir);
	}

	//clearFlagNew(isNew);
	return wasHandled;

	/*
	if (controlScheme == TSetType::continuous)
	{
		if (!continuousDraw(sender, dat, &continuous, &smoothing, &splineData, continuous.anchorSpacing, pos, dir))
		{ 
			return InputHandlerFlag::wait;
		}
	}

	splineIDCount++;
	splineData.anchors.push_back(FragmentAnchor(anchorIDCount, pos, dir, 1.0f, data.inputEvents.back()));

	glm::vec3 fragDir = dir * glm::quat(glm::vec3(0, 0, glm::radians(90.f)));
	glm::vec3 fragPos = glm::vec3(0);

	if (rake.rakeShape == TSetProp::linear)
	{
		float lengthIncrement = 0.0f;
		glm::vec3 startPos = glm::vec3(0.0f);
		int pointCount = 0;

		if (rake.pushToCanvasEdge)
		{
			glm::vec2 canvasSize = sender->getCanvasDimensions(); //canvasSize.y *= -1;
			glm::vec2 canvasV1 = glm::vec2(0);
			glm::vec2 canvasV2 = glm::vec2(canvasSize.x, 0);
			glm::vec2 canvasV3 = glm::vec2(canvasSize.x, canvasSize.y);
			glm::vec2 canvasV4 = glm::vec2(0, canvasSize.y);
			glm::vec4 canvasTop = glm::vec4(canvasV1, canvasV2);
			glm::vec4 canvasBottom = glm::vec4(canvasV4, canvasV3);
			glm::vec4 canvasLeft = glm::vec4(canvasV1, canvasV4);
			glm::vec4 canvasRight = glm::vec4(canvasV2, canvasV3);
			//
			glm::vec2 fragPointA = glm::vec2(pos);
			glm::vec2 fragPointB = glm::vec2(pos + (fragDir * 100.0f));
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
			lengthIncrement = -glm::length(pointA - pointB) / (rake.pointsCount * 2);
			startPos = (pointA.y > pointB.y) ? pointB : pointA;
			
			anchorIDCount++;
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pointA, fragDir, 1.0f, data.inputEvents.back()));

			anchorIDCount++;
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pointB, fragDir, 1.0f, data.inputEvents.back()));
			fragData.anchors.at(fragData.anchors.size() - 2).input.flagSecondary = InputFlag::point;
		}
		else
		{
			lengthIncrement = rake.size / (rake.pointsCount * 2);
			startPos = pos + (fragDir * (rake.size / 2.0f));
			for (int i = 0; i < (rake.pointsCount * 2) + 3; i++)
			{
				anchorIDCount++;
				fragPos = startPos - (fragDir * (lengthIncrement * i));
				fragData.anchors.push_back(FragmentAnchor(anchorIDCount, fragPos, fragDir, 1.0f, data.inputEvents.back()));
			}
			fragData.anchors.at(fragData.anchors.size() - size_t(((size_t)rake.pointsCount * 2) + 3)).input.flagSecondary = InputFlag::point;
		}
		wasHandled = InputHandlerFlag::allowPress;
	}

	clearFlagNew(isNew);
	if (isNew) { fragData.anchors.front().input.flagPrimary = InputFlag::newInput; }
	
	return wasHandled;
	*/
}
InputHandlerFlag In_Rake::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (controlScheme == TSetType::continuous)
	{
		wasHandled = continuousClick(sender, dat, 1, 1, InputFlag::null, InputFlag::null);
	}
	return wasHandled;
	/*
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
	{
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
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
	}
	return wasHandled;
	*/
}

InputHandlerFlag In_Rake::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (controlScheme == TSetType::continuous)
	{
		wasHandled = continuousKey(sender, dat, key, action, modKeys);
	}
	return wasHandled;
}

void In_Rake::initializeVertices(glm::vec3* pos, glm::vec3* dir, Input* dat,
	int waitCountVertex, int waitCountSpline, InputFlag vertexFlagSecondary, InputFlag splineFlagSecondary)
{
	splineData.anchors.push_back(FragmentAnchor(anchorIDCount, *pos, *dir, 1.0f, data.inputEvents.back()));
	splineData.anchors.back().input.flagPrimary = InputFlag::newInput;
	splineData.anchors.back().wait = waitCountSpline;
}

void In_Rake::addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{
	int count = 1;
	if (continuous.scatterConnections) {
		count = continuous.modulateCount(count, pos, &splineData.transform.origin, dir, dat, &anchorIDCount, &splineIDCount, &splineIDCount);
	}
	glm::vec3 outPos = *pos;
	glm::vec3 outDir = *dir;
	for (int i = 0; i < count; i++) {
		anchorIDCount++;
		outPos = *pos;
		if (continuous.scatterConnections) {
			continuous.modulatePosition(outPos, &image.tipSize, &outPos, &splineData.transform.origin, &outDir, dat, &anchorIDCount, &splineIDCount, &splineIDCount);
		}
		outDir = makeDir(splineData.anchors.at(splineData.anchors.size() - 2).pos, outPos) * glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)));

		if (rake.rakeShape == TSetProp::linear)
		{
			float lengthIncrement = 0.0f;
			glm::vec3 startPos = glm::vec3(0.0f);
			int pointCount = 0;

			if (rake.pushToCanvasEdge)
			{
				glm::vec2 canvasSize = rake.canvasDimensions; //canvasSize.y *= -1;
				glm::vec2 canvasV1 = glm::vec2(0);
				glm::vec2 canvasV2 = glm::vec2(canvasSize.x, 0);
				glm::vec2 canvasV3 = glm::vec2(canvasSize.x, canvasSize.y);
				glm::vec2 canvasV4 = glm::vec2(0, canvasSize.y);
				glm::vec4 canvasTop = glm::vec4(canvasV1, canvasV2);
				glm::vec4 canvasBottom = glm::vec4(canvasV4, canvasV3);
				glm::vec4 canvasLeft = glm::vec4(canvasV1, canvasV4);
				glm::vec4 canvasRight = glm::vec4(canvasV2, canvasV3);
				//
				glm::vec2 fragPointA = glm::vec2(*pos);
				glm::vec2 fragPointB = glm::vec2(*pos + (outDir * 100.0f));
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
				lengthIncrement = -glm::length(pointA - pointB) / (rake.pointsCount * 2);
				startPos = (pointA.y > pointB.y) ? pointB : pointA;

				anchorIDCount++;
				fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pointA, outDir, 1.0f, data.inputEvents.back()));

				anchorIDCount++;
				fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pointB, outDir, 1.0f, data.inputEvents.back()));
				fragData.anchors.at(fragData.anchors.size() - 2).input.flagSecondary = InputFlag::point;
			}
			else
			{
				lengthIncrement = rake.size / (rake.pointsCount * 2);
				startPos = *pos + (outDir * (rake.size / 2.0f));
				for (int i = 0; i < (rake.pointsCount * 2) + 3; i++)
				{
					anchorIDCount++;
					outPos = startPos - (outDir * (lengthIncrement * i));
					fragData.anchors.push_back(FragmentAnchor(anchorIDCount, outPos, outDir, 1.0f, data.inputEvents.back()));
				}
				fragData.anchors.at(fragData.anchors.size() - size_t(((size_t)rake.pointsCount * 2) + 3)).input.flagSecondary = InputFlag::point;
			}
		}
	}
}

void In_Rake::fillVertices(glm::vec3* pos1, glm::vec3* pos2, glm::vec3* dir, Input* dat)
{
	float distance = glm::length(*pos2 - *pos1);
	float usedDistance = 0.0f;
	while (usedDistance < distance)
	{
		glm::vec3 pos = lerpPos(*pos1, *pos2, usedDistance / distance);
		glm::vec3 dir = makeDir(*pos1, *pos2);
		anchorIDCount++;
		Input input = lerpInputs(&data.inputEvents.back(), &splineData.anchors.back().input, usedDistance);
		addVertices(&pos, &dir, &input);

		usedDistance += continuous.trueSpacing;
	}
}

void In_Rake::generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{
	FragmentAnchor* vertex1 = &splineData.anchors[splineData.anchors.size() - 2];
	FragmentAnchor* vertex2 = &splineData.anchors[splineData.anchors.size() - 1];
	int subdivCount = (continuous.subdivide) ? int(floor((glm::length(vertex2->pos - vertex1->pos) / continuous.trueSpacing) * continuous.subdivideAmount)) : 0;
	if (subdivCount == 0) {
		fillVertices(&vertex1->pos, &vertex1->pos, dir, dat);
	}
	else {
		for (int i = 1; i < subdivCount + 1; i++) {
			float t = float(i) / float(subdivCount);
			glm::vec3 subPos = lerpPos(vertex1->pos, *pos, t);
			glm::vec3 subDir = makeDir(splineData.anchors.at(splineData.anchors.size() - 2).pos, subPos);
			Input lerpDat = *dat;
			lerpDat.pressure = lerpf(vertex1->input.pressure, dat->pressure, t);
			addVertices(&subPos, &subDir, &lerpDat);
		}
	}
}

void In_Rake::generateCurve()
{
	FragmentAnchor* vertex1 = &splineData.anchors[splineData.anchors.size() - 2];
	FragmentAnchor* vertex2 = &splineData.anchors[splineData.anchors.size() - 1];
	glm::vec2 control_points[4] = { vertex1->pos, vertex1->headHandle, vertex2->tailHandle, vertex2->pos };

	std::vector<glm::vec3> points;
	float x = glm::length(control_points[1] - control_points[0]);
	float y = glm::length(control_points[2] - control_points[0]);
	int resolution = int(10 + ((x * y) / 10000.0f));
	glm::vec4 startScale = glm::vec4(1.0f); glm::vec4 endScale = glm::vec4(1.0f);
	float segLen = glm::length(vertex2->pos - vertex1->pos);
	float usedLen = 0.0f;
	getCubicBezierPoints(control_points, resolution, 0.0f, points, segLen, continuous.trueSpacing, startScale.z, endScale.z);
	for (int i = 0; i < points.size(); i++) {
		glm::vec3 pos1 = glm::vec3(0.0f);
		glm::vec3 pos2 = points[i];
		glm::vec3 dir = DEFAULT_DIR;
		if (i == 0) {
			pos1 = vertex1->pos;
			pos2 = points[i];
		}
		else if (i == points.size() - 1) {
			pos1 = points[i];
			pos2 = vertex2->pos;
		}
		else if (points.size() >= 2) {
			pos1 = points[(size_t)i - 1];
			pos2 = points[i];
		}
		else {
			pos1 = points[i];
			pos2 = vertex2->pos;
		}
		dir = makeDir(pos1, pos2);
		Input input = lerpInputs(&vertex1->input, &vertex2->input, usedLen / segLen);
		fillVertices(&pos1, &pos2, &dir, &input);
		usedLen += continuous.trueSpacing;
	}
}

void In_Rake::newInput(Application* sender, Input dat)
{
	if (owner.get()->checkInterestMask(TSetType::rake)) { rake = *owner.get()->getRake(); }
	else { rake.isEnabled = false; }
	rake.canvasDimensions = sender->getCanvasDimensions();
}