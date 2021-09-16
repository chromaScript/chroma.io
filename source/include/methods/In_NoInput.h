#ifndef IN_NOINPUT_H
#define IN_NOINPUT_H

#include "InputMethod.h"

#include <iostream>

class In_NoInput : public InputMethod
{
private:
protected:
public:
	In_NoInput(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		this->type = MethodType::noInput;
		this->name = "No Input";
	}
	virtual int move(Application* sender, MouseEvent dat)
	{
		std::cout << "IN_NOINPUT::MOVE" << std::endl;
		return 0;
	}
	virtual int click(Application* sender, MouseEvent dat)
	{
		std::cout << "IN_NOINPUT::CLICK::TIME=" << dat.time << "::TYPE=" << dat.modKey << std::endl;
		return 0;
	}
};

#endif