#ifndef LINE_H
#define LINE_H

#include "Widget.h"
#include "WidgetInterface.h"
#include "Fonts.h"
#include "../keys.h"

class Line : public Widget, public WidgetInterface
{
private:
	float cursorData[20] = { 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	float selectionData[20] = { 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	unsigned int cursorVAO = 0, selectionVAO = 0;
	unsigned int cursorVBO = 0, selectionVBO = 0;
	unsigned int cursorEBO = 0, selectionEBO = 0;
	int cursorPos = 0;
	int selectionStart = 0, selectionEnd = 0;
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
	Line(std::map<std::string, std::string> basicAttribs,
		std::weak_ptr<Widget> parent,
		std::weak_ptr<WidgetStyle> style,
		std::shared_ptr<Shader> shader,
		std::shared_ptr<Fonts> uiHandler);
	virtual void buildWidget();
	virtual void clearData();

	// Data Functions (Text Specific)
	void setCursorPos(int pos);
	void clearSelection();
	void advanceCursor();
	void retreatCursor();
	void acceptCharInput(std::string strChar);
	int acceptInputCmd(TextInputCmd cmd, std::string &clipBoard);
	void setTextData();

	// Reporting Functions
	virtual glm::ivec2 getSizeByChildren();
	virtual glm::ivec2 getSizeByParent();
	virtual glm::ivec2 findWidgetLocation(std::shared_ptr<Widget> childWidget);

	// Render Function
	virtual void bindBuffers();
	virtual void drawSelf(ShaderTransform xform);
};

#endif