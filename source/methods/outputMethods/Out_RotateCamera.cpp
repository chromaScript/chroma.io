#include "../../include/methods/outputMethods/Out_RotateCamera.h"

#include "../../include/Application.h"
#include "../../include/Camera.h"
#include "../../include/tool/ToolSettings.h"
#include "../../include/tool/toolSettings/ToolSettings_Forward.h"
#include "../../include/tool/Toolbox.h"
#include "../../include/tool/Tool.h"

#include <iostream>

void Out_RotateCamera::preview(Application* sender, VertexData* dat, VertexData* splineData, InputHandlerFlag action)
{
	if (dat->anchors.empty()) { return; }
	std::shared_ptr<Camera> camera = sender->getCamera();
	if (dat->anchors.front().input.flagPrimary == InputFlag::newInput)
	{
		rotate = *owner.get()->getRotate();
		initialRoll = camera->getRoll();
	}
	if (glm::length(dat->anchors.back().pos - dat->transform.origin) < 25.0f) { return; }
	// 1. Find the amount to set to cameraRoll
	glm::vec3 p0 = dat->transform.origin;
	glm::vec3 p1 = dat->anchors.back().pos;
	glm::vec3 dir = glm::normalize(p1 - p0);
	float angle = std::atan2f(dir.y, dir.x) * (180 / MATH_PI);
	float camRoll = camera->getRoll();
	if (dat->anchors.front().input.flagPrimary == InputFlag::newInput) {
		initialAngle = angle;
	}
	float outRoll = initialRoll - ((angle - initialAngle) * rotate.rotationSpeed);
	// 2. Determine if snap controls are enabled
	InputModKey snapType = dat->anchors.back().input.modKey;
	if (snapType == InputModKey::none) // Free Rotation with 2.5 degree tolerance snapping to 90 degree during finalization
	{
		camera.get()->setRoll(outRoll);
	}
	else if (snapType == rotate.snapAngleA_modKey.modKey || 
		snapType == rotate.snapAngleB_modKey.modKey || 
		snapType == rotate.snapAngleC_modKey.modKey)
	{
		// 1. Track the current virtual roll amount
		
		float snapIncrement = 45.0f;
		float snapAngle = 0.0f;
		// 2. Set the snap type
		if (snapType == rotate.snapAngleA_modKey.modKey) {
			snapIncrement = rotate.snapAngleA_angle;
		}
		else if (snapType == rotate.snapAngleB_modKey.modKey) {
			snapIncrement = rotate.snapAngleB_angle;
		}
		else if (snapType == rotate.snapAngleC_modKey.modKey) {
			snapIncrement = rotate.snapAngleC_angle;
		}
		// 3. Add the real roll to the virtual roll and divide by the snap angle
		float j = outRoll / snapIncrement;
		float rnd = round(j);
		// 4. If the division result is within range of the next nearest increment 
		if (fract(j) <= (rotate.snapTolerance_factorA / 10.0f) || 
			fract(j) >= 1.0f - (rotate.snapTolerance_factorA / 10.0f)) {
			outRoll = snapIncrement * floor(j);
			camera.get()->setRoll(outRoll);
			snapRoll = outRoll;
		}
		else if (snapRoll <= -715.0f && 
			(fract(j) <= 0.3f || fract(j) >= 0.3f)) {
			outRoll = snapIncrement * rnd;
			camera.get()->setRoll(outRoll);
			snapRoll = outRoll;
		}
	}
	else {
		camera.get()->setRoll(outRoll);
	}
}
void Out_RotateCamera::finalize(Application* sender, VertexData* dat, VertexData* splineData)
{
	float roll = sender->getCamera()->getRoll();
	float rollOrig = roll;
	if (roll >= 87.5f && roll <= 92.5f) {
		roll = 90.0f;
		sender->getCamera()->setRoll(roll);
	}
	else if (roll >= 177.5f) {
		roll = 180.0f;
		sender->getCamera()->setRoll(roll);
	}
	else if (roll >= -92.5f && roll <= -87.5f) {
		roll = -90.0f;
		sender->getCamera()->setRoll(roll);
	}
	else if (roll >= -2.5f && roll <= 2.5f) {
		roll = 0.0f;
		sender->getCamera()->setRoll(roll);
	}
	snapRoll = -720.0f;
}
void Out_RotateCamera::postprocess(Application* sender, VertexData* dat, VertexData* splineData)
{

}