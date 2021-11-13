#ifndef OUT_SAMPLER_H
#define OUT_SAMPLER_H

#include "../OutputMethod.h"
#include "../../tool/ToolSettings.h"
#include "../../tool/toolSettings/ToolSettings_Forward.h"
#include "../MethodType.h"

class Application;

#include <iostream>

class Out_Sampler : public OutputMethod
{
private:
protected:
	int lastSeenAnchorID = -1;
	TSet_Sampler sampler;
public:
	Out_Sampler(MethodType id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner }
	{ 
		this->type = MethodType::out_sampler;
		interestMask = { TSetType::sampler };
		this->name = "Color Sample";
	}
	virtual void preview(Application* sender, VertexData* dat, VertexData* splineData, InputHandlerFlag action);
	virtual void finalize(Application* sender, VertexData* dat, VertexData* splineData);
	virtual void postprocess(Application* sender, VertexData* dat, VertexData* splineData);
};

#endif