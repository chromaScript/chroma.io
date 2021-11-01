#include "../../include/methods/inputMethods/In_ShapeField.h"
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
#include <gtx/rotate_vector.hpp>

#include <iostream>
#include <algorithm>
#include <iostream>
#include <random>
#include <chrono>

InputHandlerFlag In_ShapeField::move(Application* sender, Input dat)
{
	glm::vec3 cursorPos = sender->pickScreenCoord(dat.x, dat.y);

	if (controlScheme == TSetType::onepoint)
	{
		if (!onePointMove(sender, dat, &onePoint, cursorPos)) { return InputHandlerFlag::wait; }
	}
	else if (controlScheme == TSetType::drag)
	{
		if (!dragMove(sender, dat, &drag, cursorPos)) { return InputHandlerFlag::wait; }
	}
	
	// Kick empty density input
	//if (field.random_densityMin <= 0) { return INPUT_NOSIGNAL; }
	if (false) { return InputHandlerFlag::noSignal; } // Overriding while working on settings

	// Clear the NEW TAG and erase the temporary holding vertex
	if (fragData.anchors.size() == 1 && fragData.anchors.front().input.flagPrimary == InputFlag::newInput)
	{
		fragData.anchors.pop_back();
		fragData.anchors.shrink_to_fit();
	}
	
	glm::vec2 size = glm::vec2(
		fragData.transform.bounds.p3.x - fragData.transform.bounds.p1.x, 
		fragData.transform.bounds.p3.y - fragData.transform.bounds.p1.y);
	
	anchorIDCount++;

	std::default_random_engine generator;

	// Generate verticies when field density is not constant, density = vertCount
	//if (!field.constantDensity)
	if (true) // Overriding while working on settings
	{
		
		
		std::uniform_real_distribution<double> distribution(0.0, 1.0);
		glm::vec3 pos = glm::vec3(0);
		glm::vec3 dir = DEFAULT_DIR;
		float pressure = 1.0f;
		float dist1 = 0.0f;
		float dist2 = 0.0f;
		fragData.resetAnchors();
		//for (int k = 0; k < (int)field.density; k++)
		for (int k = 0; k < 200; k++) // Overriding while working on settings
		{
			if (!field.constantSeed) { generator.seed((unsigned int)std::chrono::system_clock::now().time_since_epoch().count()); }
			else { generator.seed(field.savedSeed); }
			dist1 = (float)distribution(generator);
			if (!field.constantSeed) { generator.seed((unsigned int)std::chrono::system_clock::now().time_since_epoch().count() * 2); }
			else { generator.seed(field.savedSeed); }
			dist2 = (float)distribution(generator);

			pos = glm::vec3(
				fragData.transform.bounds.p1.x + (dist1 * size.x), 
				fragData.transform.bounds.p1.y + (dist2 * size.y), 
				0.0f);
			glm::quat rotation = glm::quat(glm::vec3(0, 0, glm::radians(fragData.transform.roll)));
			pos = fragData.transform.origin + (rotation * (pos - fragData.transform.origin));
			if (field.randomizeRotation)
			{
				//glm::mat4 matrix = glm::mat4(1.0f);
				//dir = glm::rotate(dir, glm::radians((float)distribution(generator) * 360.0f), glm::vec3(0, 0, 1));
			}
			pressure = 1.0f - ((dist1 * field.pressureXPosInfluence * 
				(1 - (field.pressureYPosInfluence / 2))) + (dist2 * field.pressureYPosInfluence * (1 - (field.pressureXPosInfluence / 2))));

			fragData.anchors.push_back(
				FragmentAnchor(anchorIDCount, pos, dir,
					0.0f,
					HandleType::linear, false, pos,
					HandleType::linear, false, pos,
					HandleRel::independent,
					data.inputEvents.back()));
			fragData.anchors.back().input.pressure = pressure;
			fragData.anchors.back().input.flagPrimary = InputFlag::null;
		}
		//std::cout << anchorIDCount << std::endl;
		//std::cout << vec3VecToString({ fragData.transform.bounds.p4 }) << std::endl;
	}
	else
	{
		// Clear the existing fragData
		//fragData.resetAnchors();
	}

	return InputHandlerFlag::allowPress_updateCursor;
}

InputHandlerFlag In_ShapeField::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (dat.action == InputAction::press && dat.button == InputMouseButton::left)
	{
		polygon = *owner.get()->getPolygon();
		field = *owner.get()->getField();
		if (field.constantSeed) 
		{ 
			field.savedSeed = std::chrono::system_clock::now().time_since_epoch().count();
		}
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
		//fragData.constantSize = (field.constantDensity) ? true : false;
		fragData.constantSize = (true) ? true : false; // Overriding while working on settings
		fragData.linearStream = false;
		fragData.connectEnds = true;
		// Switch on controlScheme
		float iniAngle = 0.0f;
		switch (controlScheme)
		{
		case TSetType::onepoint:
			onePoint = *owner.get()->getOnePointControl();
			fragData.centerAboutOrigin =
				(dat.modKey != InputModKey::none && compareModKey(onePoint.sizeAboutOriginKey, dat.modKey, false)) ? true : false;
			iniAngle = onePoint.initialRotation;
			startExtrude = drag.initialExtrusion;
			break;
		case TSetType::drag:
			drag = *owner.get()->getDragControl();
			drag.enableExtrusion = true;
			fragData.centerAboutOrigin =
				(dat.modKey != InputModKey::none && compareModKey(drag.sizeAboutOriginKey, dat.modKey, false)) ? true : false;
			iniAngle = 0.0f;
			startExtrude = drag.initialExtrusion;
			break;
		}
		
		fragData.startTime = (float)data.start.time;
		anchorIDCount = 0;
		// Set up the polygon
		// The origin will always be the locus of control. When centering/sizing about origin, the vertex
		// data is simply shifted negatively into the other 3 quadrants left/above the origin
		glm::vec3 origin = sender->pickScreenCoord(dat.x, dat.y);
		
		fragData.transform.roll = iniAngle;
		fragData.transform.origin = origin;
		fragData.transform.bounds = RectBounds(origin);

		// Place a single vertex in the array in order to flagPrimary a new input
		fragData.anchors.push_back(
			FragmentAnchor(anchorIDCount, origin, DEFAULT_DIR,
				0.0f,
				HandleType::linear, false, origin,
				HandleType::linear, false, origin,
				HandleRel::independent,
				data.inputEvents.back()));
		fragData.anchors.back().input.flagPrimary = InputFlag::newInput;
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
	//return wasHandled;

	return wasHandled;
}

InputHandlerFlag In_ShapeField::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	return InputHandlerFlag::noSignal;
}