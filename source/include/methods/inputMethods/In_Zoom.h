#ifndef IN_ZOOM_H
#define IN_ZOOM_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <iostream>

class In_Zoom : public InputMethod
{
private:
protected:
	TSet_Zoom zoom;
public:
	In_Zoom(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{ 
		this->type = MethodType::in_zoom;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::usedefault; }
		interestMask = { TSetType::zoom };
		this->maxBufferLength = 3;
		this->anchorIDCount = 0;
		this->name = "Zoom Camera";
	}
	virtual void newInput(Application* sender, Input dat) {}
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
	virtual InputHandlerFlag key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys);
	virtual void addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat) {}
	virtual void generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat) {}
	virtual void generateCurve() {}
};

#endif