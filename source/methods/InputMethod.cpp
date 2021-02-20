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
#include <gtx/rotate_vector.hpp>

#include <iostream>
#include <algorithm>
#include <iostream>

bool InputMethod::continuousMove(Application* sender, MouseEvent dat, 
	TSet_ContinuousControl* continuousControl, TSet_Smoothing* smoothing,
	bool useSplineData, glm::vec3& outPos, glm::vec3 &outDir)
{
	VertexData* target = (useSplineData) ? &splineData : &fragData;
	
	// Kick bad-calls
	if (target->anchors.size() == 0) { return false; }
	
	
	// Add the input data
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }

	int i;
	float length, angle;

	// Do setup of variables that all logic branches need
	i = (int)target->anchors.size() - 1;
	while (i >= 0 && (std::isnan(outDir.x) || std::isnan(outDir.y) || std::isnan(outDir.z)))
	{
		outDir = target->anchors[(size_t)i].dir;
		i--;
	}
	length = glm::length(outPos - target->anchors.back().pos);
	angle = glm::degrees(atan2(outDir.x, outDir.y));

	// Determine if to set a constraint angle

	// Add both the constraint angle and it's opposite angle in order to snap to either direction
	// along the constraint axis when it's enabled
	if (continuousControl->isEnabled && !continuousControl->activated && length > ROOT2 * continuousControl->constraintLengthThreshold)
	{
		std::vector<float> constraintAngles;
		std::vector<float> constraintDifferences;
		std::vector<int> constraintKeys;
		bool doConstraintA = continuousControl->enableConstrainA && dat.modKey == continuousControl->constrainA_modKey;
		if (doConstraintA)
		{
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainA_angle + 360));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainA_angle + 180));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintKeys.push_back(continuousControl->constrainA_modKey);
			constraintKeys.push_back(continuousControl->constrainA_modKey);
		}
		bool doConstraintB = continuousControl->enableConstrainB && dat.modKey == continuousControl->constrainB_modKey;
		if (doConstraintB)
		{
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainB_angle + 360));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainB_angle + 180));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintKeys.push_back(continuousControl->constrainB_modKey);
			constraintKeys.push_back(continuousControl->constrainB_modKey);
		}
		bool doConstraintC = continuousControl->enableConstrainC && dat.modKey == continuousControl->constrainC_modKey;
		if (doConstraintC)
		{
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainC_angle + 360));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainC_angle + 180));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintKeys.push_back(continuousControl->constrainC_modKey);
			constraintKeys.push_back(continuousControl->constrainC_modKey);
		}
		bool doConstraintD = continuousControl->enableConstrainD && dat.modKey == continuousControl->constrainD_modKey;
		if (doConstraintD)
		{
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainD_angle + 360));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintAngles.push_back(clampAngle_360(continuousControl->constrainD_angle + 180));
			constraintDifferences.push_back(abs(constraintAngles.back() - (clampAngle_360(angle + 360))));
			constraintKeys.push_back(continuousControl->constrainD_modKey);
			constraintKeys.push_back(continuousControl->constrainD_modKey);
		}
		if (constraintAngles.size() != 0)
		{
			// Use an iterator max_element operation to get the index (distance) to get the desired constraint angle
			std::vector<float>::iterator lowIndex = std::min_element(constraintDifferences.begin(), constraintDifferences.end());
			float lowValue = constraintDifferences[std::distance(constraintDifferences.begin(), lowIndex)];
			std::vector<float>::iterator highIndex = std::max_element(constraintDifferences.begin(), constraintDifferences.end());
			float highValue = 360 - constraintDifferences[std::distance(constraintDifferences.begin(), highIndex)];
			float selectAngle = constraintAngles[std::distance(
				constraintDifferences.begin(), (lowValue <= highValue) ? lowIndex : highIndex)];
			continuousControl->activeKey = constraintKeys[std::distance(
				constraintDifferences.begin(), (lowValue <= highValue) ? lowIndex : highIndex)];
			continuousControl->activeAngle = clampAngle_180(selectAngle);
			continuousControl->origin = outPos;
			continuousControl->dir = 
				glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(continuousControl->activeAngle + 90))) * 
				glm::vec3(1.0f, 0.0f, 0.0f);
			continuousControl->perpendicular = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)))* continuousControl->dir;
			glm::vec3 constrainP2 = continuousControl->origin + (continuousControl->dir * 100.0f);
			continuousControl->line = glm::vec4(continuousControl->origin.x, continuousControl->origin.y, constrainP2.x, constrainP2.y);
			continuousControl->activated = true;
			std::cout << continuousControl->activeAngle << std::endl;
		}
	}


	// Return and do not add an anchor if the length is less than the diagonal of 1 pixel * anchorSpacing
	if (length < ROOT2 * continuousControl->anchorSpacing) { return false; }
	if (continuousControl->activated)
	{
		glm::vec3 perpLineP2 = outPos + (continuousControl->perpendicular * 100.0f);
		glm::vec4 perpLine = glm::vec4(outPos.x, outPos.y, perpLineP2.x, perpLineP2.y);
		outPos = glm::vec3(lineIntersect2D(continuousControl->line, perpLine), 0.0f);
		if (dat.modKey != continuousControl->activeKey)
		{
			continuousControl->clearConstraint();
		}
		// Must recheck the length again after adjustment, otherwise the input gets sluggish as it piles
		// up anchors on top of eachother
		length = glm::length(outPos - target->anchors.back().pos);
		if (length < ROOT2 * continuousControl->anchorSpacing) { return false; }
	}
	else
	{
		// Adjust the direction on a wait-flag
		if (target->anchors.size() == 1 && target->anchors.front().wait > 0)
		{
			target->anchors.front().dir = outDir;
			target->anchors.front().wait = 0;
		}
		else
		{
			// Apply smoothing to the direction value as applicable
			glm::vec3 lastDir = target->anchors.back().dir;
			float angleChange = angle - glm::degrees(atan2(lastDir.x, lastDir.y));
			if (smoothing != nullptr && smoothing->isEnabled && smoothing->smoothDirection && angleChange >= smoothing->directionThreshold)
			{
				//std::cout << angleChange << std::endl;
				outDir = lerpDir(outDir, lastDir, smoothing->directionFactor);
			}
		}
	}
	// Clear the NEW TAG
	//target->anchors.front().flag = FLAG_NULL;
	return true;
}

