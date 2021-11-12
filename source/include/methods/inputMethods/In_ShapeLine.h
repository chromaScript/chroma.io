#ifndef IN_SHAPELINE_H
#define IN_SHAPELINE_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_ShapeLine : public InputMethod
{
private:
	// Tool Settings
	TSet_Polygon polygon;
protected:
public:
	In_ShapeLine(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_shapeLine;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::drag; }
		interestMask = { 
			TSetType::basic, 
			TSetType::image, 
			TSetType::character,
			TSetType::polygon, 
			TSetType::alpha,
			TSetType::color,
			TSetType::scatter,
			TSetType::effects
		};
		fragData.constantSize = false;
		fragData.linearStream = false;
		fragData.connectEnds = true;
		this->maxBufferLength = 10;
		this->anchorIDCount = 0;
		this->name = "Shape Line";
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