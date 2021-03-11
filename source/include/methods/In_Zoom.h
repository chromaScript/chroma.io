#ifndef IN_ZOOM_H
#define IN_ZOOM_H

#include "InputMethod.h"
#include "../ToolSettings.h"

#include <iostream>

class In_Zoom : public InputMethod
{
private:
protected:
	TSet_Zoom zoom;
public:
	In_Zoom(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner } 
	{ 
		if (this->controlScheme == TSetType::usedefault) { this->controlScheme = TSetType::usedefault; }
		interestMask = { TSetType::zoom };
		this->maxBufferLength = 3;
		this->anchorIDCount = 0;
	}
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif