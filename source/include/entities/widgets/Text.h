#ifndef TEXT_H
#define TEXT_H

#include "Widget.h"
#include "../Fonts.h"
#include "../../math/transform.h"

class Text : public Widget
{
private:
protected:
	// Pointer to the UserInterface fontsHandler variable (Fonts.cpp class), which is used to draw text
	std::shared_ptr<Fonts> uiHandler;
	std::shared_ptr<CachedFace> thisFace;
	std::shared_ptr<CachedCharBitmap> thisCurrentBitmap;
	// Values for when the TEXT object is not supplied a proper style with path & size
	std::filesystem::path inheritedFontPath;
	int inheritedFontSize = 0;
public:
	// Constructors
	Text(std::map<std::string, std::string> basicAttribs,
		std::weak_ptr<Widget> parent,
		std::weak_ptr<WidgetStyle> style,
		std::shared_ptr<Shader> shader,
		std::shared_ptr<Fonts> uiHandler);
	virtual void buildWidget();
	virtual void clearData();

	// Data Functions (Text Specific)
	void setTextData();

	// Reporting Functions
	virtual glm::ivec2 getSizeByChildren();
	virtual glm::ivec2 getSizeByParent();
	virtual glm::ivec2 findWidgetLocation(std::shared_ptr<Widget> childWidget);

	// Render Function
	virtual void bindBuffers();
	virtual void drawSelf(ShaderTransform* xform);
};

#endif