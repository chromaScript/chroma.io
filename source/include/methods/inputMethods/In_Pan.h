#ifndef IN_PAN_H
#define IN_PAN_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <iostream>

class In_Pan : public InputMethod
{
private:
protected:
	bool lockModeActivated = false;
	float activeAngle = -720.0f;
	glm::vec3 constraintOrigin = glm::vec3(0.0f);
	glm::vec3 constraintDir = DEFAULT_DIR;
	glm::vec3 constraintDirPerpendicular = DEFAULT_DIR;
	glm::vec4 constraintLine = glm::vec4(0.0f);
	Keybind activeKey = Keybind();
	TSet_Pan pan;
public:
	In_Pan(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{ 
		this->type = MethodType::in_pan;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::usedefault; }
		interestMask = { TSetType::pan }; 
		fragData.constantSize = true;
		fragData.centerAboutOrigin = fragData.connectEnds = false;
		this->maxBufferLength = 5;
		this->anchorIDCount = 0;
		this->name = "Pan Camera";
	}
	void clearConstraint();
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
	virtual InputHandlerFlag key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys);
};

#endif