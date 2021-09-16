#include "../include/entities/Line.h"
#include "../include/entities/Fonts.h"
#include "../include/entities/WidgetStyle.h"
#include "../include/entities/WidgetInterface.h"
#include "../include/keys.h"

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include <map>
#include <iostream>
#include <memory>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

//Constructor
Line::Line(std::map<std::string, std::string> basicAttribs,
	std::weak_ptr<Widget> parent,
	std::weak_ptr<WidgetStyle> style,
	std::shared_ptr<Shader> shader,
	std::shared_ptr<Fonts> uiHandler) : Widget(style, shader)
{
	this->type = LTokenType::LINE;
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
	generateBuffers(&(this->cursorVAO), &(this->cursorVBO), &(this->cursorEBO));
	generateBuffers(&(this->selectionVAO), &(this->selectionVBO), &(this->selectionEBO));
	setTextData();
	glm::ivec2 getSize = uiHandler.get()->findSizeTextLine(valueAttrib, thisFace, thisCurrentBitmap);
	sizeX = getSize.x;
	sizeY = style.lock().get()->fontSize;
	attachTextInterface(this);

// Temporary placement for Line Cursor Data
	// Bind vertex array and buffers with data
	glBindVertexArray(cursorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cursorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cursorData), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cursorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertArrangement_size, vertArrangement, GL_STATIC_DRAW);
	// Set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribStride, (void*)0);
	glEnableVertexAttribArray(0);
	// Set texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attribStride, (void*)tAttribOffset);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	//
	// Bind vertex array and buffers with data
	glBindVertexArray(selectionVAO);
	glBindBuffer(GL_ARRAY_BUFFER, selectionVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(selectionData), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, selectionEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertArrangement_size, vertArrangement, GL_STATIC_DRAW);
	// Set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribStride, (void*)0);
	glEnableVertexAttribArray(0);
	// Set texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attribStride, (void*)tAttribOffset);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Line::buildWidget()
{
	setTextData();
	setBounds_Widget();
	setVertData_Widget();
	bindBuffers();
}
void Line::clearData()
{
	glDeleteBuffers(1, &this->cursorVBO);
	glDeleteBuffers(1, &this->cursorEBO);
	glDeleteVertexArrays(1, &this->cursorVAO);
	glDeleteBuffers(1, &this->selectionVBO);
	glDeleteBuffers(1, &this->selectionEBO);
	glDeleteVertexArrays(1, &this->selectionVAO);
	uiHandler.reset();
	thisFace.reset();
	thisCurrentBitmap.reset();
}

// Initialize text data - Gets the fontPath, fontSize, thisFace and thisCurrentBitmap
void Line::setTextData()
{
	// First get the actual style values for path/size
	inheritedFontPath = this->style.fontPath;
	inheritedFontSize = this->style.fontSize;
	// If these don't exist, get the parent widget values by inheritance
	if (inheritedFontPath == "" && style.fontPath == "")
	{
		inheritedFontPath = findFontPath();
		std::cout << "LINE::INHERITED_NEW_PATH::" << inheritedFontPath << std::endl;
	}
	if (style.fontSize == 0 && inheritedFontSize == 0)
	{
		inheritedFontSize = (int)style.minY;
		std::cout << "LINE::INHERITED_NEW_SIZE::" << inheritedFontSize << std::endl;
	}
	// Get the face objecet for this widget's font
	thisFace = this->uiHandler.get()->getFace(inheritedFontPath);
	if (thisFace == nullptr) { std::cout << "UI::WIDGET::ERROR::LINE::FAILED_TO_LOCATED_ASSOCIATED_FONT-FACE" << std::endl; }
	if (thisCurrentBitmap == nullptr)
	{
		thisCurrentBitmap = uiHandler.get()->findBitmap(inheritedFontSize, inheritedFontPath);
	}
}
// Dimension Functions
glm::ivec2 Line::getSizeByChildren()
{
	// TEXT widgets report their size based solely upon their text-contents, in order to use fill-type
	// sizes, they must be wrapped in another widget
	glm::ivec2 report = uiHandler.get()->findSizeTextLine(valueAttrib, thisFace, thisCurrentBitmap);
	sizeX = style.minX = report.x;
	sizeY = style.minY = report.y = style.fontSize;
	return report;
}
glm::ivec2 Line::getSizeByParent()
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
glm::ivec2 Line::findWidgetLocation(std::shared_ptr<Widget> childWidget)
{
	// Image should have no child widgets, so this function should not ever get called.
	// But if it does, return the offset style attribute.
	glm::ivec2 childLoc;
	childLoc.x = childWidget.get()->style.getOffset().left;
	childLoc.y = childWidget.get()->style.getOffset().top;
	return childLoc;
}

