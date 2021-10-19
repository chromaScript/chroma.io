#ifndef IN_NOINPUT_H
#define IN_NOINPUT_H

#include "../InputMethod.h"
#include "../MethodType.h"

#include <iostream>

class In_NoInput : public InputMethod
{
private:
protected:
public:
	In_NoInput(MethodType id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::in_noInput;
		this->name = "No Input";
	}
	virtual InputHandlerFlag move(Application* sender, Input dat)
	{
		std::cout << "IN_NOINPUT::MOVE" << std::endl;
		return InputHandlerFlag::reject;
	}
	virtual InputHandlerFlag click(Application* sender, Input dat)
	{
		std::cout << "IN_NOINPUT::CLICK::TIME=" << dat.time << "::TYPE=" << static_cast<int>(dat.modKey) << std::endl;
		return InputHandlerFlag::reject;
	}
};

#endif