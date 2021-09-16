#include "../include/methods/Out_ZoomCamera.h"

#include <glm.hpp>

#include "../include/Application.h"
#include "../include/Camera.h"
#include "../include/Toolbox.h"
#include "../include/toolSettings/ToolSettings_Forward.h"
#include "../include/entities/UserInterface.h"
#include "../include/ToolSettings.h"
#include "../include/Tool.h"

#include <iostream>

void Out_ZoomCamera::preview(Application* sender, VertexData* dat)
{
	
	// Kick bad-calls. In_Zoom should always send at least the origin + 1 anchor point
	if (dat->anchors.empty() == true) { return; }
	// Get the settings
	if (dat->anchors.front().flag == FLAG_NEW_INPUT) { zoom = *sender->toolbox.get()->getActiveTool().get()->getZoom(); }
	// Use preview to handle scrub-zoom
	float y1 = dat->anchors.front().pos.y;
	float y2 = dat->anchors.back().pos.y;
	// Modulate zoom speed by current zoom level. A lower zoom means slower to reach max
	float camZoom = sender->getCamera()->getOrthoZoom();
	float f = clampf(camZoom, zoom.minZoomFactor, zoom.maxZoomFactor);
	float mod = zoom.zoomFactor_A + ((f / zoom.maxZoomFactor) * (1.0f - zoom.zoomFactor_A));
	camZoom = camZoom + ((mod * (y1 - y2)) / zoom.zoomSpeed);
	// ZoomCamera manually sets the cursor when using scrub-zoom
	if ((y1 - y2) >= 0.0f)
	{
		sender->getUI()->updateCursorImage(sender->getToolbox()->getActiveTool()->getCursorUp());
	}
	else
	{
		sender->getUI()->updateCursorImage(sender->getToolbox()->getActiveTool()->getCursorDown());
	}
	sender->getCamera()->setOrthoZoom(camZoom);
}
void Out_ZoomCamera::finalize(Application* sender, VertexData* dat)
{
	// Use finalize to handle click zoom
	float y1 = dat->transform.origin.y;
	float y2 = dat->anchors.back().pos.y;
	bool isStaticClick = true;
	// Veryify by checking start/end and start flag that the click ended approximately where it started
	if (dat->anchors.size() >= 2 || dat->anchors.back().flag == FLAG_SCRUB || abs(y1 - y2) > zoom.scrubThreshold)
	{
		isStaticClick = false;
	}
	if (isStaticClick)
	{
		// Resolve the click event
		// 1. Get the current zoom, set the lerp & sign modifier, then finalize the zoom to send
		float camZoom = sender->getCamera()->getOrthoZoom();
		float f = clampf(camZoom, zoom.minZoomFactor, zoom.maxZoomFactor);
		float mod = sqrt(0.01f + ((f / zoom.maxZoomFactor) * (1.0f - 0.01f)));
		float sign = -dat->anchors.back().dirInterpFactor;
		camZoom = camZoom + (sign * mod * zoom.stepZoomAmount);
		sender->getCamera()->setOrthoZoom(camZoom);
		// 2. Get the center of the window, find the direction, then addOffset to position closer to zoom target
		// Suggestions: This function is very rudimentary in calculation of offset target to zoom to, consider
		// reworking this later to properly zoom towards the point on canvas.
		float camX = sender->getCamera().get()->getPosition().x;
		float camY = sender->getCamera().get()->getPosition().y;
		float clickX = dat->transform.origin.x - sender->getCamera().get()->getCenterPosition().x;
		float clickY = dat->transform.origin.y - sender->getCamera().get()->getCenterPosition().y;
		glm::vec2 dir = glm::vec2(camX - clickX, camY - clickY);
		dir *= 0.6 * sign;
		if (isnan(dir.x) || isnan(dir.y)) { return; }
		else
		{
			sender->getCamera()->setPosition(dir.x, dir.y);
		}
		
	}
	else { return; }
}
void Out_ZoomCamera::postprocess(Application* sender, VertexData* dat)
{

}