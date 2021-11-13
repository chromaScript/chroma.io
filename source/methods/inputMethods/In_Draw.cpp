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

////////////////////////////////////////////////////////////////
//
// Move Function
//
////////////////////////////////////////////////////////////////
InputHandlerFlag In_Draw::move(Application* sender, Input dat)
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

////////////////////////////////////////////////////////////////
//
// Click Function
//
////////////////////////////////////////////////////////////////
InputHandlerFlag In_Draw::click(Application* sender, Input dat)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (controlScheme == TSetType::continuous) 
	{
		wasHandled = continuousClick(sender, dat, 1, 1, InputFlag::null, InputFlag::null);
	}
	return wasHandled;
}

InputHandlerFlag In_Draw::key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys)
{
	InputHandlerFlag wasHandled = InputHandlerFlag::noSignal;
	if (controlScheme == TSetType::continuous)
	{
		wasHandled = continuousKey(sender, dat, key, action, modKeys);
	}
	return wasHandled;
}

void In_Draw::initializeVertices(glm::vec3* pos, glm::vec3* dir, Input* dat,
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

void In_Draw::addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
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
		if (continuous.connectInterval > 1 && anchorIDCount % continuous.connectInterval == 0) {
			fragData.anchors.back().input.flagSecondary = InputFlag::point;
		}
	}
}

void In_Draw::generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat)
{
	glm::vec3 pos1 = splineData.anchors[splineData.anchors.size() - 2].pos;
	glm::vec3 dir1 = splineData.anchors[splineData.anchors.size() - 2].dir;
	glm::vec3 pos2 = *pos;
	glm::vec3 dir2 = *dir;
	float pressure1 = splineData.anchors[splineData.anchors.size() - 2].input.pressure;
	float pressure2 = dat->pressure;
	glm::vec3 subPos = pos2; glm::vec3 subDir = dir2;
	int subdivCount = (continuous.subdivide) ? int(floor((glm::length(pos2 - pos1) / continuous.trueSpacing) * continuous.subdivideAmount)) : 0;
	if (subdivCount == 0) {
		addVertices(&subPos, &subDir, dat);
	}
	else {
		for (int i = 1; i < subdivCount + 1; i++) {
			float t = float(i) / float(subdivCount);
			subPos = lerpPos(pos1, pos2, t);
			subDir = makeDir(fragData.anchors.back().pos, subPos);
			Input lerpDat = *dat;
			lerpDat.pressure = lerpf(pressure1, pressure2, t);
			addVertices(&subPos, &subDir, &lerpDat);
		}
	}
}

void In_Draw::generateCurve()
{
	fragData.anchors.back() = splineData.anchors[splineData.anchors.size() - 2];
	fragData.anchors.push_back(splineData.anchors.back());
	fragData.depth = 1;
	fragData.anchors.back().input.flagSecondary = InputFlag::updateData;
}

void In_Draw::newInput(Application* sender, Input dat)
{

}