bool InputMethod::dragMove(Application* sender, MouseEvent dat, 
	TSet_DragControl* dragControl, glm::vec3& cursorPos)
{
	// Pause the input if there hasn't been input movement
	glm::dvec2 lastInput = glm::dvec2(data.inputEvents.back().x, data.inputEvents.back().y);
	float moveTest = glm::length(glm::dvec2(dat.x, dat.y) - lastInput);
	if (moveTest < ROOT2) { return false; }
	// Add the input data
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }
	
	// Clear FLAG_NEW_INPUT
	if (fragData.anchors.size() != 0) { fragData.anchors.front().flag = FLAG_NULL; }
	
	// Determine the input mode and settings
	fragData.centerAboutOrigin =
		(sender->getKeyState(dragControl->sizeAboutOriginKey)) ? true : false;
	bool extrudeMode = (sender->getKeyState(dragControl->extrudeModeKey) && dragControl->enableExtrusion) ? true : false;
	bool moveMode = (sender->getKeyState(dragControl->moveOriginModeKey)) ? true : false;
	bool snapMode = (sender->getKeyState(dragControl->snapModeKey)) ? true : false;
	
	// Set up the data
	glm::vec3 lastPos = sender->pickScreenCoord(lastInput.x, lastInput.y);
	glm::vec3 dir = glm::normalize(glm::vec3(
		fragData.transform.origin.x - cursorPos.x,
		fragData.transform.origin.y - cursorPos.y,
		0.0f));

	if (moveMode && !extrudeMode)
	{
		glm::vec3 movement = cursorPos - lastPos;
		fragData.transform.origin += movement;
		// Calculate for if centerAboutOrigin is on
		fragData.transform.bounds.p1 = fragData.transform.origin;
		fragData.transform.bounds.p4 = lastCursor = cursorPos;
	}
	else if (extrudeMode && !moveMode)
	{
		dir = glm::normalize(glm::vec3(
			fragData.transform.origin.x - lastCursor.x,
			fragData.transform.origin.y - lastCursor.y,
			0.0f));
		fragData.transform.bounds.p4 = lastCursor;
		startExtrude = distancePointLine2D(glm::dvec2(cursorPos), glm::dvec4(
			fragData.transform.origin.x,
			fragData.transform.origin.y,
			fragData.transform.bounds.p4.x,
			fragData.transform.bounds.p4.y));
	}
	// Otherwise apply length/rotation changes
	else
	{
		//cursorPos = fragData.transform.origin + (rotation * (cursorPos - fragData.transform.origin));
		if (snapMode)
		{
			// Warning: Unfinished area of program!
		}
		fragData.transform.bounds.p4 = lastCursor = cursorPos;
	}

	// Calculate for if centerAboutOrigin is on
	fragData.transform.bounds.p1 = (fragData.centerAboutOrigin) ?
		fragData.transform.origin + (dir * glm::length(fragData.transform.bounds.p4 - fragData.transform.origin)) :
		fragData.transform.origin;
	

	// Calculate the remaining bounds box points p2 and p3 and then recalculate p1 and p4 for extrusion
	// The points do not receive rotation transformation here. The transform.roll is bundled with the box so that
	// any rasterizer receiving the box can apply the transform virtually itself before rendering
	glm::vec3 normal = glm::quat(glm::vec3(0, 0, glm::radians(-90.0f))) * dir;
	fragData.transform.bounds.p2 = fragData.transform.bounds.p1 + (normal * startExtrude);
	fragData.transform.bounds.p3 = fragData.transform.bounds.p4 + (normal * startExtrude);
	fragData.transform.bounds.p1 = fragData.transform.bounds.p1 + (normal * -startExtrude);
	fragData.transform.bounds.p4 = fragData.transform.bounds.p4 + (normal * -startExtrude);
	// Unrotate all the verts and then set the roll value before exiting
	glm::quat unRotate = glm::quat(glm::vec3(0, 0, atan2(dir.x, dir.y)));
	fragData.transform.bounds.p1 = fragData.transform.origin + (unRotate * (fragData.transform.bounds.p1 - fragData.transform.origin));
	fragData.transform.bounds.p2 = fragData.transform.origin + (unRotate * (fragData.transform.bounds.p2 - fragData.transform.origin));
	fragData.transform.bounds.p3 = fragData.transform.origin + (unRotate * (fragData.transform.bounds.p3 - fragData.transform.origin));
	fragData.transform.bounds.p4 = fragData.transform.origin + (unRotate * (fragData.transform.bounds.p4 - fragData.transform.origin));
	fragData.transform.roll = -glm::degrees(atan2(dir.x, dir.y));
	return true;
}

