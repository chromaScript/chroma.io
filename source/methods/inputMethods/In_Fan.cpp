#include "../../include/math/curve.h"
#include "../../include/methods/inputMethods/In_Fan.h"
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

#include <iostream>
#include <algorithm>
#include <iostream>

InputHandlerFlag In_Fan::move(Application* sender, Input dat)
{
	glm::vec3 pos = sender->pickScreenCoord(dat.x, dat.y);
	glm::vec3 dir = makeDir(splineData.anchors.back().pos, pos);
	fragData.activeModKey = splineData.activeModKey = dat.modKey;
	bool isNew = (splineData.anchors.front().input.flagPrimary == InputFlag::newInput) ? true : false;
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;

	if (controlScheme == TSetType::continuous)
	{
		wasHandled = continuousMove(sender, dat, &pos, &dir);
	}

	//clearFlagNew(isNew);
	return wasHandled;
}
InputHandlerFlag In_Fan::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (controlScheme == TSetType::continuous) 
	{
		wasHandled = continuousClick(sender, dat, 1, 1, InputFlag::null, InputFlag::null);
	}
	
	return wasHandled;
 }

InputHandlerFlag In_Fan::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (controlScheme == TSetType::continuous)
	{
		wasHandled = continuousKey(sender, dat, key, action, modKeys);
	}
	return wasHandled;
}

void In_Fan::initializeVertices(glm::vec3* pos, glm::vec3* dir, Input* dat,
	int waitCountVertex, int waitCountSpline, InputFlag vertexFlagSecondary, InputFlag splineFlagSecondary)
{
	fragData.anchors.push_back(FragmentAnchor(anchorIDCount, *pos, *dir, 1.0f, *dat));
	splineData.anchors.push_back(FragmentAnchor(splineIDCount, *pos, *dir, 1.0f, *dat));
	if (activeMode == TSetProp::line) {
		splineIDCount++;
		splineData.anchors.push_back(FragmentAnchor(splineIDCount, *pos, *dir, 1.0f, *dat));
	}
	fragData.anchors.back().wait = waitCountVertex;
	splineData.anchors.back().wait = waitCountSpline;
	fragData.anchors.front().input.flagSecondary = vertexFlagSecondary;
	splineData.anchors.front().input.flagPrimary = InputFlag::newInput;
	splineData.anchors.front().input.flagSecondary = splineFlagSecondary;
}

void In_Fan::addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{
	int count = 1;
	if (continuous.scatterConnections) {
		count = continuous.modulateCount(count, pos, &splineData.transform.origin, dir, dat, &anchorIDCount, &splineIDCount, &splineIDCount);
	}
	glm::vec3 splinePos = *pos;
	glm::vec3 splineDir = *dir;
	for (int i = 0; i < count; i++) {
		anchorIDCount++;
		splinePos = *pos;
		if (continuous.scatterConnections) {
			continuous.modulatePosition(splinePos, &image.tipSize, pos, &splineData.transform.origin, dir, dat, &anchorIDCount, &splineIDCount, &splineIDCount);
		}
		splineDir = makeDir(fragData.anchors.back().pos, splinePos);
		fragData.anchors.push_back(FragmentAnchor(anchorIDCount, splinePos, splineDir, 1.0f, *dat));
		//if (continuous.connectInterval > 1 && anchorIDCount % continuous.connectInterval == 0) {
		//	fragData.anchors.back().input.flagSecondary = InputFlag::point;
		//}
		fragData.anchors.back().input.flagSecondary = (fan.airbrushMode) ? InputFlag::connectOriginSwapClearMetadata : InputFlag::connectOrigin;
	}
}

void In_Fan::generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{
	FragmentAnchor* vertex1 = &splineData.anchors[splineData.anchors.size() - 2];
	FragmentAnchor* vertex2 = &splineData.anchors[splineData.anchors.size() - 1];
	glm::vec3 subPos = *pos; glm::vec3 subDir = *dir;
	int subdivCount = (continuous.subdivide) ? int(floor((glm::length(*pos - vertex1->pos) / continuous.trueSpacing) * continuous.subdivideAmount)) : 0;
	if (subdivCount == 0) {
		fillVertices(pos, dir, dat);
	}
	else {
		for (int i = 1; i < subdivCount + 1; i++) {
			float t = float(i) / float(subdivCount);
			subPos = lerpPos(vertex1->pos, *pos, t);
			subDir = makeDir(splineData.anchors.at(splineData.anchors.size() - 2).pos, subPos);
			Input lerpDat = *dat;
			lerpDat.pressure = lerpf(vertex1->input.pressure, dat->pressure, t);
			addVertices(&subPos, &subDir, &lerpDat);
		}
	}
}

void In_Fan::fillVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{
	glm::vec3 connectPoint = splineData.anchors.at(splineData.anchors.size() - 2).pos;
	glm::vec3 connectLineDir = makeDir(connectPoint, *pos);

	float distance = glm::length(*pos - connectPoint);
	float usedDistance = 0.0f;
	while (usedDistance < distance)
	{
		glm::vec3 fragPos = connectPoint + (connectLineDir * usedDistance);
		glm::vec3 fragDir = makeDir(fragData.transform.origin, fragPos);
		anchorIDCount++;
		Input input = lerpInputs(&data.inputEvents.back(), &splineData.anchors.back().input, usedDistance);
		addVertices(&fragPos, &fragDir, &input);

		usedDistance += continuous.trueSpacing;
	}
}

void In_Fan::generateCurve()
{
	FragmentAnchor* vertex1 = &splineData.anchors[splineData.anchors.size() - 2];
	FragmentAnchor* vertex2 = &splineData.anchors[splineData.anchors.size() - 1];
	glm::vec2 control_points[4] = { vertex1->pos, vertex1->headHandle, vertex2->tailHandle, vertex2->pos };

	std::vector<glm::vec3> points;
	float x = glm::length(control_points[1] - control_points[0]);
	float y = glm::length(control_points[2] - control_points[0]);
	int resolution = int(10 + ((x * y) / 10000.0f));
	glm::vec4 startScale = glm::vec4(1.0f); glm::vec4 endScale = glm::vec4(1.0f);
	float segLen = glm::length(vertex2->pos - vertex1->pos);
	float usedLen = 0.0f;
	getCubicBezierPoints(control_points, resolution, 0.0f, points, segLen, continuous.trueSpacing, startScale.z, endScale.z);
	for (int i = 0; i < points.size(); i++) {
		glm::vec3 pos = points[i];
		glm::vec3 dir = DEFAULT_DIR;
		if (i == 0) {
			dir = makeDir(vertex1->pos, pos);
		}
		else if (i == points.size() - 1) {
			dir = makeDir(pos, vertex2->pos);
		}
		else if (points.size() >= 2) {
			dir = makeDir(points[(size_t)i - 1], points[i]);
		}
		else {
			dir = makeDir(pos, vertex2->pos);
		}
		Input input = lerpInputs(&vertex1->input, &vertex2->input, usedLen / segLen);
		addVertices(&pos, &dir, &input);
		usedLen += continuous.trueSpacing;
	}
}

void In_Fan::newInput(Application* sender, Input dat)
{
	if (owner.get()->checkInterestMask(TSetType::fan)) { fan = *owner.get()->getFan(); }
	else { fan.isEnabled = false; }
}