#ifndef LAYER_H
#define LAYER_H

#include "../VisualEntity.h"
#include "../visuals/Fragment.h"

#include "../../Shader.h"
#include "../../math/Color.h"
#include "LayerProperties.h"
#include "../visuals/Stroke.h"

class Tool;

#include <vector>
#include <memory>

class Layer : public VisualEntity, public std::enable_shared_from_this<Layer>
{
private:
protected:
	// Note: Should integrate a lot of this into the base class VisualEntity for convenience
	// Layer properties
	LayerProperties properties = LayerProperties();	
	// Data variables
	
	// Container variables
	std::vector<std::weak_ptr<Fragment>> children; // Container for all fragments in the layer. Emplace_back into this
	std::vector<std::shared_ptr<Stroke>> strokes;
public:
	// Constructor / Destructor
	// Layer Constructor is blank. The final class in the visualEntity chain should have a specialized constructor
	Layer() {};
	~Layer();
	// Layer Properties Functions
	void clearLocks();
	std::weak_ptr<Layer> getSelf();
	LayerProperties getProperties();
	// Container Functions
	std::shared_ptr<Stroke> createNewStroke(std::shared_ptr<Shader> shader, std::shared_ptr<Tool> tool, int entityCount);
	void removeStroke(std::weak_ptr<Stroke> target);
	// Management Functions
	bool clearLayer();
	// Render f\Functions
	virtual void render(ShaderTransform* xform, unsigned int targetBuffer) = 0;
	virtual void draw(ShaderTransform* xform) = 0;
};

#endif