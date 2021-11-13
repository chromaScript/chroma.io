#ifndef OUT_NOOUTPUT_H
#define OUT_NOOUTPUT_H

#include "../OutputMethod.h"
class Application;

#include <iostream>

class Out_NoOutput : public OutputMethod
{
private:
protected:
public:
	Out_NoOutput(MethodType id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner }
	{ 
		this->type = MethodType::out_noOutput;
		this->name = "No Output"; 
	};
	virtual void preview(Application* sender, VertexData* dat, VertexData* splineData, InputHandlerFlag action);
	virtual void finalize(Application* sender, VertexData* dat, VertexData* splineData);
	virtual void postprocess(Application* sender, VertexData* dat, VertexData* splineData);
};

#endif