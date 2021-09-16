#ifndef OUT_ZOOMCAMERA_H
#define OUT_ZOOMCAMERA_H

#include "OutputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"
class Application;

#include <iostream>

class Out_ZoomCamera : public OutputMethod
{
private:
protected:
	TSet_Zoom zoom;
public:
	Out_ZoomCamera(int id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner } 
	{ 
		this->type = MethodType::zoom;
		interestMask = { TSetType::zoom };
		this->name = "Camera Zoom";
	}
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
};

#endif