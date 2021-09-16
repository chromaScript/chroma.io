#ifndef IN_ZOOM_H
#define IN_ZOOM_H

#include "InputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"

#include <iostream>

class In_Zoom : public InputMethod
{
private:
protected:
	TSet_Zoom zoom;
public:
	In_Zoom(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner } 
	{ 
		this->type = MethodType::zoom;
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::usedefault; }
		interestMask = { TSetType::zoom };
		this->maxBufferLength = 3;
		this->anchorIDCount = 0;
		this->name = "Zoom Camera";
	}
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif