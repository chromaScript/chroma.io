#ifndef IN_ROTATE_H
#define IN_ROTATE_H

#include "InputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"

#include <iostream>

class In_Rotate : public InputMethod
{
private:
protected:
	TSet_Rotate rotate;
public:
	In_Rotate(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner } 
	{ 
		this->type = MethodType::rotate;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::usedefault; }
		interestMask = { TSetType::rotate }; 
		fragData.constantSize = true; 
		this->maxBufferLength = 2;
		this->anchorIDCount = 0;
		this->name = "Rotate Camera";
	}
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif