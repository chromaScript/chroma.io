#ifndef OUT_PANCAMERA_H
#define OUT_PANCAMERA_H

#include "OutputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"
class Application;

#include <iostream>

class Out_PanCamera : public OutputMethod
{
private:
protected:
	TSet_Pan* pan;
public:
	Out_PanCamera(int id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner } 
	{ 
		this->type = MethodType::pan;
		interestMask = { TSetType::pan }; 
		this->name = "Camera Pan";
	}
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
};

#endif