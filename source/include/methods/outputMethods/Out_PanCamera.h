#ifndef OUT_PANCAMERA_H
#define OUT_PANCAMERA_H

#include "../OutputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../../entities/visuals/VertexData.h"
#include "../MethodType.h"

class Application;

#include <iostream>

class Out_PanCamera : public OutputMethod
{
private:
protected:
	TSet_Pan pan;
public:
	Out_PanCamera(MethodType id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner }
	{ 
		this->type = MethodType::out_pan;
		interestMask = { TSetType::pan }; 
		this->name = "Camera Pan";
	}
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
};

#endif