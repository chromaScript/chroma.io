#include "../include/entities/Layer.h"
#include "../include/loadImageData.h"
#include "../include/Color.h"

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../include/entities/Stroke.h"
#include "../include/ToolSettings.h"

#include <memory>

//extern std::shared_ptr<Shader> layerShader;

// Constructors & Destructors
Layer::~Layer()
{

}

// Properties Functions
void Layer::clearLocks() 
{
	properties.alphaLock = false;
	properties.dataLock = false;
	properties.positionLock = false;
	properties.mergeLock = false;
}
LayerProperties Layer::getProperties()
{
	return properties;
}

// Container Functions
std::shared_ptr<Stroke> Layer::createNewStroke(std::shared_ptr<Shader> shader, TSet_Basic* basic, TSet_Image* image, TSet_Alpha* alpha)
{
	strokes.emplace_back(std::make_shared<Stroke>(shader, basic, image, alpha));
	children.push_back(std::weak_ptr<Fragment>(strokes.back()));
	// Warning: Why is this not being done instead (below)? Define if there is a need for having two different vectors
	// if both store the same thing. If dynamic_cast or type-erasure is of concern, create an enum for Fragment
	// to store type information
	//children.push_back(std::make_shared<Stroke>(shader, basic, image));
	return strokes.back();
}