#include "../include/methods/In_Polygon.h"
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

int In_Polygon::move(Application* sender, MouseEvent dat)
{
	glm::vec3 cursorPos = sender->pickScreenCoord(dat.x, dat.y);

	bool rotateMode = false, moveMode = false, constrainRatio = false;

	if (controlScheme == TSetType::onepoint)
	{
		if (!onePointMove(sender, dat, &onePoint, cursorPos)) { return INPUT_WAIT; }
		rotateMode = (sender->getKeyState(onePoint.rotateModeKey)) ? true : false;
		moveMode = (sender->getKeyState(onePoint.moveOriginModeKey)) ? true : false;
		constrainRatio = (sender->getKeyState(onePoint.constrainRatioKey)) ? true : false;
	}

	// Verticies are counted clockwise from the upper-left corner (origin-point)
	if (polygon.sides == 4)
	{
		// Set the shape verticies based on bounding box
		std::vector<glm::vec3> verts = {
			fragData.transform.bounds.p1,
			fragData.transform.bounds.p2,
			fragData.transform.bounds.p3,
			fragData.transform.bounds.p4
		};
		// Rotate the verticies based on the transform roll. Note that the transform itself is never rotated, and all rotation calculations
		// are applied after determining the bounding box.
		glm::quat rotation = glm::quat(glm::vec3(0, 0, glm::radians(fragData.transform.roll)));
		for (int i = 0; i < verts.size(); i++)
		{
			verts[i] = fragData.transform.origin + (rotation * (verts[i] - fragData.transform.origin));
		}
		
		
		anchorIDCount++;
		// For now , only use the threshold as a literal pixel distance value. Later would either use relative vs brush size or
		// relatative vs a fraction of the diagonal length?
		if (controlScheme == TSetType::onepoint)
		{
			float lenFactor = (onePoint.pressureMapRelative) ? onePoint.pressureMapThreshold : onePoint.pressureMapThreshold;
			for (int i = 0; i < verts.size(); i++)
			{
				fragData.anchors[size_t(i)].pos = verts[size_t(i)];
				fragData.anchors[size_t(i)].ID = anchorIDCount;
				if (onePoint.pressureMapping)
				{
					float inPressure = (data.inputEvents.back().pressure * onePoint.pressureMapStrength) / 10;
					float dist = glm::length(cursorPos - fragData.anchors[size_t(i)].pos);
					if (dist <= lenFactor)
					{
						fragData.anchors[size_t(i)].pressure += inPressure * (1 + ((dist / lenFactor) * (0 - 1)));
						fragData.anchors[size_t(i)].pressure = clampf(fragData.anchors[size_t(i)].pressure, 0.0f, data.inputEvents.back().pressure);
					}
					else
					{
						fragData.anchors[size_t(i)].pressure -= onePoint.pressureMapDecayRate / ((moveMode || rotateMode) ? 1000.0f : 100.0f);
						fragData.anchors[size_t(i)].pressure = clampf(fragData.anchors[size_t(i)].pressure, 0.05f, 1.0f);
					}
				}
				else { fragData.anchors[size_t(i)].pressure = data.inputEvents.back().pressure; }
			}
		}
		
		// Set direction values
		for (int i = 0; i < verts.size(); i++)
		{
			fragData.anchors[size_t(i)].dir = glm::normalize(glm::vec3(
				fragData.anchors[size_t(loopInc(i, 0, 3))].pos.x - fragData.anchors[size_t(i)].pos.x,
				fragData.anchors[size_t(i)].pos.y - fragData.anchors[size_t(loopInc(i, 0, 3))].pos.y,
				0.0f));
		}
	}

	return INPUT_ALLOW_PRESS_UPDATECURSOR;
}

int In_Polygon::click(Application* sender, MouseEvent dat)
{
	int wasHandled = INPUT_NOSIGNAL;
	if (dat.action == INPUT_PRESS && dat.button == UI_MOUSE_LEFT)
	{
		polygon = *owner.get()->getPolygon();
		// Reset the data
		startAngle = -360.0f;
		data.reset();
		data.start = dat;
		data.inputModKey = dat.modKey;
		// Push back a two data points (For moveMode if triggered on first frame)
		data.inputEvents.push_back(dat);
		data.inputEvents.push_back(dat);
		// Reset the fragData
		fragData.reset();
		fragData.constantSize = true;
		fragData.linearStream = true;
		fragData.connectEnds = true;
		fragData.startTime = (float)data.start.time;
		// Switch on controlScheme
		float iniAngle = 0.0f;
		switch (controlScheme)
		{
		case TSetType::onepoint:
		default:
			onePoint = *owner.get()->getOnePointControl();
			fragData.centerAboutOrigin =
				(dat.modKey != INPUT_MOD_NONE && dat.modKey == onePoint.sizeAboutOriginKey) ? true : false;
			iniAngle = onePoint.initialRotation;
		}
		anchorIDCount = 0;
		// Set up the polygon
		// The origin will always be the locus of control. When centering/sizing about origin, the vertex
		// data is simply shifted negatively into the other 3 quadrants left/above the origin
		glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
		fragData.transform.roll = iniAngle;
		fragData.transform.origin = origin;
		fragData.transform.bounds = RectBounds(origin);

		// For now only handle the square input with no rotation or modKeys, in order to have at least one pipeline from
		// input to visualization
		if (polygon.sides == 4)
		{
			fragData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, anchorIDCount, origin, glm::vec3(0, 0, 0), 0.0f,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
			fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, anchorIDCount, origin, glm::vec3(0, 0, 0), 0.0f,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
			fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, anchorIDCount, origin, glm::vec3(0, 0, 0), 0.0f,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
			fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, anchorIDCount, origin, glm::vec3(0, 0, 0), 0.0f,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_LINEAR, false, origin,
				FHANDLE_REL_INDEPENDENT,
				data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
		}
		anchorIDCount++;
		wasHandled = INPUT_ALLOW_PRESS_UPDATECURSOR;
	}
	else if (dat.action == INPUT_RELEASE && dat.button == UI_MOUSE_LEFT)
	{
		data.end = dat;
		fragData.endTime = (float)data.end.time;
		wasHandled = INPUT_ALLOW_RELEASE;
		//std::cout << "IN_POLYGON::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = INPUT_NOSIGNAL;
		//std::cout << "IN_POLYGON::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "IN_POLYGON::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}