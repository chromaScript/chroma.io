#ifndef LAYER_H
#define LAYER_H

#include "VisualEntity.h"
#include "Fragment.h"
#include "Stroke.h"
#include "../Shader.h"
#include "../Color.h"

class Tool;

#include <vector>

class Layer : public VisualEntity
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
	LayerProperties getProperties();
	// Container Functions
	std::shared_ptr<Stroke> createNewStroke(std::shared_ptr<Shader> shader, std::shared_ptr<Tool> tool);
	// Management Functions
	bool clearLayer();
	// Render f\Functions
	virtual void render(ShaderTransform xform, unsigned int targetBuffer) = 0;
	virtual void draw(ShaderTransform xform) = 0;
};

#endif