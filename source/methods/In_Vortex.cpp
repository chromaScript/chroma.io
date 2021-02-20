#include "../include/methods/In_Vortex.h"
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

int In_Vortex::move(Application* sender, MouseEvent dat)
{
	glm::vec3 point = sender->pickScreenCoord(dat.x, dat.y);
	
	if (controlScheme == TSetType::continuous)
	{
		
		glm::vec3 splineDir = glm::normalize(glm::vec3(
			point.x - splineData.anchors.back().pos.x,
			splineData.anchors.back().pos.y - point.y,
			0.0f));
		bool isNew = (splineData.anchors.front().flag == FLAG_NEW_INPUT) ? true : false;
		if (!continuousMove(sender, dat, &continuous, nullptr, true, point, splineDir)) { return INPUT_WAIT; }
		splineIDCount++;
		splineData.anchors.push_back(FragmentAnchor(data.start.flag, splineIDCount, point, splineDir, 1.0f,
			FHANDLE_LINEAR, false, point,
			FHANDLE_LINEAR, false, point,
			FHANDLE_REL_INDEPENDENT,
			data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
			data.inputEvents.back().tilty, data.inputEvents.back().rotation,
			(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));

		glm::vec3 fragDir = splineDir;
		glm::vec3 fragPos = glm::vec3(0);

		float angleIncrement = 360.0f / vortex.ringDensity;

		if (vortex.connectionOrdering == TSetProp::useDefault)
		{
			// Use simplest ordering method of nested loop
			for (int i = 0; i < (int)vortex.rings; i++)
			{
				float radiusOffset = (i + 1) * (vortex.radius / (int)vortex.rings + 1);
				for (int i = 0; i < vortex.ringDensity; i++)
				{
					anchorIDCount++;
					glm::quat rotation = glm::quat(
						glm::vec3(0, 0, glm::degrees(atan2(splineDir.x, splineDir.y)) + glm::radians(i * angleIncrement)));
					fragPos = point + glm::vec3(radiusOffset, 0.0f, 0.0f);
					fragPos = point + (rotation * (fragPos - point));
					fragDir = glm::normalize(glm::vec3(point - fragPos));
					fragData.anchors.push_back(FragmentAnchor(data.start.flag, anchorIDCount, fragPos, fragDir, 1.0f,
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
	}
	else if (controlScheme == TSetType::drag)
	{
		if (!dragMove(sender, dat, &drag, point)) { return INPUT_WAIT; }

		// Clear the NEW TAG and erase the temporary holding vertex
		if (fragData.anchors.size() == 1 && fragData.anchors.front().flag == FLAG_NEW_INPUT)
		{
			fragData.anchors.pop_back();
			fragData.anchors.shrink_to_fit();
		}

		if (vortex.constantRingCount)
		{
			fragData.resetAnchors();
			glm::vec3 dragDir = glm::normalize(glm::vec3(
				point.x - fragData.transform.origin.x,
				fragData.transform.origin.y - point.y,
				0.0f));

			glm::vec3 fragDir = dragDir;
			glm::vec3 fragPos = glm::vec3(0);

			float angleIncrement = 360.0f / vortex.ringDensity;
			float totalLen = glm::length(point - fragData.transform.origin);

			if (vortex.connectionOrdering == TSetProp::useDefault)
			{
				// Use simplest ordering method of nested loop
				for (int i = 0; i < (int)vortex.rings; i++)
				{
					float radiusOffset = (i + 1) * (totalLen / (int)vortex.rings + 1);
					for (int i = 0; i < vortex.ringDensity; i++)
					{
						anchorIDCount++;
						float angle = atan2(dragDir.x, dragDir.y) + glm::radians(i * angleIncrement);
						glm::quat rotation = glm::quat(glm::vec3(0, 0, angle));
						fragPos = fragData.transform.origin + glm::vec3(radiusOffset, 0.0f, 0.0f);
						fragPos = fragData.transform.origin + (rotation * (fragPos - fragData.transform.origin));
						fragDir = glm::normalize(glm::vec3(point - fragPos));
						fragData.anchors.push_back(FragmentAnchor(data.start.flag, anchorIDCount, fragPos, fragDir, 1.0f,
							FHANDLE_LINEAR, false, fragPos,
							FHANDLE_LINEAR, false, fragPos,
							FHANDLE_REL_INDEPENDENT,
							data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
							data.inputEvents.back().tilty, data.inputEvents.back().rotation,
							(float)data.inputEvents.back().time, sender->getMouseVelocity(), 1.0f));
					}
				}
			}
		}
	}

	return INPUT_ALLOW_PRESS_UPDATECURSOR;
}
int In_Vortex::click(Application* sender, MouseEvent dat)
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
			vortex = *owner.get()->getVortex();
			glm::vec3 point = glm::vec3(0.0f);
			glm::vec3 dir = glm::vec3(0.0f);
			resetInput(sender, dat, point, dir);
			splineData.transform.origin = point;
			if (controlScheme == TSetType::continuous)
			{
				// Add the first anchor point, there is no direction yet, so a manual wait will be placed on the
				// anchor to count off 1 more anchor before rendering.
				continuous.anchorSpacing = 140.0f;
				splineData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, splineIDCount, point, dir, 1.0f,
					FHANDLE_LINEAR, false, point,
					FHANDLE_LINEAR, false, point,
					FHANDLE_REL_INDEPENDENT,
					data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
					data.inputEvents.back().tilty, data.inputEvents.back().rotation,
					(float)data.start.time, sender->getMouseVelocity(), 1.0f));
				splineIDCount++;
				splineData.anchors.back().wait = 1;
			}
			else if (controlScheme == TSetType::drag)
			{
				// Place a single vertex in the array in order to flag a new input
				drag.enableExtrusion = false;
				drag.initialExtrusion = 0.0f;
				startExtrude = 0.0f;
				fragData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, anchorIDCount, point, dir, 0.0f,
					FHANDLE_LINEAR, false, point,
					FHANDLE_LINEAR, false, point,
					FHANDLE_REL_INDEPENDENT,
					data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
					data.inputEvents.back().tilty, data.inputEvents.back().rotation,
					(float)data.start.time, sender->getMouseVelocity(), 1.0f));
				anchorIDCount++;
			}
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