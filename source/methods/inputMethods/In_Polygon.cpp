#include "../../include/methods/inputMethods/In_Polygon.h"
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

InputHandlerFlag In_Polygon::move(Application* sender, Input dat)
{
	glm::vec3 cursorPos = sender->pickScreenCoord(dat.x, dat.y);

	bool rotateMode = false, moveMode = false, constrainRatio = false;

	if (controlScheme == TSetType::onepoint)
	{
		if (!onePointMove(sender, dat, &onePoint, cursorPos)) { return InputHandlerFlag::wait; }
		rotateMode = (sender->getKeyState(onePoint.rotateModeKey.modKey)) ? true : false;
		moveMode = (sender->getKeyState(onePoint.moveOriginModeKey.modKey)) ? true : false;
		constrainRatio = (sender->getKeyState(onePoint.constrainRatioKey.modKey)) ? true : false;
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
						fragData.anchors[size_t(i)].input.pressure += inPressure * (1 + ((dist / lenFactor) * (0 - 1)));
						fragData.anchors[size_t(i)].input.pressure = clampf(fragData.anchors[size_t(i)].input.pressure, 0.0f, data.inputEvents.back().pressure);
					}
					else
					{
						fragData.anchors[size_t(i)].input.pressure -= onePoint.pressureMapDecayRate / ((moveMode || rotateMode) ? 1000.0f : 100.0f);
						fragData.anchors[size_t(i)].input.pressure = clampf(fragData.anchors[size_t(i)].input.pressure, 0.05f, 1.0f);
					}
				}
				else { fragData.anchors[size_t(i)].input.pressure = data.inputEvents.back().pressure; }
			}
		}
		
		// Set direction values
		for (int i = 0; i < verts.size(); i++)
		{
			fragData.anchors[size_t(i)].dir = glm::normalize(glm::vec3(
				fragData.anchors[size_t(i)].pos.x - fragData.anchors[size_t(loopInc(i, 0, 3))].pos.x,
				fragData.anchors[size_t(i)].pos.y - fragData.anchors[size_t(loopInc(i, 0, 3))].pos.y,
				0.0f));
		}
	}

	return InputHandlerFlag::allowPress_updateCursor;
}

InputHandlerFlag In_Polygon::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
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
				(dat.modKey != InputModKey::none && compareModKey(onePoint.sizeAboutOriginKey, dat.modKey, false)) ? true : false;
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
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				0.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				data.inputEvents.back()));
			fragData.anchors.back().input.flagPrimary = InputFlag::newInput;
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				0.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				data.inputEvents.back()));
			fragData.anchors.back().input.flagPrimary = InputFlag::null;
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				0.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				data.inputEvents.back()));
			fragData.anchors.back().input.flagPrimary = InputFlag::null;
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				0.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				data.inputEvents.back()));
			fragData.anchors.back().input.flagPrimary = InputFlag::null;
		}
		anchorIDCount++;
		wasHandled = InputHandlerFlag::allowPress_updateCursor;
	}
	else if (dat.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		data.end = dat;
		fragData.endTime = (float)data.end.time;
		wasHandled = InputHandlerFlag::release;
		//std::cout << "MethodType::in_polygon::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
		//std::cout << "MethodType::in_polygon::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "MethodType::in_polygon::CLICK::TIME=" << dat.time << "::TYPE=" << data.inputModKey << std::endl;
	return wasHandled;
}

InputHandlerFlag In_Polygon::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	return InputHandlerFlag::noSignal;
}

void In_Polygon::initializeVertices(glm::vec3* pos, glm::vec3* dir, Input* dat,
	int waitCountVertex, int waitCountSpline, InputFlag vertexFlagSecondary, InputFlag splineFlagSecondary)
{

}

void In_Polygon::addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{

}

void In_Polygon::generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{

}

void In_Polygon::generateCurve()
{

}

void In_Polygon::newInput(Application* sender, Input dat)
{

}