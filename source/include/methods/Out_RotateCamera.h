#ifndef OUT_ROTATECAMERA_H
#define OUT_ROTATECAMERA_H

#include "OutputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"
class Application;

class Out_RotateCamera : public OutputMethod
{
private:
protected:
	float snapRoll = 0.0f;
	TSet_Rotate rotate;
public:
	Out_RotateCamera(int id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner } 
	{ 
		this->type = MethodType::rotate;
		interestMask = { TSetType::rotate };
		this->name = "Camera Rotate";
	}
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
};

#endif