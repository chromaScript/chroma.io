#ifndef GRADIENTBOX_H
#define GRADIENTBOX_H

#include "Widget.h"

#include <map>

class GradientBox : public Widget
{
private:
protected:
	// Gradient-Box Specific Vertex Data
	// Note: Later the vertexData for VisualEntity will be overhauled and will include
	// the vertex color attribute as well.
	float gradientBoxData[36] = { 
		0.0f,0.0f,0.0f,		0.0f,0.0f,0.0f,0.0f,	0.0f,0.0f,
		0.0f,0.0f,0.0f,		0.0f,0.0f,0.0f,0.0f,	0.0f,0.0f,
		0.0f,0.0f,0.0f,		0.0f,0.0f,0.0f,0.0f,	0.0f,0.0f,
		0.0f,0.0f,0.0f,		0.0f,0.0f,0.0f,0.0f,	0.0f,0.0f };
	static const int gradientBoxData_size = sizeof(gradientBoxData);
	const int gbAttribStride = 9 * sizeof(float);
	const int gbVCAttribOffset = 3 * sizeof(float);
	const int gbTCAttribOffset = 7 * sizeof(float);

	// Buffer IDs
	unsigned int gbVAO = 0; // Vertex Array Object ID
	unsigned int gbVBO = 0; // Vertex Buffer Object ID
	unsigned int gbEBO = 0; // Elements Buffer Object ID
	unsigned int gbTEX0 = 0; // Texture Buffer Object ID // Do not need texture buffer

public:
	// Constructors
	GradientBox(std::map<std::string, std::string> basicAttribs,
		std::weak_ptr<Widget> parent,
		std::weak_ptr<WidgetStyle> style,
		std::shared_ptr<Shader> shader);
	virtual void buildWidget();
	void setVertData_gradientBox();

	// Dimension Functions
	virtual glm::ivec2 getSizeByChildren();
	virtual glm::ivec2 getSizeByParent();
	virtual glm::ivec2 findWidgetLocation(std::shared_ptr<Widget> childWidget);

	// Render Function
	virtual void drawSelf(ShaderTransform xform);
};

#endif