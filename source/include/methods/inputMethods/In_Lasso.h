#ifndef IN_LASSO_H
#define IN_LASSO_H

#include "../InputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_Lasso : public InputMethod
{
private:
protected:
public:
	In_Lasso(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_lasso;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::continuous; }
		interestMask = { TSetType::basic, TSetType::image, TSetType::alpha };
		fragData.constantSize = false;
		fragData.linearStream = true;
		fragData.centerAboutOrigin = false;
		fragData.connectEnds = true;
		this->maxBufferLength = 5;
		this->anchorIDCount = 0;
		this->name = "Lasso";
	};
	virtual InputHandlerFlag move(Application* sender, Input dat);
	virtual InputHandlerFlag click(Application* sender, Input dat);
	virtual InputHandlerFlag key(Application* sender, Input dat, Keybind key, InputAction action, InputModKey modKeys);
};

#endif