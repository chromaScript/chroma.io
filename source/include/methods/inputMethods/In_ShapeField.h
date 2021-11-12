#ifndef IN_SHAPEFIELD_H
#define IN_SHAPEFIELD_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_ShapeField : public InputMethod
{
private:
	// Tool Settings
	TSet_Polygon polygon;
	TSet_Field field;
protected:
public:
	In_ShapeField(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_shapeField;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::onepoint; }
		interestMask = { 
			TSetType::basic, 
			TSetType::image,
			TSetType::character,
			TSetType::polygon, 
			TSetType::field, 
			TSetType::alpha,
			TSetType::color,
			TSetType::scatter,
			TSetType::effects
		};
		fragData.constantSize = false;
		fragData.linearStream = false;
		this->maxBufferLength = 10;
		this->anchorIDCount = 0;
		this->name = "Shape Field";
	};
	virtual void newInput(Application* sender, Input dat);
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
	virtual InputHandlerFlag key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys);
	virtual void addVertices(glm::vec3* pos, glm::vec3* dir, Input* dat);
	virtual void generateVertices(glm::vec3* pos, glm::vec3* dir, Input* dat);
	virtual void generateCurve();
};

#endif