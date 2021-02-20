#ifndef OUT_NOOUTPUT_H
#define OUT_NOOUTPUT_H

#include "OutputMethod.h"
class Application;

#include <iostream>

class Out_NoOutput : public OutputMethod
{
private:
protected:
public:
	Out_NoOutput(int id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner } {};
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
};

#endif