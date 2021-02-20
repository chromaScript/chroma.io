#ifndef OUT_ZOOMCAMERA_H
#define OUT_ZOOMCAMERA_H

#include "OutputMethod.h"
#include "../ToolSettings.h"
class Application;

#include <iostream>

class Out_ZoomCamera : public OutputMethod
{
private:
protected:
	TSet_Zoom zoom;
public:
	Out_ZoomCamera(int id, std::shared_ptr<Tool> owner) : OutputMethod{ id, owner } { interestMask = { TSetType::zoom }; }
	virtual void preview(Application* sender, VertexData* dat);
	virtual void finalize(Application* sender, VertexData* dat);
	virtual void postprocess(Application* sender, VertexData* dat);
};

#endif