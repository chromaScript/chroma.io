#ifndef HORIZONTALBOX_H
#define HORIZONTALBOX_H

#include "Widget.h"

#include <map>

class HorizontalBox : public Widget
{
private:
protected:
public:
	// Constructors
	HorizontalBox(std::map<std::string, std::string> basicAttribs,
		std::weak_ptr<Widget> parent,
		std::weak_ptr<WidgetStyle> style,
		std::shared_ptr<Shader> shader);
	virtual void buildWidget();

	// Dimension Functions
	virtual glm::ivec2 getSizeByChildren();
	virtual glm::ivec2 getSizeByParent();
	virtual glm::ivec2 findWidgetLocation(std::shared_ptr<Widget> childWidget);

	// Render Function
	virtual void drawSelf(ShaderTransform xform);
};

#endif