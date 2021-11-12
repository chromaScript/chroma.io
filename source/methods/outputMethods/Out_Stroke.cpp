#include "../../include/methods/outputMethods/Out_Stroke.h"
#include "../../include/math/Color.h"
#include "../../include/math/curve.h"

#ifndef APPLICATION_H
#include "../../include/Application.h"
#endif
#ifndef USERINTERFACE_H
#include "../../include/entities/UserInterface.h"
#endif
#ifndef STROKE_H
#include "../../include/entities/Stroke.h"
#endif
#ifndef TOOLBOX_H
#include "../../include/tool/Toolbox.h"
#endif
#ifndef TOOL_H
#include "../../include/tool/Tool.h"
#endif

#include "../../include/entities/visuals/Visualizer.h"
#include "../../include/entities/visuals/PreviewObj.h"
#include "../../include/tool/toolSettings/ToolSettings_Forward.h"

#include <ext/matrix_projection.hpp>

#include <memory>
#include <iostream>

void Out_Stroke::preview(Application* sender, VertexData* dat, InputHandlerFlag action)
{
	// Kick bad-calls
	if (dat->anchors.size() == 0) { return; }

	// Handle incoming vertex data that is not of constant size, all behavior is
	// influenced by this variable, so follow two main logic branches
	
	if (!dat->constantSize && dat->linearStream)
	{
		// Check if the input is new. Inputs will always mark their first anchor with NEW_INPUT and then
		// on the next update remove the flagPrimary.
		if (dat->anchors.front().input.flagPrimary == InputFlag::newInput)
		{
			// Begin new stroke, pass copies of the tool settings to the new stroke
			// Only the settings that the stroke needs to render will get passed. Settings for
			// things like smoothing and correction do not get passed along.
			if (activeFrag == nullptr && !sender->getUI()->getCanvas()->getActiveLayer().expired())
			{
				activeFrag = sender->getUI()->getCanvas()->getActiveLayer().lock()->createNewStroke(
					sender->getShardShader(), owner, increaseEntityCount());
				activeFrag.get()->fragData.transform = dat->transform;
				activeFrag.get()->fragData.constantSize = dat->constantSize;
				activeFrag.get()->fragData.linearStream = dat->linearStream;
				activeFrag.get()->fragData.connectEnds = dat->connectEnds;
				lastAnchorIndex = 0;
				activePointsLayer = activeLinesLayer = activeBoundsLayer = activeCurvesLayer = 0;
				lastAnchorArrayIndex = 0;
				std::cout << "OUT_STROKE::CREATENEWSTROKE::CONST_POSITION= " << std::endl;
			}

		}
		if (action == InputHandlerFlag::previewLine)
		{
			if (activeFrag == nullptr) { 
				sender->ui->visualizer->setPreview(PreviewLayerType::inputPoint, false);
				sender->ui->visualizer->setPreview(PreviewLayerType::inputLine, false);
				sender->ui->visualizer->setPreview(PreviewLayerType::inputCurves, false);
				if (activePointsLayer != 0) { sender->ui->visualizer->removeLayer(activePointsLayer); }
				if (activeLinesLayer != 0) { sender->ui->visualizer->removeLayer(activeLinesLayer); }
				if (activeCurvesLayer != 0) { sender->ui->visualizer->removeLayer(activeCurvesLayer); }
				activePointsLayer = activeLinesLayer = activeCurvesLayer = 0;
				return; 
			}
			if (dat->anchors.size() < 2) { return; }
			if (activePointsLayer == 0) {
				unsigned int pointsLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputPoint);
				if (sender->ui->visualizer->addLayer(pointsLayer, (size_t)dat->subdivCount + 2, BlendMode::multiply) == pointsLayer) {
					activePointsLayer = pointsLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputPoint, true);
				}
			}
			if (activeLinesLayer == 0) {
				unsigned int linesLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
				if (sender->ui->visualizer->addLayer(linesLayer, 1, BlendMode::multiply) == linesLayer) {
					activeLinesLayer = linesLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
				}
			}
			if (activeCurvesLayer != 0) {
				sender->ui->visualizer->removeLayer(activeCurvesLayer);
				sender->ui->visualizer->setPreview(PreviewLayerType::inputCurves, false);
				activeCurvesLayer = 0;
			}
			size_t size = dat->anchors.size();
			if (activePointsLayer != 0) {
				glm::vec3 pos1 = dat->anchors[size - 2].pos;
				glm::vec3 dir1 = dat->anchors[size - 2].dir;
				glm::vec3 pos2 = dat->anchors[size - 1].pos;
				glm::vec3 dir2 = dat->anchors[size - 1].dir;
				float pressure1 = dat->anchors[size - 2].input.pressure;
				float pressure2 = dat->anchors[size - 1].input.pressure;
				glm::vec3 subPos = pos1; glm::vec3 subDir = dir1;
				for (int i = 0; i < dat->subdivCount + 2; i++) {
					if (i == 0) {
						sender->ui->visualizer->putLayerObject(activePointsLayer, 0,
							PreviewObj(dat->anchors[size - 2].ID, pos1, dir1,
								sender->ui->fgColor, ShapeType::square,
								3.0f + (pressure1 * (4.0f))));
					}
					else if (i == dat->subdivCount + 1) {
						sender->ui->visualizer->putLayerObject(activePointsLayer, (size_t)dat->subdivCount + 1,
							PreviewObj(dat->anchors[size - 1].ID, pos2, dir2,
								sender->ui->fgColor, ShapeType::square,
								3.0f + (pressure2 * (4.0f))));
					}
					else {
						float t = float(i) / float(dat->subdivCount + 1);
						subPos = lerpPos(pos1, pos2, t);
						subDir = lerpDir(dir1, dir2, t);
						sender->ui->visualizer->putLayerObject(activePointsLayer, i,
							PreviewObj(dat->anchors[size - 1].ID, subPos, subDir,
								sender->ui->fgColor, ShapeType::square,
								3.0f + (lerpf(pressure1, pressure2, t) * (4.0f))));
					}
					sender->ui->visualizer->trimLayer(activePointsLayer, (size_t)dat->subdivCount + 2);
				}
			}
			if (activeLinesLayer != 0) {
				sender->ui->visualizer->updateLayerObject(activeLinesLayer, 0,
					PreviewObj(dat->anchors[size - 1].ID,
						dat->anchors[size - 2].pos, dat->anchors[size - 1].pos, sender->ui->fgColor, ShapeType::line, 2.0f));
			}
			return;
		}
		else if (action == InputHandlerFlag::previewCurves || action == InputHandlerFlag::editMode)
		{
			if (activeFrag == nullptr) {
				sender->ui->visualizer->setPreview(PreviewLayerType::inputPoint, false);
				sender->ui->visualizer->setPreview(PreviewLayerType::inputLine, false);
				if (activePointsLayer != 0) { sender->ui->visualizer->removeLayer(activePointsLayer); }
				if (activeLinesLayer != 0) { sender->ui->visualizer->removeLayer(activeLinesLayer); }
				activePointsLayer = activeLinesLayer = 0;
				return;
			}
			if (activePointsLayer == 0) {
				unsigned int pointsLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputPoint);
				if (sender->ui->visualizer->addLayer(pointsLayer, (size_t)dat->subdivCount + 2, BlendMode::multiply) == pointsLayer) {
					activePointsLayer = pointsLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputPoint, true);
				}
			}
			else {
				sender->ui->visualizer->setPreview(PreviewLayerType::inputPoint, true);
			}
			if (activeLinesLayer == 0) {
				unsigned int linesLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
				if (sender->ui->visualizer->addLayer(linesLayer, 1, BlendMode::multiply) == linesLayer) {
					activeLinesLayer = linesLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
				}
			}
			else {
				sender->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
			}
			if (activeCurvesLayer == 0) {
				unsigned int curvesLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputCurves);
				if (sender->ui->visualizer->addLayer(curvesLayer, 1, BlendMode::multiply) == curvesLayer) {
					activeCurvesLayer = curvesLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputCurves, true);
				}
			}
			else {
				sender->ui->visualizer->setPreview(PreviewLayerType::inputCurves, true);
			}
			size_t size = dat->anchors.size();
			if (activePointsLayer != 0) {
				// Start / End Points
				sender->ui->visualizer->putLayerObject(activePointsLayer, 0,
					PreviewObj(dat->anchors[size - 1].ID, dat->anchors[size - 2].pos, makeDir(dat->anchors[size - 2].pos, dat->anchors[size - 2].headHandle),
						sender->ui->fgColor, ShapeType::square,
						3.0f + (dat->anchors[size - 2].input.pressure * (4.0f))));
				sender->ui->visualizer->putLayerObject(activePointsLayer, 1,
					PreviewObj(dat->anchors[size - 2].ID, dat->anchors[size - 1].pos, makeDir(dat->anchors[size - 1].tailHandle, dat->anchors[size - 1].pos),
						sender->ui->fgColor, ShapeType::square,
						3.0f + (dat->anchors[size - 1].input.pressure * (4.0f))));
				// Handle Points
				sender->ui->visualizer->putLayerObject(activePointsLayer, 2,
					PreviewObj(dat->anchors[size - 2].ID, 
						dat->anchors[size - 2].headHandle, dat->anchors[size - 2].dir,
						blue, ShapeType::square,
						4.0f));
				sender->ui->visualizer->putLayerObject(activePointsLayer, 3,
					PreviewObj(dat->anchors[size - 1].ID,
						dat->anchors[size - 1].tailHandle, dat->anchors[size - 1].dir,
						blue, ShapeType::square,
						4.0f));
				sender->ui->visualizer->trimLayer(activePointsLayer, 4);
			}
			if (activeLinesLayer != 0) {
				sender->ui->visualizer->putLayerObject(activeLinesLayer, 0,
					PreviewObj(dat->anchors[size - 1].ID,
						dat->anchors[size - 2].pos, dat->anchors[size - 2].headHandle,
						blue, ShapeType::line, 1.0f));
				sender->ui->visualizer->putLayerObject(activeLinesLayer, 1,
					PreviewObj(dat->anchors[size - 1].ID,
						dat->anchors[size - 1].tailHandle, dat->anchors[size - 1].pos,
						blue, ShapeType::line, 1.0f));
				sender->ui->visualizer->trimLayer(activeLinesLayer, 2);
			}
			if (activeCurvesLayer != 0) {
				// Construct the bounds and points data for the curves preview object
				RectBounds bezierBounds = RectBounds();
				getCubicBezierOBB(bezierBounds, &dat->anchors[size - 2], &dat->anchors[size - 1], true, 50.0f);
				sender->ui->visualizer->putLayerObject(activeCurvesLayer, 0,
					PreviewObj(dat->anchors[size - 1].ID,
						dat->anchors[size - 2].pos, dat->anchors[size - 2].headHandle, 
						dat->anchors[size - 1].tailHandle, dat->anchors[size - 1].pos,
						bezierBounds, sender->ui->fgColor, ShapeType::curve, 2.0f));
			}
		}
		else {
			if (activePointsLayer != 0) {
				sender->ui->visualizer->removeLayer(activePointsLayer);
				sender->ui->visualizer->setPreview(PreviewLayerType::inputPoint, false);
				activePointsLayer = 0;
			}
			if (activeLinesLayer != 0) {
				sender->ui->visualizer->removeLayer(activeLinesLayer);
				sender->ui->visualizer->setPreview(PreviewLayerType::inputLine, false);
				activeLinesLayer = 0;
			}
			if (activeCurvesLayer != 0) {
				sender->ui->visualizer->removeLayer(activeCurvesLayer);
				sender->ui->visualizer->setPreview(PreviewLayerType::inputCurves, false);
				activeCurvesLayer = 0;
			}
			if (action == InputHandlerFlag::continueInput)
			{
				if (lastFrag != nullptr) { 
					activeFrag = lastFrag; 
				}
			}
			if (activeFrag == nullptr) { return; }
			if (dat->anchors.size() != 0 && lastAnchorArrayIndex != 0)
			{
				if (dat->anchors.size() < lastAnchorArrayIndex || dat->anchors[size_t(lastAnchorArrayIndex) - size_t(1)].ID != lastAnchorIndex)
				{
					std::cout << "LAST_INDEX::" << lastAnchorArrayIndex << "  ANCHOR_SIZE::" << dat->anchors.size() << "  SKIP_A" << std::endl;
					int i = 0;
					int syncIndex = -1;
					while (i < dat->anchors.size())
					{
						if (dat->anchors[i].ID >= lastAnchorIndex) { syncIndex = i; break; }
						i++;
					}
					if (syncIndex != -1)
					{
						lastAnchorArrayIndex = syncIndex;
						lastAnchorIndex = dat->anchors[lastAnchorArrayIndex].ID;
					}
					else
					{
						lastAnchorArrayIndex = dat->anchors.size() - 1;
						lastAnchorIndex = dat->anchors[lastAnchorArrayIndex].ID;
					}
				}
			}
			while (lastAnchorArrayIndex < dat->anchors.size())
			{
				bool enableBatch = false;
				// Special handling for batch rendering
				if (enableBatch && dat->anchors.size() - lastAnchorArrayIndex > 3)
				{
					int advance = lastAnchorArrayIndex;
					for (int i = advance; i < dat->anchors.size(); i++)
					{
						int k = 5;
					}
				}
				// Regular linear input stream
				else if (true)
				{
					if (dat->anchors[lastAnchorArrayIndex].input.flagSecondary == InputFlag::updateData) {
						size_t size = activeFrag.get()->fragData.anchors.size();
						size_t depth = 1;
						for (int i = size - 1; i >= size - ((size_t)dat->depth + 0); --i) {
							if ((int)lastAnchorArrayIndex - (int)depth < 0) { break; }
							activeFrag.get()->fragData.anchors[i] = dat->anchors[(size_t)lastAnchorArrayIndex - depth];
							depth++;
						}
						dat->anchors[lastAnchorArrayIndex].input.flagSecondary == InputFlag::null;
						dat->depth = 0;
					}
					activeFrag.get()->fragData.anchors.push_back(dat->anchors[lastAnchorArrayIndex]);
					activeFrag.get()->processNewAnchor();
					lastAnchorArrayIndex++;
					lastAnchorIndex = activeFrag.get()->fragData.anchors.back().ID;
				}

			}
		}
		
	}
	// Handle vertex data that is of a constant size, and also follows a linear input pattern
	else if (dat->constantSize && dat->linearStream)
	{
		size_t polycount = (int)dat->anchors.size();
		if (dat->anchors.front().input.flagPrimary == InputFlag::newInput)
		{
			// Begin new stroke, pass copies of the tool settings to the new stroke
			// Only the settings that the stroke needs to render will get passed. Settings for
			// things like smoothing and correction do not get passed along.
			if (!sender->getUI()->getCanvas()->getActiveLayer().expired())
			{
				activeFrag = sender->getUI()->getCanvas()->getActiveLayer().lock()->createNewStroke(
					sender->getShardShader(), owner, increaseEntityCount());
				activePointsLayer = activeLinesLayer = activeBoundsLayer = 0; lastAnchorIndex = -1;
				copyVertexData(activeFrag.get()->fragData, dat);
				// Create Preview Layers
				unsigned int pointsLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputPoint);
				if (sender->ui->visualizer->addLayer(pointsLayer, polycount, BlendMode::multiply) == pointsLayer) {
					activePointsLayer = pointsLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputPoint, true);
				}
				unsigned int linesLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
				if (sender->ui->visualizer->addLayer(linesLayer, polycount, BlendMode::multiply) == linesLayer) {
					activeLinesLayer = linesLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
				}
				unsigned int boundsLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputBounds);
				if (sender->ui->visualizer->addLayer(boundsLayer, 8, BlendMode::multiply) == boundsLayer) {
					activeBoundsLayer = boundsLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputBounds, true);
				}
			}
		}
		if (activeFrag != nullptr && lastAnchorIndex != dat->anchors.front().ID)
		{
			// Copy the transform data
			activeFrag.get()->fragData.transform = dat->transform;
			copyVertices(activeFrag.get()->fragData, dat);
			// Update Preview Layers
			if (activePointsLayer != 0) {
				for (size_t i = 0; i < polycount; i++) {
					sender->ui->visualizer->updateLayerObject(activePointsLayer, i,
						PreviewObj(activeFrag.get()->fragData.anchors[i].ID,
							activeFrag.get()->fragData.anchors[i].pos,
							activeFrag.get()->fragData.anchors[i].dir,
							sender->ui->fgColor, ShapeType::square, 
							3.0f + ((activeFrag.get()->fragData.anchors[i].input.pressure / 1.0f) * (8.0f)))
					);
				}
			}
			if (activeLinesLayer != 0) {
				glm::vec3 pos1 = glm::vec3(0.0f);
				glm::vec3 pos2 = glm::vec3(0.0f);
				for (size_t i = 0; i < polycount; i++) 
				{
					if (i != polycount - 1) {
						pos1 = activeFrag.get()->fragData.anchors[i].pos;
						pos2 = activeFrag.get()->fragData.anchors[i + 1].pos;
					}
					else {
						pos1 = activeFrag.get()->fragData.anchors[i].pos;
						pos2 = activeFrag.get()->fragData.anchors[0].pos;
					}
					sender->ui->visualizer->updateLayerObject(activeLinesLayer, i,
						PreviewObj(activeFrag.get()->fragData.anchors[i].ID,
							pos1, pos2, sender->ui->fgColor, ShapeType::line, 3.0f)
					);
				}
			}
			if (activeBoundsLayer != 0) {
				sender->ui->visualizer->putBoundsObject(activeBoundsLayer, activeFrag.get()->fragData.transform);
			}
			lastAnchorIndex = dat->anchors.front().ID;
		}
		else
		{
			//std::cout << "MethodType::out_stroke::CONST_VERTCOUNT::INPUT_STREAM_PAUSED" << std::endl;
		}
	}
	// Handle vertex data that may or may not be constant, but follows a non-linear input pattern
	else if (!dat->linearStream)
	{
		size_t polycount = (int)dat->anchors.size();
		// Non-linear stream with either constant or varying size
		if (dat->anchors.front().input.flagPrimary == InputFlag::newInput)
		{
			if (!sender->getUI()->getCanvas()->getActiveLayer().expired())
			{
				activeFrag = sender->getUI()->getCanvas()->getActiveLayer().lock()->createNewStroke(
					sender->getShardShader(), owner, increaseEntityCount());
				activePointsLayer = activeLinesLayer = activeBoundsLayer = 0; lastAnchorIndex = -1;
				copyVertexData(activeFrag.get()->fragData, dat);
				// Create Preview Layers
				unsigned int pointsLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputPoint);
				if (sender->ui->visualizer->addLayer(pointsLayer, polycount, BlendMode::multiply) == pointsLayer) {
					activePointsLayer = pointsLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputPoint, true);
				}
				unsigned int linesLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputLine);
				if (sender->ui->visualizer->addLayer(linesLayer, polycount, BlendMode::multiply) == linesLayer) {
					activeLinesLayer = linesLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputLine, true);
				}
				unsigned int boundsLayer = sender->ui->visualizer->requestNewLayer(PreviewLayerType::inputBounds);
				if (sender->ui->visualizer->addLayer(boundsLayer, 8, BlendMode::multiply) == boundsLayer) {
					activeBoundsLayer = boundsLayer;
					sender->ui->visualizer->setPreview(PreviewLayerType::inputBounds, true);
				}
			}
		}
		if (activeFrag != nullptr && lastAnchorIndex != dat->anchors.front().ID)
		{
			// Copy the transform data
			copyVertices(activeFrag.get()->fragData, dat);
			activeFrag.get()->fragData.transform = dat->transform;
			// Update Preview Layers
			if (activePointsLayer != 0) {
				for (size_t i = 0; i < polycount; i++) {
					sender->ui->visualizer->putLayerObject(activePointsLayer, i,
						PreviewObj(activeFrag.get()->fragData.anchors[i].ID,
							activeFrag.get()->fragData.anchors[i].pos,
							activeFrag.get()->fragData.anchors[i].dir,
							sender->ui->fgColor, ShapeType::square, 5.0f)
					);
				}
			}
			if (activeLinesLayer != 0) {
				glm::vec3 pos1 = glm::vec3(0.0f);
				glm::vec3 pos2 = glm::vec3(0.0f);
				CColor color = sender->ui->fgColor;
				for (size_t i = 0; i < polycount; i++)
				{
					color = sender->ui->fgColor;
					if (i != polycount - 1) {
						if (activeFrag.get()->fragData.anchors[i + 1].input.flagSecondary != InputFlag::point) {
							pos1 = activeFrag.get()->fragData.anchors[i].pos;
							pos2 = activeFrag.get()->fragData.anchors[i + 1].pos;
						}
						else {
							color.a = 0.0f;
							pos1 = glm::vec3(0.0f);
							pos2 = glm::vec3(0.0f);
						}
					}
					else if (dat->connectEnds) {
						pos1 = activeFrag.get()->fragData.anchors[i].pos;
						pos2 = activeFrag.get()->fragData.anchors[0].pos;
					}
					else {
						color.a = 0.0f;
						pos1 = glm::vec3(0.0f);
						pos2 = glm::vec3(0.0f);
					}
					sender->ui->visualizer->putLayerObject(activeLinesLayer, i,
						PreviewObj(activeFrag.get()->fragData.anchors[i].ID,
							pos1, pos2, color, ShapeType::line, 2.0f)
					);
				}
			}
			if (activeBoundsLayer != 0) {
				sender->ui->visualizer->putBoundsObject(activeBoundsLayer, activeFrag.get()->fragData.transform);
			}
			lastAnchorIndex = dat->anchors.front().ID;
		}
		else
		{
			//std::cout << "MethodType::out_stroke::CONST_VERTCOUNT::INPUT_STREAM_PAUSED" << std::endl;
		}
	}
	return;
}


