#ifndef IN_PAN_H
#define IN_PAN_H

#include "InputMethod.h"
#include "../ToolSettings.h"

#include <iostream>

class In_Pan : public InputMethod
{
private:
protected:
	TSet_Pan pan;
public:
	In_Pan(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner } 
	{ 
		interestMask = { TSetType::pan }; 
		fragData.constantSize = true;
		fragData.centerAboutOrigin = fragData.connectEnds = false;
		this->maxBufferLength = 5;
		this->anchorIDCount = 0;
	}
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif