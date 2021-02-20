#ifndef CANVAS_H
#define CANVAS_H

#include <glm.hpp>

#include "../structs.h"
#include "../Color.h"

#include "Layer.h"
#include "../Shader.h"
#include "../Camera.h"
class Application;

#include <memory>

class Canvas : public VisualEntity
{
private:
	// canvasShader property - Powers of 2 only
	int checkerSize = 16; 
	// Canvas container variables
	std::vector<std::shared_ptr<Layer>> layers;
	std::shared_ptr<Layer> activeLayer;
	// Render Target Buffers
	std::shared_ptr<Shader> compositeFrameShader = nullptr;
	std::shared_ptr<Shader> compositeShader = nullptr;
	unsigned int renderBuffer = 0, renderColorBuffer = 0;
	float* renderData;
protected:
public:
	// Constructor / Destructor
	Canvas(int width, int height, std::shared_ptr<Shader> shader);
	// Container functions
	std::shared_ptr<Layer> newDataLayer(glm::ivec2 dimensions, std::string name, CColor fill, std::shared_ptr<Shader> shader);
	void setActiveLayer(std::shared_ptr<Layer> select);
	std::shared_ptr<Layer> getActiveLayer();
	// Dimensions Functions
	glm::ivec2 getCanvasSize() { return glm::ivec2(transform.boundBox.x2, transform.boundBox.y2); }
	// Render functions
	float* renderCanvas();
	void deleteRenderData();
	virtual void render(ShaderTransform xform, unsigned int targetBuffer);
	virtual void draw(ShaderTransform xform);
	void drawLayers(ShaderTransform xform);
};

#endif