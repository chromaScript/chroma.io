#ifndef OUT_SAMPLER_H
#define OUT_SAMPLER_H

#include "OutputMethod.h"
#include "../ToolSettings.h"
#include "../toolSettings/ToolSettings_Forward.h"
class Application;

#include <iostream>

class Out_Sampler : public OutputMethod
{
private:
protected:
	int lastSeenAnchorID = -1;
	TSet_Sampler sampler;
public:
	Out_Sampler(int id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner } 
	{ 
		this->type = MethodType::sampler;
		interestMask = { TSetType::sampler };
		this->name = "Color Sample";
	}
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
};

#endif