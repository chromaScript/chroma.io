#include "../include/entities/Text.h"
#include "../include/entities/Fonts.h"
#include "../include/entities/WidgetStyle.h"

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <map>
#include <iostream>
#include <memory>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

//Constructor
Text::Text(std::map<std::string, std::string> basicAttribs,
	std::weak_ptr<Widget> parent,
	std::weak_ptr<WidgetStyle> style,
	std::shared_ptr<Shader> shader,
	std::shared_ptr<Fonts> uiHandler) : Widget(style, shader)
{
	this->type = LTokenType::TEXT;
	this->parentWidget = parent;
	if (basicAttribs.count("id")) { this->idAttrib = basicAttribs.at("id"); }
	if (basicAttribs.count("name")) { this->nameAttrib = basicAttribs.at("name"); }
	if (basicAttribs.count("img")) { this->imgAttrib = basicAttribs.at("img"); }
	if (basicAttribs.count("value")) { this->valueAttrib = basicAttribs.at("value"); }
	if (basicAttribs.count("innerContent")) { this->innerContent = basicAttribs.at("innerContent"); }
	maxChildren = 0;
	//this->textValue = value;
	this->uiHandler = uiHandler;
	generateBuffers(&(this->VAO), &(this->VBO), &(this->EBO), &(this->TEX0));
	setTextData();
	glm::ivec2 getSize = uiHandler.get()->findSizeTextLine(valueAttrib, thisFace, thisCurrentBitmap);
	sizeX = getSize.x;
	sizeY = style.lock().get()->fontSize;
}

void Text::buildWidget()
{
	setTextData();
	setBounds_Widget();
	setVertData_Widget();
	bindBuffers();
}
void Text::clearData()
{
	uiHandler.reset();
	thisFace.reset();
	thisCurrentBitmap.reset();
}

// Initialize text data - Gets the fontPath, fontSize, thisFace and thisCurrentBitmap
void Text::setTextData()
{
	// First get the actual style values for path/size
	inheritedFontPath = this->style.fontPath;
	inheritedFontSize = this->style.fontSize;
	// If these don't exist, get the parent widget values by inheritance
	if (inheritedFontPath == "" && style.fontPath == "")
	{
		inheritedFontPath = findFontPath();
		std::cout << "TEXT::INHERITED_NEW_PATH::" << inheritedFontPath << std::endl;
	}
	if (style.fontSize == 0 && inheritedFontSize == 0)
	{
		inheritedFontSize = (int)style.minY;
		std::cout << "TEXT::INHERITED_NEW_SIZE::" << inheritedFontSize << std::endl;
	}
	// Get the face objecet for this widget's font
	thisFace = this->uiHandler.get()->getFace(inheritedFontPath);
	if (thisFace == nullptr) { std::cout << "UI::WIDGET::ERROR::TEXT::FAILED_TO_LOCATED_ASSOCIATED_FONT-FACE" << std::endl; }
	if (thisCurrentBitmap == nullptr)
	{
		thisCurrentBitmap = uiHandler.get()->findBitmap(inheritedFontSize, inheritedFontPath);
	}
}
// Dimension Functions
glm::ivec2 Text::getSizeByChildren()
{
	// TEXT widgets report their size based solely upon their text-contents, in order to use fill-type
	// sizes, they must be wrapped in another widget
	glm::ivec2 report = uiHandler.get()->findSizeTextLine(valueAttrib, thisFace, thisCurrentBitmap);
	sizeX = style.minX = report.x;
	sizeY = style.minY = report.y = style.fontSize;
	return report;
}
glm::ivec2 Text::getSizeByParent()
{
	// TEXT widgets report their size based solely upon their text-contents, in order to use fill-type
	// sizes, they must be wrapped in another widget
	glm::ivec2 report = uiHandler.get()->findSizeTextLine(valueAttrib, thisFace, thisCurrentBitmap);
	sizeX = style.minX = report.x;
	sizeY = style.minY = report.y = style.fontSize;
	return report;
}
// Report the location of child widget relative to it's siblings (Per-Widget behavior, this is how a widget
// manages it's children locations.
glm::ivec2 Text::findWidgetLocation(std::shared_ptr<Widget> childWidget)
{
	// Image should have no child widgets, so this function should not ever get called.
	// But if it does, return the offset style attribute.
	glm::ivec2 childLoc;
	childLoc.x = childWidget.get()->style.getOffset().left;
	childLoc.y = childWidget.get()->style.getOffset().top;
	return childLoc;
}

// Render Functions
void Text::bindBuffers()
{
	// Bind vertex array and buffers with data
	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertData_size, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertArrangement_size, vertArrangement, GL_STATIC_DRAW);
	// Set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribStride, (void*)0);
	glEnableVertexAttribArray(0);
	// Set texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attribStride, (void*)tAttribOffset);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}
void Text::drawSelf(ShaderTransform xform)
{
	glm::vec4 textColor;
	glm::vec2 screenLocation;
	int overflowValue = (applyOverflowMask && !overflowTarget.expired()) ? 0 : 1;
	glm::vec4 overflowBoxMask = glm::vec4(0.0f);
	// Calculate the overflowMask if needed (Note that the box is pre-calculated, but given an offset per-frame)
	// offsetBox is given as BottomLeft.xy, TopRight.xy
	if (overflowValue == 0)
	{
		overflowBoxMask = calculateOverflowMask();
	}
	// Draw the drop shadow if applicable
	if (style.boxShadowSizeX != 0 || style.boxShadowSizeY != 0)
	{
		screenLocation = getScreenLocation(style.boxShadowSizeX, style.boxShadowSizeY, true);
		textColor = style.boxShadowColor.makeVec4();
		uiHandler.get()->renderTextLine(shader, textColor, style.boxShadowAlpha, valueAttrib,
			thisFace, thisCurrentBitmap,
			screenLocation, glm::ivec2(transform.boundBox.x1, transform.boundBox.y1),
			this->VAO, this->VBO, this->EBO, overflowValue, overflowBoxMask);
	}

	screenLocation = getScreenLocation(0, 0, true);
	textColor = style.fillColor.makeVec4();
	uiHandler.get()->renderTextLine(shader, textColor, style.fillAlpha, valueAttrib,
		thisFace, thisCurrentBitmap, 
		screenLocation, glm::ivec2(transform.boundBox.x1, transform.boundBox.y1),
		this->VAO, this->VBO, this->EBO, overflowValue, overflowBoxMask);
}