// Line Functions
void Line::setCursorPos(int pos)
{
	cursorPos = clampi(pos, 0, int(valueAttrib.size()));;
	std::cout << "LINE::CURSOR_POS::=" << cursorPos << std::endl;
}
void Line::clearSelection()
{
	selectionStart = selectionEnd = 0;
}
void Line::advanceCursor()
{
	cursorPos = clampi(cursorPos + 1, 0, int(valueAttrib.size()));
	std::cout << "LINE::CURSOR_POS::=" << cursorPos << std::endl;
}
void Line::retreatCursor()
{
	cursorPos = clampi(cursorPos - 1, 0, int(valueAttrib.size()));
	std::cout << "LINE::CURSOR_POS::=" << cursorPos << std::endl;
}
void Line::acceptCharInput(std::string strChar)
{
	if (selectionStart - selectionEnd != 0)
	{
		valueAttrib.erase(
			valueAttrib.begin() + size_t(clampi(selectionStart, 0, LONG_MAX)),
			valueAttrib.begin() + size_t(clampi(selectionEnd, 0, LONG_MAX)));
		cursorPos = selectionStart;
		if (cursorPos > valueAttrib.size()) { cursorPos = valueAttrib.size(); }
	}
	if (strChar.size() == 1)
	{
		valueAttrib.insert(valueAttrib.begin() + (size_t)cursorPos, strChar[0]);
		clearSelection(); advanceCursor();
	}
	else
	{
		valueAttrib.insert(valueAttrib.begin() + (size_t)cursorPos, strChar.begin(), strChar.end());
		clearSelection(); cursorPos = cursorPos + strChar.size();
	}
}
int Line::acceptInputCmd(TextInputCmd cmd, std::string &clipBoard)
{
	if (cmd == TextInputCmd::escapeInput) { return 1; }
	else if (cmd == TextInputCmd::commitInput) { return 2; }
	else
	{
		switch (cmd)
		{
		case TextInputCmd::moveCursorLeft:
			clearSelection(); retreatCursor(); break;
		case TextInputCmd::moveCursorRight:
			clearSelection(); advanceCursor(); break;
		case TextInputCmd::selectAll:
			if (valueAttrib.size() == 0) { break; }
			selectionStart = 0; cursorPos = selectionEnd = valueAttrib.size();
			break;
		case TextInputCmd::selectMoveLeft:
			if (selectionStart - selectionEnd == 0) 
			{ 
				selectionStart = selectionEnd = cursorPos;
				retreatCursor(); selectionStart = cursorPos;
			}
			else if (cursorPos > selectionStart)
			{
				retreatCursor(); selectionEnd = cursorPos;
			}
			else
			{
				retreatCursor(); selectionStart = cursorPos;
			}
			break;
		case TextInputCmd::selectMoveRight:
			if (selectionStart - selectionEnd == 0) 
			{ 
				selectionStart = selectionEnd = cursorPos;
				advanceCursor(); selectionEnd = cursorPos;
			}
			else if (cursorPos < selectionEnd)
			{
				advanceCursor(); selectionStart = cursorPos;
			}
			else
			{
				advanceCursor(); selectionEnd = cursorPos;
			}
			break;
		case TextInputCmd::backspaceChar:
			if (valueAttrib.size() == 0 || (cursorPos == 0 && selectionStart - selectionEnd == 0)) { break; }
			if (selectionStart - selectionEnd == 0)
			{
				valueAttrib.erase(valueAttrib.begin() + size_t(clampi(cursorPos - 1, 0, LONG_MAX)));
				clearSelection(); retreatCursor();
			}
			else
			{
				valueAttrib.erase(
					valueAttrib.begin() + size_t(clampi(selectionStart, 0, LONG_MAX)),
					valueAttrib.begin() + size_t(clampi(selectionEnd, 0, LONG_MAX)));
				clearSelection();
				if (cursorPos > valueAttrib.size()) { cursorPos = valueAttrib.size(); }
			}
			break;
		case TextInputCmd::copyText:
			if (selectionStart - selectionEnd == 0) { clipBoard = ""; }
			else
			{
				clipBoard = valueAttrib.substr(selectionStart, (size_t)selectionEnd - (size_t)selectionStart);
			}
			break;
		case TextInputCmd::pasteText:
			acceptCharInput(clipBoard);
			break;
		case TextInputCmd::cutText:
			acceptInputCmd(TextInputCmd::copyText, clipBoard);
			acceptInputCmd(TextInputCmd::backspaceChar, clipBoard);
			break;
		}
		return false;
	}
}

// Render Functions
void Line::bindBuffers()
{
	// Bind vertex array and buffers with data
	glBindVertexArray(VAO);
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
void Line::drawSelf(ShaderTransform xform)
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
			VAO, VBO, EBO, overflowValue, overflowBoxMask);
	}

	screenLocation = getScreenLocation(0, 0, true);
	textColor = style.fillColor.makeVec4();
	uiHandler.get()->renderTextInput(shader, valueAttrib,
		thisFace, thisCurrentBitmap,
		screenLocation, glm::ivec2(transform.boundBox.x1, transform.boundBox.y1), 
		style.fillColor.makeVec4(), style.highlightColor.makeVec4(), style.focusColor.makeVec4(),
		{ VAO, VBO, EBO, cursorVAO, cursorVBO, cursorEBO, selectionVAO, selectionVBO, selectionEBO },
		glm::ivec4(transform.boundBox.x1, transform.boundBox.y1, transform.boundBox.x2, transform.boundBox.y2),
		cursorPos, selectionStart, selectionEnd,
		isActive, (isActive && (selectionEnd - selectionStart != 0)), true, cursorData, selectionData, overflowValue, overflowBoxMask);
		
}