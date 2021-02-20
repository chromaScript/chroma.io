#include "../include/methods/In_PolyLine.h"
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

int In_PolyLine::move(Application* sender, MouseEvent dat)
{
	glm::vec3 cursorPos = sender->pickScreenCoord(dat.x, dat.y);

	bool rotateMode = false, snapMode = false, extrudeMode = false, moveMode = false, constrainRatio = false;

	if (controlScheme == TSetType::drag)
	{
		if (!dragMove(sender, dat, &drag, cursorPos)) { return INPUT_WAIT; }
		snapMode = (sender->getKeyState(drag.snapModeKey)) ? true : false;
		moveMode = (sender->getKeyState(drag.moveOriginModeKey)) ? true : false;
		constrainRatio = (sender->getKeyState(drag.snapModeKey)) ? true : false;
		extrudeMode = (sender->getKeyState(drag.extrudeModeKey)) ? true : false;
	}

	glm::quat rotate = glm::quat(glm::vec3(0, 0, glm::radians(fragData.transform.roll)));
	// Verticies are counted clockwise from the upper-left corner (origin-point)
	if (fragData.anchors.front().flag == FLAG_NEW_INPUT)
	{
		fragData.anchors.front().flag == FLAG_NULL;
	}

	// Set the shape verticies based on bounding box
	std::vector<glm::vec3> verts = {
		fragData.transform.origin + (rotate * (fragData.transform.bounds.p1 - fragData.transform.origin)),
		fragData.transform.origin + (rotate * (fragData.transform.bounds.p4 - fragData.transform.origin)),
		fragData.transform.origin + (rotate * (fragData.transform.bounds.p2 - fragData.transform.origin)),
		fragData.transform.origin + (rotate * (fragData.transform.bounds.p3 - fragData.transform.origin))
	};
	anchorIDCount++;
	
	// For now , only use the threshold as a literal pixel distance value. Later would either use relative vs brush size or
	// relatative vs a fraction of the diagonal length?
	if (controlScheme == TSetType::drag)
	{
		float lenFactor = (drag.pressureMapRelative) ? drag.pressureMapThreshold : drag.pressureMapThreshold;
		for (int i = 0; i < verts.size(); i++)
		{
			FragmentAnchor* anchor = nullptr;
			switch (i)
			{
			case 0:
				anchor = &fragData.anchors[1];
				anchor->pos = verts[0];
				break;
			case 1:
				anchor = &fragData.anchors[0];
				anchor->pos = verts[1];
				break;
			case 2:
				anchor = &fragData.anchors[fragData.anchors.size() - 2];
				anchor->pos = verts[2];
				break;
			case 3:
				anchor = &fragData.anchors[fragData.anchors.size() - 1];
				anchor->pos = verts[3];
				break;
			}
			anchor->ID = anchorIDCount;
			/*
			if (drag.pressureMapping)
			{
				float inPressure = (data.inputEvents.back().pressure * drag.pressureMapStrength) / 10;
				float dist = glm::length(cursorPos - anchor->pos);
				if (dist <= lenFactor)
				{
					anchor->pressure += inPressure * (1 + ((dist / lenFactor) * (0 - 1)));
					anchor->pressure = clampf(anchor->pressure, 0.0f, data.inputEvents.back().pressure);
				}
				else
				{
					anchor->pressure -= drag.pressureMapDecayRate / ((moveMode || rotateMode) ? 1000.0f : 100.0f);
					anchor->pressure = clampf(anchor->pressure, 0.05f, 1.0f);
				}
			}
			else { anchor->pressure = data.inputEvents.back().pressure; }
			*/
		}
	}

	// Set direction values
	for (int i = 0; i < verts.size(); i++)
	{
		FragmentAnchor* anchor = nullptr;
		switch (i)
		{
		case 0:
			anchor = &fragData.anchors[1];
			anchor->dir = glm::normalize(glm::vec3(
				fragData.anchors[1].pos.x - fragData.anchors[2].pos.x,
				fragData.anchors[2].pos.y - fragData.anchors[1].pos.y,
				0.0f));
			break;
		case 1:
			anchor = &fragData.anchors[0];
			anchor->dir = glm::normalize(glm::vec3(
				fragData.anchors[0].pos.x - fragData.anchors[1].pos.x,
				fragData.anchors[1].pos.y - fragData.anchors[0].pos.y,
				0.0f));
			break;
		case 2:
			anchor = &fragData.anchors[fragData.anchors.size() - 2];
			anchor->dir = glm::quat(glm::vec3(0, 0, glm::radians(180.0f))) * glm::normalize(glm::vec3(
				fragData.anchors[fragData.anchors.size() - 2].pos.x - fragData.anchors[fragData.anchors.size() - 1].pos.x,
				fragData.anchors[fragData.anchors.size() - 1].pos.y - fragData.anchors[fragData.anchors.size() - 2].pos.y,
				0.0f));
			break;
		case 3:
			anchor = &fragData.anchors[fragData.anchors.size() - 1];
			anchor->dir = glm::normalize(glm::vec3(
				fragData.anchors[fragData.anchors.size() - 2].pos.x - fragData.anchors[fragData.anchors.size() - 1].pos.x,
				fragData.anchors[fragData.anchors.size() - 1].pos.y - fragData.anchors[fragData.anchors.size() - 2].pos.y,
				0.0f));
			break;
		}
	}

	// Apply Miter Joints
	if (polyLine.useMiterJoints && inputCount > 1)
	{
		
		
		
	}
	return INPUT_ALLOW_PRESS_UPDATECURSOR;
}