bool InputMethod::onePointMove(Application* sender, MouseEvent dat, 
	TSet_OnePointControl* onePointControl, glm::vec3 &cursorPos)
{
	// Pause the input if there hasn't been input movement
	glm::dvec2 lastInput = glm::dvec2(data.inputEvents.back().x, data.inputEvents.back().y);
	float moveTest = glm::length(glm::dvec2(dat.x, dat.y) - lastInput);
	if (moveTest < ROOT2) { return false; }
	// Add the input data
	if (data.inputEvents.size() < maxBufferLength) { data.inputEvents.push_back(dat); }
	else { data.inputEvents.erase(data.inputEvents.begin()); data.inputEvents.push_back(dat); }

	// Clear FLAG_NEW_INPUT
	if (fragData.anchors.size() != 0) { fragData.anchors.front().flag = FLAG_NULL; }

	// Determine the input mode and settings
	fragData.centerAboutOrigin =
		(sender->getKeyState(onePointControl->sizeAboutOriginKey)) ? true : false;
	bool rotateMode = (sender->getKeyState(onePointControl->rotateModeKey)) ? true : false;
	bool moveMode = (sender->getKeyState(onePointControl->moveOriginModeKey)) ? true : false;
	bool constrainRatio = (sender->getKeyState(onePointControl->constrainRatioKey)) ? true : false;
	// Set up the data
	glm::vec3 lastPos = sender->pickScreenCoord(lastInput.x, lastInput.y);

	// Apply Movement if enabled. Cannot apply move and rotate at the same time
	if (moveMode && !rotateMode)
	{
		glm::vec3 movement = cursorPos - lastPos;
		fragData.transform.bounds.p1 += movement;
		fragData.transform.bounds.p3 += movement;
		fragData.transform.origin += movement;
	}
	// Apply rotation if enabled.
	else if (rotateMode && !moveMode)
	{
		glm::vec2 dir = glm::normalize(fragData.transform.origin - cursorPos);
		float angleIn = glm::degrees(atan2(dir.x, dir.y));
		float angleOut = 0.0f;
		if (startAngle <= -359.0f)
		{
			startAngle = clampAngle_180(angleIn + onePointControl->initialRotation);
			angleOut = 0.0f;
		}
		else
		{
			angleOut = startAngle - angleIn;
		}
		fragData.transform.roll = clampAngle_180(angleOut);
		if (constrainRatio)
		{
			float count = floor(fragData.transform.roll / onePointControl->constrainAngleIncrement);
			float remainder = fragData.transform.roll - (count * onePointControl->constrainAngleIncrement);
			fragData.transform.roll =
				(remainder < onePointControl->constrainAngleIncrement / 2.0f) ?
				count * onePointControl->constrainAngleIncrement : (count + 1) * onePointControl->constrainAngleIncrement;
		}
	}
	// Otherwise apply size changes
	else
	{
		glm::quat rotation = glm::quat(glm::vec3(0, 0, glm::radians(-fragData.transform.roll)));
		cursorPos = fragData.transform.origin + (rotation * (cursorPos - fragData.transform.origin));
		if (constrainRatio)
		{
			float x = cursorPos.x - fragData.transform.origin.x;
			float y = cursorPos.y - fragData.transform.origin.y;
			float s = (x * y < 0.0f) ? -1.0f : 1.0f;
			float a = (abs(x) > abs(y)) ? abs(y) : abs(x);
			a *= x / abs(x);
			fragData.transform.bounds.p3 = fragData.transform.origin + glm::vec3(a, a * s, 0);
		}
		else
		{
			fragData.transform.bounds.p3 = cursorPos;
		}
		// Calculate for if centerAboutOrigin is on
		fragData.transform.bounds.p1 = (fragData.centerAboutOrigin) ?
			fragData.transform.origin - (fragData.transform.bounds.p3 - fragData.transform.origin) :
			fragData.transform.origin;
	}
	// Calculate the remaining bounds box points p2 and p4
	// The points do not receive rotation transformation here. The transform.roll is bundled with the box so that
	// any rasterizer receiving the box can apply the transform virtually itself before rendering
	fragData.transform.bounds.p2 = fragData.transform.bounds.p1 + glm::vec3(fragData.transform.bounds.p3.x - fragData.transform.bounds.p1.x, 0.0f, 0.0f);
	fragData.transform.bounds.p4 = fragData.transform.bounds.p1 + glm::vec3(0.0f, fragData.transform.bounds.p3.y - fragData.transform.bounds.p1.y, 0.0f);

	return true;
}

