#ifndef OUT_PANCAMERA_H
#define OUT_PANCAMERA_H

#include "OutputMethod.h"
#include "../ToolSettings.h"
class Application;

#include <iostream>

class Out_PanCamera : public OutputMethod
{
private:
protected:
	TSet_Pan* pan;
public:
	Out_PanCamera(int id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner } { interestMask = { TSetType::pan }; }
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
};

#endif