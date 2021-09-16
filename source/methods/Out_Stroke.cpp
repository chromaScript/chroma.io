#include "../include/methods/Out_Stroke.h"
#include "../include/Color.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif
#ifndef USERINTERFACE_H
#include "../include/entities/UserInterface.h"
#endif
#ifndef STROKE_H
#include "../include/entities/Stroke.h"
#endif
#ifndef TOOLBOX_H
#include "../include/Toolbox.h"
#endif
#ifndef TOOL_H
#include "../include/Tool.h"
#endif

#include "../include/toolSettings/ToolSettings_Forward.h"

#include <ext/matrix_projection.hpp>

#include <memory>
#include <iostream>

void Out_Stroke::preview(Application* sender, VertexData* dat)
{
	// Kick bad-calls
	if (dat->anchors.size() == 0) { return; }

	// Handle incoming vertex data that is not of constant size, all behavior is
	// influenced by this variable, so follow two main logic branches
	if (!dat->constantSize && dat->linearStream)
	{
		// Check if the input is new. Inputs will always mark their first anchor with NEW_INPUT and then
		// on the next update remove the flag.
		if (dat->anchors.front().flag == FLAG_NEW_INPUT)
		{
			// Begin new stroke, pass copies of the tool settings to the new stroke
			// Only the settings that the stroke needs to render will get passed. Settings for
			// things like smoothing and correction do not get passed along.
			if (!sender->getUI()->getCanvas()->getActiveLayer().expired())
			{
				activeFrag = sender->getUI()->getCanvas()->getActiveLayer().lock()->createNewStroke(
					sender->getShardShader(), owner);
				activeFrag.get()->fragData.constantSize = dat->constantSize;
				activeFrag.get()->fragData.linearStream = dat->linearStream;
				activeFrag.get()->fragData.connectEnds = dat->connectEnds;
				lastAnchorIndex = 0;
				lastAnchorArrayIndex = 0;
				std::cout << "OUT_STROKE::CREATENEWSTROKE::CONST_POSITION= " << std::endl;
			}
		}
		// Prevent bad pointer access, should not trigger this, but checking just in case
		if (activeFrag == nullptr) { return; }
		if (dat->anchors.size() != 0 && lastAnchorArrayIndex != 0)
		{
			if (lastAnchorArrayIndex >= dat->anchors.size()) { std::cout << "SKIP_A" << std::endl; }
			if (dat->anchors.size() < lastAnchorArrayIndex || dat->anchors[lastAnchorArrayIndex - 1].ID != lastAnchorIndex)
			{
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
			//if (dat->anchors[lastAnchorIndex].wait != 0) { waitSize = (size_t)lastAnchorIndex + (size_t)dat->anchors[lastAnchorIndex].wait; }
			//if (dat->anchors.size() > waitSize)
			if (true)
			{
				activeFrag.get()->fragData.anchors.push_back(dat->anchors[lastAnchorArrayIndex]);
				activeFrag.get()->processNewAnchor();
				lastAnchorArrayIndex++;
				lastAnchorIndex = activeFrag.get()->fragData.anchors.back().ID;
			}
		}
	}
	// Handle vertex data that is of a constant size, and also follows a linear input pattern
	else if (dat->constantSize && dat->linearStream)
	{
		int polyCount = dat->anchors.size();
		if (dat->anchors.front().flag == FLAG_NEW_INPUT)
		{
			// Begin new stroke, pass copies of the tool settings to the new stroke
			// Only the settings that the stroke needs to render will get passed. Settings for
			// things like smoothing and correction do not get passed along.
			if (!sender->getUI()->getCanvas()->getActiveLayer().expired())
			{
				activeFrag = sender->getUI()->getCanvas()->getActiveLayer().lock()->createNewStroke(
					sender->getShardShader(), owner);
				std::cout << "OUT_STROKE::CREATENEWSTROKE::CONST_VERTCOUNT= " << std::endl;
				activeFrag.get()->fragData.constantSize = dat->constantSize;
				activeFrag.get()->fragData.linearStream = dat->linearStream;
				activeFrag.get()->fragData.connectEnds = dat->connectEnds;
				lastAnchorIndex = 0;
				activeFrag.get()->fragData.transform = dat->transform;
				activeFrag.get()->fragData.anchors = dat->anchors;
			}
			
		}
		if (lastAnchorIndex != dat->anchors.front().ID)
		{
			// Copy the transform data
			activeFrag.get()->fragData.transform = dat->transform;
			// Copy the vertices
			std::vector<glm::vec3> lineLoop;			
			for (int i = 0; i < dat->anchors.size(); i++) 
			{ 
				activeFrag.get()->fragData.anchors[i] = dat->anchors[i];
				lineLoop.push_back(dat->anchors[i].pos); 
			}
			// Enable the preview-drawing / set the preview vert data
			activeFrag.get()->setLineDraw(lineLoop);
			activeFrag.get()->setBoundsDraw(dat->transform);
			lastAnchorIndex = dat->anchors.front().ID;
		}
		else
		{
			//std::cout << "OUT_STROKE::CONST_VERTCOUNT::INPUT_STREAM_PAUSED" << std::endl;
		}
	}
	// Handle vertex data that may or may not be constant, but follows a non-linear input pattern
	else if (!dat->linearStream)
	{
		// Non-linear stream with either constant or varying size
		if (dat->anchors.front().flag == FLAG_NEW_INPUT)
		{
			if (!sender->getUI()->getCanvas()->getActiveLayer().expired())
			{
				activeFrag = sender->getUI()->getCanvas()->getActiveLayer().lock()->createNewStroke(
					sender->getShardShader(), owner);
				std::cout << "OUT_STROKE::CREATENEWSTROKE::NON_LINEAR_STREAM= " << std::endl;
				lastAnchorIndex = 0;
				activeFrag.get()->fragData.constantSize = dat->constantSize;
				activeFrag.get()->fragData.linearStream = dat->linearStream;
				activeFrag.get()->fragData.connectEnds = dat->connectEnds;
				activeFrag.get()->fragData.transform = dat->transform;
				//activeFrag.get()->fragData.anchors = dat->anchors;
				activeFrag.get()->setBoundsDraw(dat->transform);
			}
			
		}
		if (lastAnchorIndex != dat->anchors.back().ID)
		{
			// Copy the transform data
			//std::cout << vec3VecToString({ dat->transform.bounds.p4 }) << std::endl;
			activeFrag.get()->fragData.transform = dat->transform;
			lastAnchorIndex = dat->anchors.front().ID;
			// Copy the anchors
			std::vector<glm::vec3> lineLoop;
			if (activeFrag.get()->fragData.anchors.size() != dat->anchors.size())
			{
				// For if the size was not constant
				activeFrag.get()->fragData.resetAnchors();
				for (int i = 0; i < dat->anchors.size(); i++)
				{
					activeFrag.get()->fragData.anchors.push_back(dat->anchors[i]);
					lineLoop.push_back(dat->anchors[i].pos);
				}
			}
			else
			{
				// For if the size was constant
				for (int i = 0; i < dat->anchors.size(); i++)
				{
					activeFrag.get()->fragData.anchors[i] = dat->anchors[i];
					lineLoop.push_back(dat->anchors[i].pos);
				}
			}
			// Enable the preview-drawing / set the preview vert data
			activeFrag.get()->setLineDraw(lineLoop);
			activeFrag.get()->setBoundsDraw(dat->transform);
			lastAnchorIndex = dat->anchors.front().ID;
			
		}
		else
		{
			//std::cout << "OUT_STROKE::CONST_VERTCOUNT::INPUT_STREAM_PAUSED" << std::endl;
		}
	}
	return;
}


void Out_Stroke::finalize(Application* sender, VertexData* dat)
{
	// Clear these just in case
	if (activeFrag != nullptr)
	{
		activeFrag.get()->disableBoundsDraw();
		activeFrag.get()->disableLineDraw();
	}
	// Kick bad-calls
	if (dat->anchors.size() == 0 || activeFrag == nullptr) { return; }

	if (!dat->constantSize && dat->linearStream)
	{
		lastAnchorIndex++;
		if (activeFrag.get()->fragData.connectEnds)
		{
			activeFrag.get()->fragData.anchors.push_back(dat->anchors.front());
			activeFrag.get()->processNewAnchor();
		}
		// See comments below for cues as to what might be done for non-constant, linear input
	}
	else if (dat->constantSize && dat->linearStream)
	{
		if (activeFrag == nullptr) { return; }
		int polyCount = dat->anchors.size();
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
			lastAnchorIndex = dat->anchors.front().ID;
		}
		activeFrag.get()->fragData.transform = dat->transform;
		activeFrag.get()->disableBoundsDraw();
		activeFrag.get()->disableLineDraw();
		activeFrag = nullptr;
	}
	else if (!dat->linearStream)
	{
		if (activeFrag == nullptr) { return; }
		int polyCount = dat->anchors.size();
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
			lastAnchorIndex = dat->anchors.front().ID;
		}
		activeFrag.get()->fragData.transform = dat->transform;
		activeFrag.get()->disableBoundsDraw();
		activeFrag.get()->disableLineDraw();
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
	if (abs(anchor1->pressure - anchor2->pressure) > pressureThreshold) { return false; }
	if (abs((anchor1->tiltx + anchor1->tilty) - (anchor2->tiltx + anchor2->tilty)) > tiltThreshold) { return false; }
	if (abs(anchor1->rotation - anchor2->rotation) > rotationThreshold) { return false; }
	if (abs(anchor1->velocity - anchor2->velocity) > velocityThreshold) { return false; }
	if (abs(anchor1->intensity - anchor2->intensity) > intensityThreshold) { return false; }
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
	outAnchor.pressure = (anchor1->pressure + outAnchor.pressure) / 2.0f;
	outAnchor.tiltx = (anchor1->tiltx + outAnchor.tiltx) / 2.0f;
	outAnchor.tilty = (anchor1->tilty + outAnchor.tilty) / 2.0f;
	outAnchor.rotation = (anchor1->rotation + outAnchor.rotation) / 2.0f;
	outAnchor.velocity = (anchor1->velocity + outAnchor.velocity) / 2.0f;
	outAnchor.intensity = (anchor1->intensity + outAnchor.intensity) / 2.0f;
	return outAnchor;
}