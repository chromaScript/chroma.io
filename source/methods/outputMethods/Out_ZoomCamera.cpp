#include "../../include/methods/outputMethods/Out_ZoomCamera.h"

#include <glm.hpp>

#include "../../include/Application.h"
#include "../../include/Camera.h"
#include "../../include/tool/Toolbox.h"
#include "../../include/tool/toolSettings/ToolSettings_Forward.h"
#include "../../include/entities/UserInterface.h"
#include "../../include/tool/ToolSettings.h"
#include "../../include/tool/Tool.h"

#include <iostream>

void Out_ZoomCamera::preview(Application* sender, VertexData* dat, VertexData* splineData, InputHandlerFlag action)
{
	
	// Kick bad-calls. In_Zoom should always send at least the origin + 1 anchor point
	if (dat->anchors.empty() == true) { return; }
	// Get the settings
	if (dat->anchors.front().input.flagPrimary == InputFlag::newInput) 
	{ 
		zoom = *sender->toolbox.get()->getActiveTool().get()->getZoom(); 
	}
	// Use preview to handle scrub-zoom
	float camRoll = sender->getCamera()->getRoll();
	glm::vec3 p1 = glm::vec3(dat->anchors.front().pos.x, dat->anchors.front().pos.y, 0.0f);
	glm::vec3 p2 = glm::vec3(dat->anchors.back().pos.x, dat->anchors.back().pos.y, 0.0f);
	float length = glm::length(p2 - p1);
	glm::vec3 dir = glm::normalize(p2 - p1);
	dir = dir * glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(-camRoll)));
	length *= (dir.y < 0.0f) ? 1.0f : -1.0f;
	// Modulate zoom speed by current zoom level. A lower zoom means slower to reach max
	float camZoom = sender->getCamera()->getOrthoZoom();
	float maxZoom = sender->getCamera()->getMaxZoom() / 5.0f;
	float ratio = (dir.y < 0.0f) ? pow(camZoom / maxZoom, zoom.accelerationOut) : pow(camZoom / maxZoom, zoom.accelerationIn);
	//float f = clampf(camZoom, zoom.minZoomFactor, zoom.maxZoomFactor);
	//float mod = zoom.zoomFactor_A + ((f / zoom.maxZoomFactor) * (1.0f - zoom.zoomFactor_A));
	//camZoom = camZoom + ((mod * (length)) / zoom.zoomSpeed);
	camZoom = camZoom + (length * zoom.zoomSpeed * (ratio));
	// ZoomCamera manually sets the cursor when using scrub-zoom
	if ((dir.y) >= 0.0f)
	{
		sender->getUI()->updateCursorImage(sender->getToolbox()->getActiveTool()->getCursorUp());
	}
	else
	{
		sender->getUI()->updateCursorImage(sender->getToolbox()->getActiveTool()->getCursorDown());
	}
	sender->getCamera()->setOrthoZoom(camZoom);
}
void Out_ZoomCamera::finalize(Application* sender, VertexData* dat, VertexData* splineData)
{
	if (dat->anchors.size() == 0) { return; }
	std::shared_ptr<Camera> camera = sender->getCamera();
	float camRoll = camera->getRoll();
	glm::vec3 p1 = glm::vec3(dat->transform.origin.x, dat->transform.origin.y, 0.0f);
	glm::vec3 p2 = glm::vec3(dat->anchors.back().pos.x, dat->anchors.back().pos.y, 0.0f);
	float length = glm::length(p2 - p1);
	glm::vec3 dir = glm::normalize(p2 - p1);
	dir = dir * glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(-camRoll)));
	//length *= (dir.y < 0.0f) ? 1.0f : -1.0f;
	// Use finalize to handle click zoom
	float y1 = dat->transform.origin.y;
	float y2 = dat->anchors.back().pos.y;
	bool isStaticClick = true;
	// Veryify by checking start/end and start flag that the click ended approximately where it started
	//if (dat->anchors.size() >= 2 || dat->anchors.back().input.flagPrimary == InputFlag::scrub || abs(y1 - y2) > zoom.scrubThreshold)
	if (dat->anchors.size() >= 2 || dat->anchors.back().input.flagSecondary == InputFlag::scrub || length > zoom.scrubThreshold)
	{
		isStaticClick = false;
	}
	if (isStaticClick)
	{
		if (zoom.zoomSteps.size() < 2) { return; }
		// Resolve the click event
		// 1. Get the current zoom, set the lerp & sign modifier, then finalize the zoom to send
		float maxZoom = camera->getMaxZoom();
		float camZoom = camera->getOrthoZoom();
		glm::ivec2 dimensions = sender->getCanvasDimensions();
		camZoom = (camZoom / (float(max(dimensions.x, dimensions.y)) / 2.0f)) * 100.0f;
		float sign = -dat->anchors.back().dirInterpFactor;
		float zoomLast = 0.0f; float zoomNext = 0.0f;
		float zoomSelect = 0.0f;
		int i = 0;
		for (i = 0; i < zoom.zoomSteps.size() - 1; i++) {
			if (camZoom >= zoom.zoomSteps[i] && camZoom < zoom.zoomSteps[size_t(i) + 1]) {
				break;
			}
		}
		if (sign > 0.0f) {
			if (i + 1 > zoom.zoomSteps.size() - 1) { 
				zoomSelect = zoom.zoomSteps.back();
			}
			else { zoomSelect = zoom.zoomSteps[size_t(i) + 1]; }
		}
		else {
			if (i - 1 < 0) { 
				zoomSelect = zoom.zoomSteps.front(); 
			}
			else { zoomSelect = zoom.zoomSteps[size_t(i) - 1]; }
		}
		zoomSelect /= 100.0f;
		zoomSelect *= (float(max(dimensions.x, dimensions.y)) / 2.0f);
		camera->setOrthoZoom(zoomSelect);
		// 2. Get the center of the window, find the direction, then addOffset to position closer to zoom target
		// Suggestions: This function is very rudimentary in calculation of offset target to zoom to, consider
		// reworking this later to properly zoom towards the point on canvas.
		if (zoom.clickTarZoom) {
			glm::vec3 camPos = camera->getPosition();
			glm::vec3 centerPos = camera->getCenterPosition();
			glm::vec3 clickPos = dat->transform.origin - centerPos;
			clickPos = clickPos * glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(-camRoll)));
			centerPos.z = camPos.z = clickPos.z = 0.0f;
			glm::vec3 dstPos = glm::normalize(clickPos - camPos) * 
				(glm::length(clickPos - camPos) * 0.35f * dat->anchors.back().dirInterpFactor);
			camera->setPosition(dstPos.x, dstPos.y);
		}
	}
	else { return; }
}
void Out_ZoomCamera::postprocess(Application* sender, VertexData* dat, VertexData* splineData)
{

}