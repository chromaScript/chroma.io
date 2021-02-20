#ifndef CANVAS_H
#define CANVAS_H

#include <glm.hpp>

#include "structs.h"
#include "Color.h"

#include "../include/entities/Layer.h"
#include "../include/Shader.h"
#include "../include/Camera.h"
class Application;

#include <memory>

class Canvas : public VisualEntity
{
private:
	// Object owning this document
	Application* owner;
	// canvasShader property - Powers of 2 only
	int checkerSize = 16; 
	// Canvas container variables
	std::vector<std::shared_ptr<Layer>> layers;
	std::shared_ptr<Layer> activeLayer;
protected:
public:
	// Constructor / Destructor
	Canvas(int width, int height, std::shared_ptr<Shader> shader, Application* owner);
	// Container functions
	std::shared_ptr<Layer> newDataLayer(glm::ivec2 dimensions, std::string name, CColor fill, std::shared_ptr<Shader> shader);
	void setActiveLayer(std::shared_ptr<Layer> select);
	std::shared_ptr<Layer> getActiveLayer();
	// Render functions
	virtual void draw(ShaderTransform xform);
	void drawLayers(ShaderTransform xform);
};

#endif