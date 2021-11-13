#ifndef IN_POINT_H
#define IN_POINT_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <iostream>

class In_Point : public InputMethod
{
private:
protected:
public:
	In_Point(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_point;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::drag; }
		interestMask = { TSetType::sampler }; 
		fragData.constantSize = true; 
		this->maxBufferLength = 5;
		this->anchorIDCount = 0;
		this->name = "Point";
	}
	virtual void newInput(Application* sender, Input dat) {}
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
	virtual InputHandlerFlag key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys);
	virtual void initializeVertices(glm::vec3* pos, glm::vec3* dir, Input* dat,
		int waitCountVertex, int waitCountSpline, InputFlag vertexFlagSecondary, InputFlag splineFlagSecondary) {}
	virtual void addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat) {}
	virtual void generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat) {}
	virtual void generateCurve() {}
};

#endif