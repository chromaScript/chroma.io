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
#include <string>

class Canvas : public VisualEntity, std::enable_shared_from_this<Canvas>
{
private:
	// canvasShader property - Powers of 2 only
	int checkerSize = 16; 
	// Canvas container variables
	std::vector<std::shared_ptr<Layer>> layers;
	std::weak_ptr<Layer> activeLayer;
	// Render Target Buffers
	std::shared_ptr<Shader> compositeFrameShader = nullptr;
	std::shared_ptr<Shader> compositeShader = nullptr;
	unsigned int renderBuffer = 0, renderColorBuffer = 0;
	float* renderData;
protected:
public:
	// Constructor / Destructor
	Canvas(std::string documentName, int width, int height, std::shared_ptr<Shader> shader);
	~Canvas();
	// Container functions
	std::weak_ptr<Layer> newDataLayer(glm::ivec2 dimensions, std::string name, CColor fill, std::shared_ptr<Shader> shader);
	void setActiveLayer(std::weak_ptr<Layer> select);
	std::weak_ptr<Layer> getActiveLayer();
	// Dimensions Functions
	glm::ivec2 getCanvasSize() { return glm::ivec2(transform.boundBox.x2, transform.boundBox.y2); }
	// Close Document
	bool close(bool saveBeforeExit);
	// Render functions
	float* renderCanvas();
	void deleteRenderData();
	virtual void render(ShaderTransform xform, unsigned int targetBuffer);
	virtual void draw(ShaderTransform xform);
	void drawLayers(ShaderTransform xform);
};

#endif