#include "../../include/methods/inputMethods/In_Spline.h"
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

void In_Spline::beginRecording()
{
	this->isRecording = true;
	std::cout << "IN_SPLINE::RECORDING::STARTED" << std::endl;
	if (splineActive) { activeSpline.reset(); }
}
void In_Spline::stopRecording()
{
	this->isRecording = false;
	std::cout << "IN_SPLINE::RECORDING::STOPPED::ACTIVE-SPLINE-SIZE= " << (int)activeSpline.anchors.size() << std::endl;
}

InputHandlerFlag In_Spline::move(Application* sender, Input dat)
{
	//std::cout << "MethodType::in_draw::MOVE" << std::endl;
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 dir = glm::vec3(0.0f);

	if (isRecording)
	{
		dir = glm::normalize(glm::vec3(
			pos.x - activeSpline.anchors.back().pos.x,
			pos.y - activeSpline.anchors.back().pos.y,
			0.0f));
		activeSpline.activeModKey = dat.modKey;
		bool isNew = (activeSpline.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
		if (controlScheme == TSetType::continuous)
		{
			if (!continuousDraw(sender, dat, &continuous, &smoothing, &activeSpline, continuous.trueSpacing, pos, dir)) { return InputHandlerFlag::wait; }
		}
		anchorIDCount++;
		activeSpline.anchors.push_back(FragmentAnchor(anchorIDCount, pos, dir,
			1.0f,
			HandleType::linear, false, pos,
			HandleType::linear, false, pos,
			HandleRel::independent,
			dat));
		fragData.anchors.push_back(FragmentAnchor(anchorIDCount, pos, dir,
			1.0f,
			HandleType::linear, false, pos,
			HandleType::linear, false, pos,
			HandleRel::independent,
			dat));

		if (isNew)
		{
			activeSpline.anchors.front().input.flagPrimary = InputFlag::null;
			fragData.anchors.front().input.flagPrimary = InputFlag::null;
		}
	}
	// Movement for regular input
	else
	{
		if (splineActive)
		{
			glm::vec3 splineDir = glm::normalize(glm::vec3(
				pos.x - splineData.anchors.back().pos.x,
				pos.y - splineData.anchors.back().pos.y,
				0.0f));

			if (controlScheme == TSetType::continuous)
			{
				if (!continuousDraw(sender, dat, &continuous, nullptr, &splineData, continuous.anchorSpacing, pos, splineDir)) { return InputHandlerFlag::wait; }
			}
			bool isNew = (splineData.anchors.front().input.flagPrimary == InputFlag::newInput);
			splineIDCount++;
			splineData.anchors.push_back(
				FragmentAnchor(anchorIDCount, pos, splineDir,
					1.0f,
					HandleType::linear, false, pos,
					HandleType::linear, false, pos,
					HandleRel::independent,
					data.inputEvents.back()));
			splineData.anchors.back().input.flagPrimary = InputFlag::null;
			splineData.anchors.back().input.flagSecondary = InputFlag::null;

			// Create Spline

			for (int i = 0; i < activeSpline.anchors.size(); i++)
			{
				anchorIDCount++;
				fragData.anchors.push_back(
					FragmentAnchor(anchorIDCount, pos + activeSpline.anchors[i].pos, activeSpline.anchors[i].dir,
						1.0f,
						HandleType::linear, false, pos + activeSpline.anchors[i].pos,
						HandleType::linear, false, pos + activeSpline.anchors[i].pos,
						HandleRel::independent,
						activeSpline.anchors[i].input));
				fragData.anchors.back().input.flagPrimary = InputFlag::null;
				fragData.anchors.back().input.flagSecondary = InputFlag::null;
			}
			fragData.anchors.at(fragData.anchors.size() - activeSpline.anchors.size()).input.flagSecondary = InputFlag::point;

			// Update Flags
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
		else
		{
			dir = glm::normalize(glm::vec3(
				pos.x - fragData.anchors.back().pos.x,
				pos.y - fragData.anchors.back().pos.y,
				0.0f));
			fragData.activeModKey = dat.modKey;
			bool isNew = (fragData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
			if (controlScheme == TSetType::continuous)
			{
				if (!continuousDraw(sender, dat, &continuous, &smoothing, &fragData, continuous.trueSpacing, pos, dir)) { return InputHandlerFlag::wait; }
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
		}
	}
	
	return InputHandlerFlag::allowPress_updateCursor;
}
InputHandlerFlag In_Spline::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
	{
		if (isRecording)
		{
			// Get new copies of the tool settings
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
			// Reset the activeSpline
			activeSpline.reset();
			activeSpline.centerAboutOrigin = activeSpline.connectEnds = activeSpline.constantSize = false;
			activeSpline.startTime = (float)data.start.time;
			// Set the upper left corner for the entity 
			glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
			activeSpline.transform.origin = origin;
			// Add the first anchor point, there is no direction yet, so a manual wait will be placed on the
			// anchor to count off 1 more anchor before rendering.
			anchorIDCount = splineIDCount = 0;
			activeSpline.anchors.push_back(FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				1.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				dat));
			activeSpline.anchors.back().wait = 1;
			activeSpline.anchors.back().input.flagPrimary = InputFlag::newInput;
			activeSpline.anchors.back().input.flagSecondary = InputFlag::newRecording;

			// Add the spline to fragData for previewing
			fragData.reset();
			fragData.centerAboutOrigin = fragData.connectEnds = fragData.constantSize = false;
			fragData.startTime = (float)data.start.time;
			// Set the upper left corner for the entity 
			fragData.transform.origin = origin;
			// Add the first anchor point, there is no direction yet, so a manual wait will be placed on the
			// anchor to count off 1 more anchor before rendering.
			fragData.anchors.push_back(FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				1.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				dat));
			fragData.anchors.back().wait = 1;
			fragData.anchors.back().input.flagPrimary = InputFlag::newInput;
			fragData.anchors.back().input.flagSecondary = InputFlag::newRecording;
			wasHandled = InputHandlerFlag::allowPress_updateCursor;
		}
		////////////////////////////////////////////////////////////////
		// If not recording, Check for active spline, without an active spline, default to draw-like behavior
		////////////////////////////////////////////////////////////////
		else
		{
			// Handling for when not recording and active spline is ready to render
			if (splineActive && activeSpline.anchors.size() != 0)
			{
				switch (controlScheme)
				{
				case TSetType::continuous:
				default:
					continuous = *owner.get()->getContinuousControl();
				}
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
						0.0f,
						HandleType::linear, false, origin,
						HandleType::linear, false, origin,
						HandleRel::independent,
						data.inputEvents.back()));
				splineData.anchors.back().input.flagPrimary = InputFlag::newInput;
				splineData.anchors.back().wait = 1;
				wasHandled = InputHandlerFlag::allowPress_updateCursor;
			}
			////////////////////////////////////////////////////////////////
			// Handling for when not recording and no spline is active, fall back to draw-like behavior
			////////////////////////////////////////////////////////////////
			else
			{
				// Clear any constraint variables, these should not remain constant between different
			// press events of the same stroke
				continuous.clearConstraint();
				fragData.activeModKey = dat.modKey;
				// Scenario in which 'Shift' is used to connect the end of the anchors to the next one
				// Keep previous anchor data and simply continue adding to it
				if (dat.modKey == InputModKey::shift)
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
						1.0f,
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
			}
		}
		std::cout << "IN_SPLINE::CLICK::ALLOW_PRESS" << std::endl;
	}
	else if (dat.action == InputAction::release && dat.button == InputMouseButton::left)
	{
		data.end = dat;
		if (fragData.anchors.size() == 1) { fragData.anchors.back().input.flagPrimary = InputFlag::null; }
		fragData.activeModKey = dat.modKey;
		fragData.endTime = (float)data.end.time;
		if (isRecording && activeSpline.anchors.size() != 0)
		{
			fragData.anchors.back().input.flagSecondary = InputFlag::endRecording;
			activeSpline.anchors.back().input.flagSecondary = InputFlag::endRecording;
			activeSpline.activeModKey = dat.modKey;
			activeSpline.endTime = (float)data.end.time;
			activeSpline.originToVertices();
			activeSpline.centerVertsToOrigin();
			stopRecording();
			if (activeSpline.anchors.size() != 0) { splineActive = true; }
		}
		wasHandled = InputHandlerFlag::release;
		std::cout << "IN_SPLINE::CLICK::ALLOW_RELEASE" << std::endl;
	}
	else
	{
		wasHandled = InputHandlerFlag::noSignal;
		std::cout << "IN_SPLINE::CLICK::NOSIGNAL" << std::endl;
	}
	std::cout << "IN_SPLINE::CLICK::TIME=" << dat.time << "::TYPE=" << keybindToString(Keybind(InputKey::unknown, data.inputModKey)) << std::endl;
	return wasHandled;
}

InputHandlerFlag In_Spline::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	return InputHandlerFlag::noSignal;
}

void In_Spline::initializeVertices(glm::vec3* pos, glm::vec3* dir, Input* dat,
	int waitCountVertex, int waitCountSpline, InputFlag vertexFlagSecondary, InputFlag splineFlagSecondary)
{

}

void In_Spline::addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{

}

void In_Spline::generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{

}

void In_Spline::generateCurve()
{

}

void In_Spline::newInput(Application* sender, Input dat)
{

}