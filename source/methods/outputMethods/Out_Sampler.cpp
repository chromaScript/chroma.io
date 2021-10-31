#include "../../include/methods/outputMethods/Out_Sampler.h"
#include "../../include/tool/Toolbox.h"
#include "../../include/tool/ToolSettings.h"
#include "../../include/tool/toolSettings/ToolSettings_Forward.h"
#include "../../include/math/Color.h"

#ifndef APPLICATION_H
#include "../../include/Application.h"
#endif
#include "../../include/entities/UserInterface.h"

#include <iostream>

void Out_Sampler::preview(Application* sender, VertexData* dat, InputHandlerFlag action)
{
	// Kick bad calls
	if (dat->anchors.size() < 2) { return; }
	if (dat->anchors.front().input.flagPrimary == InputFlag::newInput) { 
		sampler = *sender->toolbox.get()->getActiveTool().get()->getSampler();
		lastSeenAnchorID = -1; 
	}
	else
	{
		// Exit if the anchors weren't updated
		if (dat->anchors.front().ID <= lastSeenAnchorID) { return; }
	}
	// Get the position data, the screen coordinates are found in the second anchor
	int x1 = (int)dat->anchors[1].pos.x;
	int y1 = (int)dat->anchors[1].pos.y;
	CColor sample = black;
	if (sampler.shapeType == TSetProp::point)
	{
		if (sampler.sampleType == TSetProp::allLayers)
		{
			sample = sender->sampleScreen(x1, y1);
		}
	}
	else if (sampler.shapeType == TSetProp::radius)
	{
		if (sampler.sampleType == TSetProp::allLayers)
		{
			sample = sender->sampleScreen(x1, y1, sampler.radius);
		}
	}
	else if (sampler.shapeType == TSetProp::shape)
	{

	}
	else if (sampler.shapeType == TSetProp::tip)
	{

	}

	CColor fgColor = sample;
	CColor bgColor = sender->ui.get()->bgColor;
	// Modulate Color samples
	sampler.modulateColor(fgColor, bgColor, &sample, 
		&dat->anchors.back().pos, &dat->transform.origin, &dat->anchors.back().dir, &dat->anchors.back().input,
		&dat->anchors.back().ID, &dat->anchors.back().ID, &dat->anchors.back().ID);
	// Send the FG Color Sample
	if (dat->activeModKey == sampler.sampleBGModKey.modKey) {
		sender->ui.get()->updateBGColor(fgColor, x1, y1);
		if (sampler.useAutoBGUpdater) {
			sender->ui.get()->updateFGColor(bgColor, x1, y1);
		}
	}
	else { 
		sender->ui.get()->updateFGColor(fgColor, x1, y1);
		if (sampler.useAutoBGUpdater) {
			sender->ui.get()->updateBGColor(bgColor, x1, y1);
		}
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