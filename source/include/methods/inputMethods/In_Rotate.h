#ifndef IN_ROTATE
#define IN_ROTATE

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <iostream>

class In_Rotate : public InputMethod
{
private:
protected:
	TSet_Rotate rotate;
public:
	In_Rotate(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{ 
		this->type = MethodType::in_rotate;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::usedefault; }
		interestMask = { TSetType::rotate }; 
		fragData.constantSize = true; 
		this->maxBufferLength = 3;
		this->anchorIDCount = 0;
		this->name = "Rotate Camera";
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