#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "Widget.h"
#include "../../Shader.h"
#include "../../math/transform.h"

class ImageWidget : public Widget
{
private:
protected:
	// Layout defined image, overrides the style defined image
	std::filesystem::path image;
public:
	// Constructors
	ImageWidget(std::map<std::string, std::string> basicAttribs,
		std::weak_ptr<Widget> parent,
		std::weak_ptr<WidgetStyle> style,
		std::shared_ptr<Shader> shader);
	virtual void buildWidget();
	virtual void clearData();

	// Reporting Functions
	virtual glm::ivec2 getSizeByChildren();
	virtual glm::ivec2 getSizeByParent();
	virtual glm::ivec2 findWidgetLocation(std::shared_ptr<Widget> childWidget);

	// Render Function
	virtual void drawSelf(ShaderTransform xform);
};

#endif