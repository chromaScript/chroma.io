#ifndef OUT_ROTATECAMERA_H
#define OUT_ROTATECAMERA_H

#include "../OutputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

class Application;

class Out_RotateCamera : public OutputMethod
{
private:
protected:
	float initialRoll = 0.0f;
	float initialAngle = 0.0f;
	float snapRoll = -720.0f;
	TSet_Rotate rotate;
public:
	Out_RotateCamera(MethodType id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner }
	{ 
		this->type = MethodType::out_rotate;
		interestMask = { TSetType::rotate };
		this->name = "Camera Rotate";
	}
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
};

#endif