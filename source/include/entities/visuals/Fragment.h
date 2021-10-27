#ifndef FRAGMENT_H
#define FRAGMENT_H

#include "../VisualEntity.h"
#include "../visuals/VertexData.h"
#include "../../math/transform.h"

class Fragment : public VisualEntity
{
private:
protected:
public:
	VertexData fragData;

	Fragment();
	~Fragment();

	// Transform Functions
	void updateTransformByAnchors();

	// Frag Data Functions
	VertexData* getFragmentData() { return &fragData; }

	// Render Functions
	virtual void render(ShaderTransform* xform, unsigned int targetBuffer) = 0;
	virtual void draw(ShaderTransform* xform) = 0;
};

#endif