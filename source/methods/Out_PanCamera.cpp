#include "../include/methods/Out_PanCamera.h"

#include <glm.hpp>
#include <gtx/rotate_vector.hpp>

#include "../include/Application.h"
#include "../include/Camera.h"
#include "../include/ToolSettings.h"
#include "../include/toolSettings/ToolSettings_Forward.h"
#include "../include/Toolbox.h"
#include "../include/Tool.h"
#include "../include/keys.h"

#include <iostream>
#include <cmath>

void Out_PanCamera::preview(Application* sender, VertexData* dat)
{
	if (dat->anchors.front().flag == FLAG_NEW_INPUT) { pan = sender->toolbox.get()->getActiveTool().get()->getPan(); }
	float x1, x2;
	float y1, y2;
	if (dat->anchors.size() < 2) { return; }
	else
	{
		size_t size = dat->anchors.size();
		x1 = (float)dat->anchors[size - 2].pos.x;
		x2 = (float)dat->anchors[size - 1].pos.x;
		y1 = (float)dat->anchors[size - 2].pos.y;
		y2 = (float)dat->anchors[size - 1].pos.y;
		if (sender->getKeyState(pan->slowModKey / 10) == 1)
		{
			x2 -= (x2 - x1) * (pan->slowFactor / 1.0f);
			y2 -= (y2 - y1) * (pan->slowFactor / 1.0f);
		}
		x2 += (x2 - x1) * pan->speedAmount;
		y2 += (y2 - y1) * pan->speedAmount;
	}
	sender->getCamera()->setPosition(sender->pickScreenCoord(x1, y1), sender->pickScreenCoord(x2, y2));
}
void Out_PanCamera::finalize(Application* sender, VertexData* dat)
{
	/*
	// At the end of panCamera, calculate the speed and direction of the action and allow the camera to deaccelerate after releasing
	int i = (int)dat.inputEvents.size();
	if (i == 0)
	{
		return;
	}
	else
	{
		float t1 = (float)dat.inputEvents.front().time;
		float t2 = (float)dat.end.time;
		float x1 = (float)dat.inputEvents.front().x;
		float x2 = (float)dat.end.x;
		float y1 = (float)dat.inputEvents.front().y;
		float y2 = (float)dat.end.y;
		glm::vec2 dir = glm::vec2(x1 - x2, y2 - y1);
		glm::vec3 norm = glm::normalize(glm::vec3(dir.x, dir.y, 0.0f));
		if (isnan(norm.x) || isnan(norm.y))
		{
			dir = glm::vec2(0.0f, 0.0f);
		}
		else
		{
			dir = glm::vec2(norm.x, norm.y);
		}
		// Momentum is equal to c = sqrt(a^2 + b^2) / time elapsed
		float momentum = 1.0f * (sqrtf(powf((x1 - x2), 2) + powf((y2 - y1), 2)) / (t2 - t1));
		float z = sender->getCamera()->getOrthoZoom();
		if (z >= 1500.0f)
		{
			z = 1500.0f;
		}
		if (z <= 50.f)
		{
			z = 50.f;
		}
		// Modulate (lerp) momentum by zoom level to reduce speed when zoomed in
		momentum = momentum * (0.05f + ((z / 1500) * (1.0f - 0.05f)));
		sender->getCamera()->addMomentum(dir, momentum);
		std::cout << "PANCAMERA::MOMENTUM::X=" << dir.x << "::Y=" << dir.y << "::AMOUNT=" << momentum << std::endl;
	}
	*/
}
void Out_PanCamera::postprocess(Application* sender, VertexData* dat)
{

}