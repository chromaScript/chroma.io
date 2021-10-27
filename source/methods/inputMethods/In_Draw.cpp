#include "../../include/methods/inputMethods/In_Draw.h"
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

InputHandlerFlag In_Draw::move(Application* sender, Input dat)
{
	//std::cout << "MethodType::in_draw::MOVE" << std::endl;
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 dir = glm::normalize(glm::vec3(
		pos.x - fragData.anchors.back().pos.x,
		pos.y - fragData.anchors.back().pos.y,
		0.0f));
	fragData.activeModKey = dat.modKey;
	
	bool isNew = (fragData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
	if (controlScheme == TSetType::continuous)
	{
		if (!continuousMove(sender, dat, &continuous, &smoothing, &fragData, continuous.trueSpacing, pos, dir)) { return InputHandlerFlag::wait; }
	}

	anchorIDCount++;
	fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pos, dir,
		1.0f,
		HandleType::linear, false, pos,
		HandleType::linear, false, pos,
		HandleRel::independent,
		dat));

	if (isNew)
	{
		fragData.anchors.front().input.flagPrimary = InputFlag::null;
	}

	return InputHandlerFlag::allowPress_updateCursor;
}
InputHandlerFlag In_Draw::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
	{
		// Clear any constraint variables, these should not remain constant between different
		// press events of the same stroke
		continuous.clearConstraint();
		fragData.activeModKey = dat.modKey;
		// Scenario in which 'Shift' is used to connect the end of the anchors to the next one
		// Keep previous anchor data and simply continue adding to it
		if (dat.modKey == InputModKey::shift && fragData.anchors.size() != 0)
		{
			// Copy new continuous data
			switch (controlScheme)
			{
			case TSetType::continuous:
			default:
				owner.get()->getContinuousControl()->updateTrueSpacing(owner, sender->getCanvasDimensions().x, sender->getCanvasDimensions().y);
				continuous.~TSet_ContinuousControl();
				continuous = *owner.get()->getContinuousControl();
				continuous.clearConstraint();
			}
			// Reset the end-input && end-time
			data.end.resetAll();
			fragData.endTime = 0.0;
			// Push back a new data point
			data.inputEvents.push_back(dat);
			// Determine the anchor position and direction
			glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
			glm::vec3 dir = glm::normalize(glm::vec3(
				pos.x - fragData.anchors.back().pos.x,
				pos.y - fragData.anchors.back().pos.y,
				0.0f));
			// Insert new anchor, set dirInterpFactor to 0.0f so that the stroke will hard-snap
			// the shards to the new direction rather than smoothly interpolate the change
			// Note: Must revisit this section later to inject adjustment of pressure via input settings. Interpolate along the pressure graph
			anchorIDCount++;
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pos, dir,
				0.0f,
				HandleType::linear, false, pos,
				HandleType::linear, false, pos,
				HandleRel::independent,
				dat));
			wasHandled = InputHandlerFlag::continueInput;
		}
		// A new set of anchors is started
		else
		{
			// Get new copies of the tool settings
			// Once a new input has begun, the order has been 'sent' and whatever
			// settings were in place at that time is what the output will
			// use to render the input. After this point, the verticies and tool settings
			// for the output fragment may only be changed using vertex/anchor manipulation tools
			if (owner.get()->checkInterestMask(TSetType::image)) { image = *owner.get()->getImage(); }
			else { image.isEnabled = false; }
			if (owner.get()->checkInterestMask(TSetType::smoothing)) { smoothing = *owner.get()->getSmoothing(); }
			else { smoothing.isEnabled = false; }

			switch (controlScheme)
			{
			case TSetType::continuous:
			default:
				owner.get()->getContinuousControl()->updateTrueSpacing(owner, sender->getCanvasDimensions().x, sender->getCanvasDimensions().y);
				continuous = *owner.get()->getContinuousControl();
			}
			
			// Reset the data
			data.reset();
			data.start = dat;
			data.inputModKey = dat.modKey;
			// Push back a new data point
			data.inputEvents.push_back(dat);
			// Reset the fragData
			fragData.reset();
			fragData.centerAboutOrigin = fragData.connectEnds = fragData.constantSize = false;
			fragData.startTime = (float)data.start.time;
			// Set the upper left corner for the entity 
			glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
			fragData.transform.origin = origin;
			// Add the first anchor point, there is no direction yet, so a manual wait will be placed on the
			// anchor to count off 1 more anchor before rendering.
			anchorIDCount = 0;
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR, 
				1.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				dat));
			fragData.anchors.back().wait = 1;
			fragData.anchors.back().input.flagPrimary = InputFlag::newInput;
			wasHandled = InputHandlerFlag::allowPress_updateCursor;
		}
		//std::cout << "MethodType::in_draw::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		data.end = dat;
		if (fragData.anchors.size() == 1) { fragData.anchors.back().input.flagPrimary = InputFlag::null; }
		fragData.activeModKey = dat.modKey;
		fragData.endTime = (float)data.end.time;
		wasHandled = InputHandlerFlag::release;
		//std::cout << "MethodType::in_draw::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
		//std::cout << "MethodType::in_draw::CLICK::NOSIGNAL" << std::endl;
	}
	//std::cout << "MethodType::in_draw::CLICK::TIME=" << dat.time << "::TYPE=" << keybindToString(Keybind(InputKey::unknown, data.inputModKey)) << std::endl;
	return wasHandled;
}