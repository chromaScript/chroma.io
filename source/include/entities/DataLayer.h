#ifndef DATALAYER_H
#define DATALAYER_H

#include "Layer.h"
#include "../Color.h"

class DataLayer : public Layer
{
private:
protected:
public:
	DataLayer(glm::ivec2 dimensions, std::string name, CColor fill, std::shared_ptr<Shader> shader);
	~DataLayer() {}

	virtual void render(ShaderTransform xform, unsigned int targetBuffer);
	virtual void draw(ShaderTransform xform);
};

#endif