void Out_Stroke::finalize(Application* sender, VertexData* dat)
{
	// Clear Preview Data
	sender->ui->visualizer->setPreview(PreviewLayerType::inputPoint, false);
	sender->ui->visualizer->setPreview(PreviewLayerType::inputLine, false);
	sender->ui->visualizer->setPreview(PreviewLayerType::inputBounds, false);
	sender->ui->visualizer->setPreview(PreviewLayerType::inputCurves, false);
	if (activePointsLayer != 0) { sender->ui->visualizer->removeLayer(activePointsLayer); }
	if (activeLinesLayer != 0) { sender->ui->visualizer->removeLayer(activeLinesLayer); }
	if (activeBoundsLayer != 0) { sender->ui->visualizer->removeLayer(activeBoundsLayer); }
	if (activeCurvesLayer != 0) { sender->ui->visualizer->removeLayer(activeCurvesLayer); }
	activePointsLayer = activeLinesLayer = activeBoundsLayer = activeCurvesLayer = 0;

	// Kick bad-calls
	if (dat->anchors.size() == 0 || activeFrag == nullptr) { return; }

	if (!dat->constantSize && dat->linearStream)
	{
		lastAnchorIndex = dat->anchors.back().ID;
		if (dat->anchors.back().input.flagSecondary == InputFlag::endRecording)
		{
			sender->requestRemove_targetStroke(activeFrag.get()->container, activeFrag.get()->weak_from_this());
		}
		else {
			lastFrag = activeFrag;
			activeFrag = nullptr;
		}
		// See comments below for cues as to what might be done for non-constant, linear input
	}
	else if (dat->constantSize && dat->linearStream)
	{
		if (activeFrag == nullptr) { return; }
		int polyCount = (int)dat->anchors.size();
		activeFrag.get()->fragData.anchors.clear();
		for (int i = 0; i < polyCount; i++)
		{
			activeFrag.get()->fragData.anchors.push_back(dat->anchors[i]);
			activeFrag.get()->processNewAnchor();
			if (i == polyCount - 1 && dat->connectEnds)
			{
				activeFrag.get()->fragData.anchors.push_back(dat->anchors[0]);
				activeFrag.get()->processNewAnchor();
			}
			lastAnchorIndex = dat->anchors.back().ID;
		}
		activeFrag.get()->fragData.transform = dat->transform;
		lastFrag = activeFrag;
		activeFrag = nullptr;
	}
	else if (!dat->linearStream)
	{
		if (activeFrag == nullptr) { return; }
		int polyCount = (int)dat->anchors.size();
		activeFrag.get()->fragData.anchors.clear();
		for (int i = 0; i < polyCount; i++)
		{
			activeFrag.get()->fragData.anchors.push_back(dat->anchors[i]);
			activeFrag.get()->processNewAnchor();
			if (i == polyCount - 1 && dat->connectEnds)
			{
				activeFrag.get()->fragData.anchors.push_back(dat->anchors[0]);
				activeFrag.get()->processNewAnchor();
			}
			lastAnchorIndex = dat->anchors.back().ID;
		}
		activeFrag.get()->fragData.transform = dat->transform;
		lastFrag = activeFrag;
		activeFrag = nullptr;
	}
	
	return;
	// 1. Apply head/tail correction Note: unfinished area
		// A. Adjust the taper of the head/tail according to tool settings (for testing allow minor correction)

		// B. Adjust the opacity of the head/tail according to tool settings

	// 2. Combine redundant anchors
	// Warning: This feels very unsafe, but so far looks OK in the output log. May need to very carefully look over this
	// later in order to ensure proper function. Without visualization its unclear whether this is having the desired
	// effect on the anchor array.

	/*
	int size = (int)activeFrag.get()->getFragmentData().get()->anchors.size();
	size_t i = 1;
	std::vector<size_t> deleteIndicies;
	if (size > 3) // Note: Should add a case for small Fragments later. Should a single point stroke should have more than 1 anchor?
	{
		for (i; i < size; i++)
		{
			glm::vec3 p1 = activeFrag.get()->getFragmentData().get()->anchors[i - 1].pos;
			glm::vec3 p2 = activeFrag.get()->getFragmentData().get()->anchors[i].pos;
			glm::vec3 dir1 = activeFrag.get()->getFragmentData().get()->anchors[i - 1].dir;
			glm::vec3 dir2 = activeFrag.get()->getFragmentData().get()->anchors[i].dir;
			// Compare position & directiton
			if (compareVec3_byDistance(p1, p2, posThresholdLength) == true
				&& compareDir_byAngle(dir1, dir2, dirThresholdAngle) == true)
			{
				// Then compare secondary characteristics
				if (compareInputByProperties(&(activeFrag.get()->getFragmentData().get()->anchors[i]),
					&(activeFrag.get()->getFragmentData().get()->anchors[i - 1])) == true)
				{ 
					// Average properties of the two anchors and copy that to anchor[i - 1], then mark anchor[i] for deletion
					activeFrag.get()->getFragmentData().get()->anchors[i - 1] = average2Anchors(&(activeFrag.get()->getFragmentData().get()->anchors[i - 1]),
						&(activeFrag.get()->getFragmentData().get()->anchors[i]));
					deleteIndicies.push_back(i);
					std::cout << "STROKE::RAW-INPUT::COMPARISON::MARKDELETE::AT_INDEX=" << i << std::endl;
				}
			}
		}
		std::cout << "STROKE::ANCHORCOUNT=" << size << "::MARKED_DELETE=" << deleteIndicies.size() << std::endl;
		int deleteCount = 0;
		auto begin = activeFrag.get()->getFragmentData().get()->anchors.begin();
		for (int j = 0; j < (int)deleteIndicies.size(); j++)
		{
			activeFrag.get()->getFragmentData().get()->anchors.erase(begin + (deleteIndicies[j] - deleteCount));
			std::cout << "STROKE::RAW-INPUT::CLEANUP::ERASE::AT_INDEX=" << deleteIndicies[j] << "::OF_REAL_INDEX="
				<< deleteIndicies[j] - 1 << std::endl;
			deleteCount++;
		}
		std::cout << "STROKE::RAW-INPUT::CLEANUP::POST-ANCHORCOUNT=" << activeFrag.get()->getFragmentData().get()->anchors.size() << std::endl;
	}
		// A. Check each anchor in the line against it's neighbors.
			// A1. Evaluate groups of four. Skip the first anchor, then sample in clusters as follows (0, 1, 2, 2, 1, 0)
			// move through the anchor array one at a time, but skip if the anchor directions are too different

			// A2. If the anchors are very similar in characteristics, calculate their average, then delete one, and
			// overwrite the other with the averaged data

	*/

	// 3. Calculate & set the transform data
	//activeFrag.get()->updateTransformByAnchors();
}
void Out_Stroke::postprocess(Application* sender, VertexData* dat)
{
	// Note: unfinished area
	// Branch (Tool settings -> post correction vs. stabilization setting vs. raw imput
		// 1x. Stabilization: Calculate Bezier Curves (do not implement on first draft)
			// A. Sample along the curve? Or Sample for significant direction changes?

			// B. Remove anchors that can be expressed by a curve between it's neighboring points

		// 1y. Post Correction: Similar to Bezier Curve but more aggressive? Allow straight lines?

		// 1z. Raw Input (first implementation task)
			// A. Double check everything? Do nothing?
	
	// Output is done, reset and request the next stroke
	//sender->fragmentDataDump(activeFrag.get()->getFragmentData(), std::string("STROKE"), activeFrag.get()->getUEID());
	//_activeFrag.get()->rebuildFragmentShards();
	//activeStroke = false;
}
bool Out_Stroke::compareInputByProperties(FragmentAnchor* anchor1, FragmentAnchor* anchor2)
{
	if (abs(anchor1->input.pressure - anchor2->input.pressure) > pressureThreshold) { return false; }
	if (abs((anchor1->input.tiltX + anchor1->input.tiltY) - (anchor2->input.tiltX + anchor2->input.tiltY)) > tiltThreshold) { return false; }
	if (abs(anchor1->input.rotation - anchor2->input.rotation) > rotationThreshold) { return false; }
	if (abs(anchor1->input.velocity - anchor2->input.velocity) > velocityThreshold) { return false; }
	return true;
}
FragmentAnchor Out_Stroke::average2Anchors(FragmentAnchor* anchor1, FragmentAnchor* anchor2)
{
	// Default is copy modKey of anchor1, assign new values where necessary
	FragmentAnchor outAnchor = *anchor1;
	outAnchor.pos = glm::vec3(
		((anchor1->pos.x + anchor2->pos.x) / 2.0f),
		((anchor1->pos.y + anchor2->pos.y) / 2.0f),
		((anchor1->pos.z + anchor2->pos.z) / 2.0f)
	);
	outAnchor.dir = glm::normalize(glm::vec3(
		anchor1->dir.x + anchor2->dir.x,
		anchor1->dir.y + anchor2->dir.y,
		anchor1->dir.z + anchor2->dir.z));
	// Check twice that the direction is valid (NaN is very bad)
	if (std::isnan(outAnchor.dir.x))
	{
		outAnchor.dir = anchor1->dir;
	}
	if (std::isnan(outAnchor.dir.x))
	{
		outAnchor.dir = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	outAnchor.input.pressure = (anchor1->input.pressure + outAnchor.input.pressure) / 2.0f;
	outAnchor.input.tiltX = (anchor1->input.tiltX + outAnchor.input.tiltX) / 2.0f;
	outAnchor.input.tiltY = (anchor1->input.tiltY + outAnchor.input.tiltY) / 2.0f;
	outAnchor.input.rotation = (anchor1->input.rotation + outAnchor.input.rotation) / 2.0f;
	outAnchor.input.velocity = (anchor1->input.velocity + outAnchor.input.velocity) / 2.0f;
	return outAnchor;
}