int In_PolyLine::click(Application* sender, MouseEvent dat)
{
	int wasHandled = INPUT_NOSIGNAL;
	if (dat.action == INPUT_PRESS && dat.button == UI_MOUSE_LEFT)
	{
		if (sender->getIsDoingInput())
		{
			glm::vec3 point = glm::vec3(0.0f), dir = glm::vec3(0.0f);
			point = sender->pickScreenCoord(dat.x, dat.y);
			// Update the transform
			glm::quat rotation = glm::quat(glm::vec3(0, 0, glm::radians(fragData.transform.roll)));
			fragData.transform.origin =
				((fragData.transform.origin + (rotation * (fragData.transform.bounds.p3 - fragData.transform.origin))) +
				(fragData.transform.origin + (rotation * (fragData.transform.bounds.p4 - fragData.transform.origin)))) /
				2.0f;
			fragData.transform.roll = -360.0f;
			fragData.transform.bounds = RectBounds(point);
			// Erase the overlapping verts from previous miter joint
			if (polyLine.useMiterJoints && inputCount != 0 && polyLine.bevelType == TSetProp::none)
			{
				fragData.anchors.erase(fragData.anchors.begin() + 2);
				fragData.anchors.erase(fragData.anchors.end() - 2);
			}
			// Apply bevel miter joints if enabled
			else if (polyLine.useMiterJoints && inputCount > 1 && polyLine.bevelType != TSetProp::none)
			{
				glm::vec3 leftPoint = glm::vec3(lineIntersect2D(glm::vec4(
					fragData.anchors[2].pos.x, fragData.anchors[2].pos.y,
					fragData.anchors[3].pos.x, fragData.anchors[3].pos.y
				), glm::vec4(
					fragData.anchors[4].pos.x, fragData.anchors[4].pos.y,
					fragData.anchors[5].pos.x, fragData.anchors[5].pos.y
				)), 0.0f);
				glm::vec3 rightPoint = glm::vec3(lineIntersect2D(glm::vec4(
					fragData.anchors[fragData.anchors.size() - 3].pos.x, fragData.anchors[fragData.anchors.size() - 3].pos.y,
					fragData.anchors[fragData.anchors.size() - 4].pos.x, fragData.anchors[fragData.anchors.size() - 4].pos.y
				), glm::vec4(
					fragData.anchors[fragData.anchors.size() - 5].pos.x, fragData.anchors[fragData.anchors.size() - 5].pos.y,
					fragData.anchors[fragData.anchors.size() - 6].pos.x, fragData.anchors[fragData.anchors.size() - 6].pos.y
				)), 0.0f);

				if (polyLine.bevelType != TSetProp::none)
				{
					// Calculate the furthest points on the bevel
					float leftDot = (glm::degrees(glm::dot(fragData.anchors[2].dir, fragData.anchors[3].dir)) < 0.0f) ? -1.0f : 1.0f;
					glm::vec3 leftFront = leftPoint - (glm::normalize(leftPoint - fragData.anchors[3].pos) * (polyLine.bevelWidth * leftDot));
					glm::vec3 leftBack = leftPoint - (glm::normalize(leftPoint - fragData.anchors[4].pos) * (polyLine.bevelWidth * leftDot));
					glm::vec3 rightFront = rightPoint + (glm::normalize(fragData.anchors[fragData.anchors.size() - 4].pos - rightPoint) * (polyLine.bevelWidth * leftDot * -1));
					glm::vec3 rightBack = rightPoint + (glm::normalize(fragData.anchors[fragData.anchors.size() - 5].pos - rightPoint) * (polyLine.bevelWidth * leftDot * -1));
					// Apply this to the bevel points
					fragData.anchors[3].pos = leftFront;
					fragData.anchors[4].pos = leftBack;
					fragData.anchors[fragData.anchors.size() - 4].pos = rightFront;
					fragData.anchors[fragData.anchors.size() - 5].pos = rightBack;
				}
				else
				{
					fragData.anchors[1].pos = leftPoint;
					fragData.anchors[2].pos = leftPoint;
					fragData.anchors[fragData.anchors.size() - 2].pos = rightPoint;
					fragData.anchors[fragData.anchors.size() - 3].pos = rightPoint;
				}
			}
			// Also clean up the last verts if this is a terminate operation.
			if (sender->getKeyState(drag.terminateInputKey))
			{
				// Erase the overlapping verts from previous miter joint
				if (polyLine.useMiterJoints && polyLine.bevelType == TSetProp::none)
				{
					fragData.anchors.erase(fragData.anchors.begin() + 2);
					fragData.anchors.erase(fragData.anchors.end() - 2);
				}
				// Apply bevel miter joints if enabled
				else if (polyLine.useMiterJoints && polyLine.bevelType != TSetProp::none)
				{
					glm::vec3 leftPoint = glm::vec3(lineIntersect2D(glm::vec4(
						fragData.anchors[0].pos.x, fragData.anchors[0].pos.y,
						fragData.anchors[1].pos.x, fragData.anchors[1].pos.y
					), glm::vec4(
						fragData.anchors[2].pos.x, fragData.anchors[2].pos.y,
						fragData.anchors[3].pos.x, fragData.anchors[3].pos.y
					)), 0.0f);
					glm::vec3 rightPoint = glm::vec3(lineIntersect2D(glm::vec4(
						fragData.anchors[fragData.anchors.size() - 1].pos.x, fragData.anchors[fragData.anchors.size() - 1].pos.y,
						fragData.anchors[fragData.anchors.size() - 2].pos.x, fragData.anchors[fragData.anchors.size() - 2].pos.y
					), glm::vec4(
						fragData.anchors[fragData.anchors.size() - 3].pos.x, fragData.anchors[fragData.anchors.size() - 3].pos.y,
						fragData.anchors[fragData.anchors.size() - 4].pos.x, fragData.anchors[fragData.anchors.size() - 4].pos.y
					)), 0.0f);

					if (polyLine.bevelType != TSetProp::none)
					{
						// Calculate the furthest points on the bevel
						float leftDot = (glm::degrees(glm::dot(fragData.anchors[1].dir, fragData.anchors[2].dir)) < 0.0f) ? -1.0f : 1.0f;
						glm::vec3 leftFront = leftPoint - (glm::normalize(leftPoint - fragData.anchors[1].pos) * (polyLine.bevelWidth * leftDot));
						glm::vec3 leftBack = leftPoint - (glm::normalize(leftPoint - fragData.anchors[2].pos) * (polyLine.bevelWidth * leftDot));
						glm::vec3 rightFront = rightPoint + (glm::normalize(fragData.anchors[fragData.anchors.size() - 2].pos - rightPoint) * (polyLine.bevelWidth * leftDot * -1));
						glm::vec3 rightBack = rightPoint + (glm::normalize(fragData.anchors[fragData.anchors.size() - 3].pos - rightPoint) * (polyLine.bevelWidth * leftDot * -1));
						// Apply this to the bevel points
						fragData.anchors[1].pos = leftFront;
						fragData.anchors[2].pos = leftBack;
						fragData.anchors[fragData.anchors.size() - 2].pos = rightFront;
						fragData.anchors[fragData.anchors.size() - 3].pos = rightBack;
					}
					else
					{
						fragData.anchors[1].pos = leftPoint;
						fragData.anchors[2].pos = leftPoint;
						fragData.anchors[fragData.anchors.size() - 2].pos = rightPoint;
						fragData.anchors[fragData.anchors.size() - 3].pos = rightPoint;
					}
				}
				wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
			}
			else
			{
				// Add the next set of verts for the line
				fragData.anchors.push_front(FragmentAnchor(FLAG_NULL, anchorIDCount, point, glm::vec3(0, 0, 0), 0.0f,
					FHANDLE_LINEAR, false, point,
					FHANDLE_LINEAR, false, point,
					FHANDLE_REL_INDEPENDENT,
					data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
					data.inputEvents.back().tilty, data.inputEvents.back().rotation,
					(float)data.start.time, sender->getMouseVelocity(), 1.0f));
				fragData.anchors.push_front(FragmentAnchor(FLAG_NULL, anchorIDCount, point, glm::vec3(0, 0, 0), 0.0f,
					FHANDLE_LINEAR, false, point,
					FHANDLE_LINEAR, false, point,
					FHANDLE_REL_INDEPENDENT,
					data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
					data.inputEvents.back().tilty, data.inputEvents.back().rotation,
					(float)data.start.time, sender->getMouseVelocity(), 1.0f));
				fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, anchorIDCount, point, glm::vec3(0, 0, 0), 0.0f,
					FHANDLE_LINEAR, false, point,
					FHANDLE_LINEAR, false, point,
					FHANDLE_REL_INDEPENDENT,
					data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
					data.inputEvents.back().tilty, data.inputEvents.back().rotation,
					(float)data.start.time, sender->getMouseVelocity(), 1.0f));
				fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, anchorIDCount, point, glm::vec3(0, 0, 0), 0.0f,
					FHANDLE_LINEAR, false, point,
					FHANDLE_LINEAR, false, point,
					FHANDLE_REL_INDEPENDENT,
					data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
					data.inputEvents.back().tilty, data.inputEvents.back().rotation,
					(float)data.start.time, sender->getMouseVelocity(), 1.0f));
				inputCount++;
				anchorIDCount++;
				wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
			}
		}
		else
		{
			polyLine = *owner.get()->getPolyLine();
			glm::vec3 point = glm::vec3(0.0f), dir = glm::vec3(0.0f);
			resetInput(sender, dat, point, dir);
			drag.enableExtrusion = true;
			drag.moveOriginModeKey = -1;
			// For now only handle the square input with no rotation or modKeys, in order to have at least one pipeline from
			// input to visualization
			
			fragData.anchors.push_front(FragmentAnchor(FLAG_NEW_INPUT, anchorIDCount, point, glm::vec3(0, 0, 0), 0.0f,
				FHANDLE_LINEAR, false, point,
				FHANDLE_LINEAR, false, point,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
			fragData.anchors.push_front(FragmentAnchor(FLAG_NEW_INPUT, anchorIDCount, point, glm::vec3(0, 0, 0), 0.0f,
				FHANDLE_LINEAR, false, point,
				FHANDLE_LINEAR, false, point,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
			fragData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, anchorIDCount, point, glm::vec3(0, 0, 0), 0.0f,
				FHANDLE_LINEAR, false, point,
				FHANDLE_LINEAR, false, point,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
			fragData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, anchorIDCount, point, glm::vec3(0, 0, 0), 0.0f,
				FHANDLE_LINEAR, false, point,
				FHANDLE_LINEAR, false, point,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
				
			anchorIDCount++;
			wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
		}
	}
	else if (dat.action == INPUT_RELEASE && dat.button == UI_MOUSE_LEFT)
	{
		if (sender->getKeyState(drag.terminateInputKey))
		{
     			data.end = dat;
			fragData.endTime = (float)data.end.time;
			wasHandled = INPUT_ALLOW_RELEASE;
			//std::cout << "IN_POLYGON::CLICK::ALLOW_RELEASE" << std::endl;
		}
		else
		{
			data.end = dat;
			fragData.endTime = (float)data.end.time;
			wasHandled = INPUT_CONTINUE;
			//std::cout << "IN_POLYGON::CLICK::CONTINUE_INPUT" << std::endl;
		}
		
		//data.end = dat;
		//fragData.endTime = (float)data.end.time;
		//wasHandled = INPUT_ALLOW_RELEASE;
		//std::cout << "IN_POLYGON::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = INPUT_NOSIGNAL;
		std::cout << "IN_POLYLINE::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "IN_POLYLINE::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}