void InputMethod::resetInput(Application* sender, MouseEvent dat, glm::vec3& point, glm::vec3& dir)
{
	// Reset measurements
	startAngle = -360.0f;
	startExtrude = 0.0f;
	// Get new copies of the control scheme settings
	switch (controlScheme)
	{
	case TSetType::continuous:
		continuous = *owner.get()->getContinuousControl();
		fragData.transform.roll = 0.0f;
		startExtrude = 0.0f;
		break;
	case TSetType::drag:
		drag = *owner.get()->getDragControl();
		fragData.centerAboutOrigin =
			(dat.modKey != INPUT_MOD_NONE && dat.modKey == drag.sizeAboutOriginKey) ? true : false;
		fragData.transform.roll = 0.0f;
		startExtrude = drag.initialExtrusion;
		break;
	case TSetType::onepoint:
		continuous = *owner.get()->getContinuousControl();
		fragData.centerAboutOrigin =
			(dat.modKey != INPUT_MOD_NONE && dat.modKey == onePoint.sizeAboutOriginKey) ? true : false;
		fragData.transform.roll = onePoint.initialRotation;
		startExtrude = 0.0f;
		break;
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
	point = sender->pickScreenCoord(dat.x, dat.y);
	dir = glm::vec3(0.0f);
	// Set Basic Transform Data
	fragData.transform.roll = startAngle;
	fragData.transform.origin = point;
	fragData.transform.bounds = RectBounds(point);
	// Reset the ID Counters
	anchorIDCount = 0;
	splineIDCount = 0;
	inputCount = 0;
}