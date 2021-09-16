#include "../include/methods/Out_Sampler.h"
#include "../include/Toolbox.h"
#include "../include/ToolSettings.h"
#include "../include/toolSettings/ToolSettings_Forward.h"
#include "../include/Color.h"

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif
#include "../include/entities/UserInterface.h"

#include <iostream>

void Out_Sampler::preview(Application* sender, VertexData* dat)
{
	// Kick bad calls
	if (dat->anchors.size() < 2) { return; }
	if (dat->anchors.front().flag == FLAG_NEW_INPUT) { lastSeenAnchorID = -1; }
	else
	{
		// Exit if the anchors weren't updated
		if (dat->anchors.front().ID <= lastSeenAnchorID) { return; }
	}
	// Get the position data, the screen coordinates are found in the second anchor
	int x1 = (int)dat->anchors[1].pos.x;
	int y1 = (int)dat->anchors[1].pos.y;
	TSet_Sampler* samplerSettings = sender->toolbox.get()->getActiveTool().get()->getSampler();
	CColor sample = black;
	if (samplerSettings->shapeType == TSetProp::point)
	{
		if (samplerSettings->pointRandAmount != 0)
		{

		}
		else
		{
			if (samplerSettings->sampleType == TSetProp::allLayers)
			{
				sample = sender->sampleScreen(x1, y1);
			}
			else
			{

			}
		}
		
	}
	else if (samplerSettings->shapeType == TSetProp::radius)
	{

	}
	else if (samplerSettings->shapeType == TSetProp::shape)
	{

	}
	else if (samplerSettings->shapeType == TSetProp::tip)
	{

	}
	// Check for FGVariance
	if (samplerSettings->useFGVariance)
	{
		// Modify the sample color accordingly
	}
	// Send the FG Color Sample
	if (dat->activeModKey == samplerSettings->sampleBGModKey) { sender->ui.get()->updateBGColor(sample, x1, y1); }
	else { sender->ui.get()->updateFGColor(sample, x1, y1); }

	// Do BG Color matching if enabled
	if (samplerSettings->useAutoBGUpdater && dat->activeModKey != samplerSettings->sampleBGModKey)
	{
		CColor bgSample = sample;
		// Modify the BG Color

		// Send the BG Color sample
		sender->ui.get()->updateBGColor(sample, x1, y1);
	}

	return;
}
void Out_Sampler::finalize(Application* sender, VertexData* dat)
{
	return;
}
void Out_Sampler::postprocess(Application* sender, VertexData* dat)
{
	return;
}