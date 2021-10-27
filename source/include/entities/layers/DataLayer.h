#ifndef DATALAYER_H
#define DATALAYER_H

#include "Layer.h"
#include "../../Shader.h"
#include "../../math/Color.h"
#include "../../math/transform.h"

class DataLayer : public Layer
{
private:
protected:
public:
	DataLayer(glm::ivec2 dimensions, std::string name, CColor fill, std::shared_ptr<Shader> shader);

	virtual void render(ShaderTransform* xform, unsigned int targetBuffer);
	virtual void draw(ShaderTransform* xform);
};

#endif