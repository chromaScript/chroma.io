#include "../include/methods/In_ShapeField.h"
#include "../include/methods/InputMethod.h"
#include "../include/ToolSettings.h"
#include "../include/toolSettings/ToolSettings_Forward.h"
#include "../include/Tool.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
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

int In_ShapeField::move(Application* sender, MouseEvent dat)
{
	glm::vec3 cursorPos = sender->pickScreenCoord(dat.x, dat.y);

	if (controlScheme == TSetType::onepoint)
	{
		if (!onePointMove(sender, dat, &onePoint, cursorPos)) { return INPUT_WAIT; }
	}
	else if (controlScheme == TSetType::drag)
	{
		if (!dragMove(sender, dat, &drag, cursorPos)) { return INPUT_WAIT; }
	}
	
	// Kick empty density input
	//if (field.random_densityMin <= 0) { return INPUT_NOSIGNAL; }
	if (false) { return INPUT_NOSIGNAL; } // Overriding while working on settings

	// Clear the NEW TAG and erase the temporary holding vertex
	if (fragData.anchors.size() == 1 && fragData.anchors.front().flag == FLAG_NEW_INPUT)
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
		glm::vec3 dir = glm::vec3(1, 0, 0);
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

			fragData.anchors.push_back(FragmentAnchor(FLAG_NULL, anchorIDCount, pos, dir, 0.0f,
				FHANDLE_LINEAR, false, pos,
				FHANDLE_LINEAR, false, pos,
				FHANDLE_REL_INDEPENDENT,
				pressure, data.inputEvents.back().tiltx,
				data.inputEvents.back().tilty, data.inputEvents.back().rotation,
				(float)data.start.time, sender->getMouseVelocity(), 1.0f));
		}
		//std::cout << anchorIDCount << std::endl;
		//std::cout << vec3VecToString({ fragData.transform.bounds.p4 }) << std::endl;
	}
	else
	{
		// Clear the existing fragData
		//fragData.resetAnchors();
	}

	return INPUT_ALLOW_PRESS_UPDATECURSOR;
}

int In_ShapeField::click(Application* sender, MouseEvent dat)
{
	int wasHandled = INPUT_NOSIGNAL;
	if (dat.action == INPUT_PRESS && dat.button == UI_MOUSE_LEFT)
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
				(dat.modKey != INPUT_MOD_NONE && dat.modKey == onePoint.sizeAboutOriginKey) ? true : false;
			iniAngle = onePoint.initialRotation;
			startExtrude = drag.initialExtrusion;
			break;
		case TSetType::drag:
			drag = *owner.get()->getDragControl();
			drag.enableExtrusion = true;
			fragData.centerAboutOrigin =
				(dat.modKey != INPUT_MOD_NONE && dat.modKey == drag.sizeAboutOriginKey) ? true : false;
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

		// Place a single vertex in the array in order to flag a new input
		fragData.anchors.push_back(FragmentAnchor(FLAG_NEW_INPUT, anchorIDCount, origin, glm::vec3(0, 0, 0), 0.0f,
			FHANDLE_LINEAR, false, origin,
			FHANDLE_LINEAR, false, origin,
			FHANDLE_REL_INDEPENDENT,
			data.inputEvents.back().pressure, data.inputEvents.back().tiltx,
			data.inputEvents.back().tilty, data.inputEvents.back().rotation,
			(float)data.start.time, sender->getMouseVelocity(), 1.0f));
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
	//return wasHandled;

	return wasHandled;
}