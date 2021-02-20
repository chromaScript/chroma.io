#ifndef IN_LASSO_H
#define IN_LASSO_H

#include "InputMethod.h"
#include "../ToolSettings.h"

#include <glm.hpp>
class Application;

#include <iostream>

class In_Lasso : public InputMethod
{
private:
protected:
public:
	In_Lasso(int id, TSetType controlScheme, std::shared_ptr<Tool> owner) : InputMethod{ id, controlScheme, owner }
	{
		interestMask = { TSetType::basic, TSetType::image, TSetType::alpha };
		fragData.constantSize = false;
		fragData.linearStream = true;
		fragData.centerAboutOrigin = false;
		fragData.connectEnds = true;
		this->maxBufferLength = 5;
		this->anchorIDCount = 0;
	};
	virtual int move(Application* sender, MouseEvent dat);
	virtual int click(Application* sender, MouseEvent dat);
};

#endif