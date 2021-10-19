#include "../../include/gladHelper.h"

#include "../../include/entities/layers/Layer.h"
#include "../../include/loadImageData.h"
#include "../../include/math/Color.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "../../include/entities/visuals/Stroke.h"
#include "../../include/tool/ToolSettings.h"
#include "../../include/tool/Tool.h"

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
std::weak_ptr<Layer> Layer::getSelf()
{
	return weak_from_this();
}

// Management Functions
bool Layer::clearLayer()
{
	for (std::shared_ptr<Stroke> stroke : strokes)
	{
		stroke.get()->deleteBuffers();
		stroke.get()->cleanup_stroke();
		stroke.reset();
	}
	children.clear();
	strokes.clear();
	children.shrink_to_fit();
	strokes.shrink_to_fit();
	return true;
}

// Container Functions
std::shared_ptr<Stroke> Layer::createNewStroke(std::shared_ptr<Shader> shader, std::shared_ptr<Tool> tool, int entityCount)
{
	strokes.emplace_back(std::make_shared<Stroke>(shared_from_this(), shader, tool, entityCount));
	children.push_back(std::weak_ptr<Fragment>(strokes.back()));
	// Warning: Why is this not being done instead (below)? Define if there is a need for having two different vectors
	// if both store the same thing. If dynamic_cast or type-erasure is of concern, create an enum for Fragment
	// to store type information
	//children.push_back(std::make_shared<Stroke>(shader, basic, image));
	return strokes.back();
}

void Layer::removeStroke(std::weak_ptr<Stroke> target)
{
	// Clear from children
	size_t index = 0;
	for (std::weak_ptr<Fragment> child : children)
	{
		if (target.lock() == child.lock()) { child.reset(); break; }
		index++;
	}
	children.erase(children.begin() + index);
	children.shrink_to_fit();
	// Clear from strokes
	index = 0;
	for (std::shared_ptr<Stroke> stroke : strokes)
	{
		if (stroke != nullptr && target.lock() == stroke)
		{ 
			stroke.get()->deleteBuffers();
			stroke.get()->cleanup_stroke();
			stroke.reset();
			break;
		}
		index++;
	}
	strokes.erase(strokes.begin() + index);
	strokes.shrink_to_fit();
}