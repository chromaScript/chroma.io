#include "../../include/methods/inputMethods/In_Vortex.h"
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

InputHandlerFlag In_Vortex::move(Application* sender, Input dat)
{
	glm::vec3 point = sender->pickScreenCoord(dat.x, dat.y);
	
	if (controlScheme == TSetType::continuous)
	{
		
		glm::vec3 splineDir = glm::normalize(glm::vec3(
			point.x - splineData.anchors.back().pos.x,
			point.y - splineData.anchors.back().pos.y,
			0.0f));
		bool isNew = (splineData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
		if (!continuousMove(sender, dat, &continuous, nullptr, &splineData, continuous.anchorSpacing, point, splineDir)) { return InputHandlerFlag::wait; }
		splineIDCount++;
		splineData.anchors.push_back(
			FragmentAnchor(anchorIDCount, point, splineDir,
				1.0f,
				HandleType::linear, false, point,
				HandleType::linear, false, point,
				HandleRel::independent,
				data.inputEvents.back()));

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
					fragDir = glm::normalize(glm::vec3(point - fragPos)) * glm::quat(glm::vec3(0.0f, 0.0f, MATH_PI / -2.0f));
					fragData.anchors.push_back(
						FragmentAnchor(anchorIDCount, fragPos, fragDir,
							1.0f,
							HandleType::linear, false, fragPos,
							HandleType::linear, false, fragPos,
							HandleRel::independent,
							data.inputEvents.back()));
				}
				fragData.anchors.at(fragData.anchors.size() - vortex.ringDensity).input.flagPrimary = InputFlag::point;
			}
		}
		fragData.anchors.at(fragData.anchors.size() - ((int)vortex.rings * vortex.ringDensity)).input.flagPrimary = InputFlag::point;

		if (isNew)
		{
			splineData.anchors.front().input.flagPrimary = InputFlag::null;
			fragData.anchors.front().input.flagPrimary = InputFlag::newInput;
		}
		else if (fragData.anchors.front().input.flagPrimary == InputFlag::newInput)
		{
			fragData.anchors.front().input.flagPrimary = InputFlag::null;
		}
	}
	else if (controlScheme == TSetType::drag)
	{
		if (!dragMove(sender, dat, &drag, point)) { return InputHandlerFlag::wait; }

		// Clear the NEW TAG and erase the temporary holding vertex
		if (fragData.anchors.size() == 1 && fragData.anchors.front().input.flagPrimary == InputFlag::newInput)
		{
			fragData.anchors.pop_back();
			fragData.anchors.shrink_to_fit();
		}

		if (vortex.constantRingCount)
		{
			fragData.resetAnchors();
			glm::vec3 dragDir = glm::normalize(glm::vec3(
				point.x - fragData.transform.origin.x,
				point.y - fragData.transform.origin.y,
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
						fragData.anchors.push_back(
							FragmentAnchor(anchorIDCount, fragPos, fragDir,
								1.0f,
								HandleType::linear, false, fragPos,
								HandleType::linear, false, fragPos,
								HandleRel::independent,
								data.inputEvents.back()));
					}
					fragData.anchors.at(fragData.anchors.size() - vortex.ringDensity).input.flagPrimary = InputFlag::point;
				}
			}
			fragData.anchors.at(fragData.anchors.size() - ((int)vortex.rings * vortex.ringDensity)).input.flagPrimary = InputFlag::point;
		}
	}

	return InputHandlerFlag::allowPress_updateCursor;
}
InputHandlerFlag In_Vortex::click(Application* sender, Input dat)
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
		}
		// A new set of anchors is started
		else
		{
			vortex = *owner.get()->getVortex();
			glm::vec3 point = glm::vec3(0.0f);
			glm::vec3 dir = DEFAULT_DIR;
			resetInput(sender, dat, point, dir);
			splineData.transform.origin = point;
			if (controlScheme == TSetType::continuous)
			{
				// Add the first anchor point, there is no direction yet, so a manual wait will be placed on the
				// anchor to count off 1 more anchor before rendering.
				continuous.anchorSpacing = 140.0f;
				splineData.anchors.push_back(
					FragmentAnchor(anchorIDCount, point, dir,
						1.0f,
						HandleType::linear, false, point,
						HandleType::linear, false, point,
						HandleRel::independent,
						data.inputEvents.back()));
				splineData.anchors.back().input.flagPrimary = InputFlag::newInput;
				splineIDCount++;
				splineData.anchors.back().wait = 1;
			}
			else if (controlScheme == TSetType::drag)
			{
				// Place a single vertex in the array in order to flagPrimary a new input
				drag.enableExtrusion = false;
				drag.initialExtrusion = 0.0f;
				startExtrude = 0.0f;
				fragData.anchors.push_back(
					FragmentAnchor(anchorIDCount, point, dir,
						0.0f,
						HandleType::linear, false, point,
						HandleType::linear, false, point,
						HandleRel::independent,
						data.inputEvents.back()));
				fragData.anchors.back().input.flagPrimary = InputFlag::newInput;
				anchorIDCount++;
			}
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