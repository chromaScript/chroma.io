#ifndef IMAGE_H
#define IMAGE_H

#include "Widget.h"

class Image : public Widget
{
private:
protected:
	// Layout defined image, overrides the style defined image
	std::filesystem::path image;
public:
	// Constructors
	Image(std::map<std::string, std::string> basicAttribs,
		std::weak_ptr<Widget> parent,
		std::weak_ptr<WidgetStyle> style,
		std::shared_ptr<Shader> shader);
	virtual void buildWidget();

	// Reporting Functions
	virtual glm::ivec2 getSizeByChildren();
	virtual glm::ivec2 getSizeByParent();
	virtual glm::ivec2 findWidgetLocation(std::shared_ptr<Widget> childWidget);

	// Render Function
	virtual void drawSelf(ShaderTransform xform);
};

#endif