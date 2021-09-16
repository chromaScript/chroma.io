#include "../include/methods/Out_RotateCamera.h"

#include "../include/Application.h"
#include "../include/Camera.h"
#include "../include/ToolSettings.h"
#include "../include/toolSettings/ToolSettings_Forward.h"
#include "../include/Toolbox.h"
#include "../include/Tool.h"

#include <iostream>

void Out_RotateCamera::preview(Application* sender, VertexData* dat)
{
	if (dat->anchors.size() == 0) { return; }
	rotate = *owner.get()->getRotate();
	glm::vec2 p0 = glm::vec2(dat->transform.origin.x, dat->transform.origin.y);
	glm::vec2 pA = glm::vec2(dat->anchors.front().pos.x, dat->anchors.front().pos.y);
	glm::vec2 pB = glm::vec2(dat->anchors.back().pos.x, dat->anchors.back().pos.y);
	glm::vec2 dirA = glm::normalize(pA - p0);
	glm::vec2 dirB = glm::normalize(pB - p0);
	float angleA = glm::degrees(atan2(dirA.x, dirA.y));
	float angleB = glm::degrees(atan2(dirB.x, dirB.y));
	// 4. Find the amount to add to cameraRoll, then clamp just incase something odd happened
	float increment = clampf((angleA - angleB), -1.0f , 1.0f) * rotate.rotationSpeed;
	//std::cout << "ROTATECAMERA::INCREMENT=" << increment << std::endl;
	if (increment == 0) { return; }
	// 5. Determine if snap controls are enabled
	int snapType = dat->anchors.back().flag;
	if (snapType == INPUT_MOD_NONE) // Free Rotation with 2.5 degree tolerance snapping to 90 degree during finalization
	{
		sender->getCamera()->addRoll(-increment);
	}
	else if (snapType == rotate.snapAngleA_modKey || snapType == rotate.snapAngleB_modKey || snapType == rotate.snapAngleC_modKey) // 45 degree // 15 degree // 5 degree
	{
		// 1. Track the current virtual roll amount
		snapRoll = snapRoll + increment;
		//std::cout << "ROLL::=" << snapRoll << "TYPE::=" << snapType << std::endl;
		float snapIncrement = 45.0f;
		float snapAngle = 0.0f;
		// 2. Set the snap type
		if (snapType == rotate.snapAngleA_modKey)
		{
			snapIncrement = rotate.snapAngleA_angle;
		}
		else if (snapType == rotate.snapAngleB_modKey)
		{
			snapIncrement = rotate.snapAngleB_angle;
		}
		else if (snapType == rotate.snapAngleC_modKey)
		{
			snapIncrement = rotate.snapAngleC_angle;
		}
		// 3. Add the real roll to the virtual roll and divide by the snap angle
		float currentAngle = sender->getCamera()->getRoll();
		float j = (currentAngle + snapRoll) / snapIncrement;
		float jdec = j - floor(j);
		//std::cout << "CURRENTANGLE::=" << currentAngle << "::VAR_J::=" << j << std::endl;
		// 4. If the division result is within range of the next nearest increment 
		if (jdec <= (snapIncrement / 120.0f) || jdec >= 1 - (snapIncrement / 120.0f))
		{
			snapAngle = round(j) * snapIncrement;
			// 5. If the angle to snap to is not the current angle, snap camera to the next increment
			float l = rotate.snapTolerance_factorA + 
				((snapIncrement - TSET_ROTATE_ANGLESNAP_MIN / TSET_ROTATE_ANGLESNAP_MAX) * (1.5f - rotate.snapTolerance_factorA)); // Find the tolerance lerp value. A lower tolerance means it snaps to the next angle faster, important for smaller snapIncrements.
			if (abs(currentAngle - snapAngle) >= snapIncrement / l)
			{
				sender->getCamera()->setRoll(snapAngle);
				snapRoll = 0.0f;
			}
		}
	}
	else
	{
		// Any other input operation type defaults to free rotation;
		sender->getCamera()->addRoll(-increment);
	}
}
void Out_RotateCamera::finalize(Application* sender, VertexData* dat)
{
	float roll = sender->getCamera()->getRoll();
	float rollOrig = roll;
	if (roll >= 87.5f && roll <= 92.5f)
	{
		roll = 90.0f;
		sender->getCamera()->setRoll(roll);
	}
	else if (roll >= 177.5f)
	{
		roll = 180.0f;
		sender->getCamera()->setRoll(roll);
	}
	else if (roll >= -92.5f && roll <= -87.5f)
	{
		roll = -90.0f;
		sender->getCamera()->setRoll(roll);
	}
	else if (roll >= -2.5f && roll <= 2.5f)
	{
		roll = 0.0f;
		sender->getCamera()->setRoll(roll);
	}
	snapRoll = 0.0f;
}
void Out_RotateCamera::postprocess(Application* sender, VertexData* dat)
{

}