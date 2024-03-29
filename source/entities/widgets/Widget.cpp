#include "../../include/entities/widgets/Widget.h"
#include "../../include/entities/widgets/HorizontalBox.h"
#include "../../include/entities/widgets/Noise_ToolControl.h"

#include "../../include/entities/widgets/WidgetStyle.h"
#include "../../include/cscript/CObject.h"
#include "../../include/clayout/LEnums.h"
#include "../../include/cstyle/SEnums.h"
#include "../../include/cscript/CStmt.h"
#include "../../include/cscript/ChromaScript.h"

#include "../../include/entities/UserInterface.h"
#include "../../include/input/keys.h"

#include <iostream>
#include <memory>
#include <map>

extern std::shared_ptr<Application> chromaIO;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

// Constructors
Widget::Widget(std::weak_ptr<WidgetStyle> style, std::shared_ptr<Shader> shader)
{
	generateUEID();
	//
	this->style = *style.lock();
	this->savedStyle = this->style;
	this->shader = shader;
	// Note: Move this later to a hierarchy-based function
	setTransform(glm::vec3(0.0f, 0.0f, 0.0f));
	setTransform(0.0f);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Widget Transform Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Set Transform for Widget
// Only cares about the widget's dimensions, the bounds are then set by the widget anchor setting
// The pxPosition, and location are handled by the draw call and get/set functions for location
void Widget::setBounds_Widget()
{
	int remainderX = sizeX % 2;
	int remainderY = sizeY % 2;
	switch (style.anchoring)
	{
	case UI_ANCHORING_UPPERLEFT:
		transform.boundBox.x1 = 0;
		transform.boundBox.x2 = sizeX;
		transform.boundBox.y1 = 0;
		transform.boundBox.y2 = sizeY;
		break;
	case UI_ANCHORING_UPPERCENTER:
		transform.boundBox.x1 = -(sizeX / 2);
		transform.boundBox.x2 = (sizeX / 2) + remainderX;
		transform.boundBox.y1 = 0;
		transform.boundBox.y2 = sizeY;
		break;
	case UI_ANCHORING_UPPERRIGHT:
		transform.boundBox.x1 = -sizeX;
		transform.boundBox.x2 = 0;
		transform.boundBox.y1 = 0;
		transform.boundBox.y2 = sizeY;
		break;
	case UI_ANCHORING_CENTERLEFT:
		transform.boundBox.x1 = 0;
		transform.boundBox.x2 = sizeX;
		transform.boundBox.y1 = -(sizeY / 2);
		transform.boundBox.y2 = (sizeY / 2) + remainderY;
		break;
	case UI_ANCHORING_CENTER:
		transform.boundBox.x1 = -(sizeX / 2);
		transform.boundBox.x2 = (sizeX / 2) + remainderX;
		transform.boundBox.y1 = -(sizeY / 2);
		transform.boundBox.y2 = (sizeY / 2) + remainderY;
		break;
	case UI_ANCHORING_CENTERRIGHT:
		transform.boundBox.x1 = -sizeX;
		transform.boundBox.x2 = 0;
		transform.boundBox.y1 = -(sizeY / 2);
		transform.boundBox.y2 = (sizeY / 2) + remainderY;
		break;
	case UI_ANCHORING_LOWERLEFT:
		transform.boundBox.x1 = 0;
		transform.boundBox.x2 = sizeX;
		transform.boundBox.y1 = -sizeY;
		transform.boundBox.y2 = 0;
		break;
	case UI_ANCHORING_LOWERCENTER:
		transform.boundBox.x1 = -(sizeX / 2);
		transform.boundBox.x2 = (sizeX / 2) + remainderX;
		transform.boundBox.y1 = -sizeY;
		transform.boundBox.y2 = 0;
		break;
	case UI_ANCHORING_LOWERRIGHT:
		transform.boundBox.x1 = -sizeX;
		transform.boundBox.x2 = 0;
		transform.boundBox.y1 = -sizeY;
		transform.boundBox.y2 = 0;
		break;
	default: // UI_ANCHORING_UPPERLEFT is DEFAULT
		transform.boundBox.x1 = 0;
		transform.boundBox.x2 = sizeX;
		transform.boundBox.y1 = 0;
		transform.boundBox.y2 = sizeY;
		break;
	}
}
// Data Functions
// Update the vertex data when changing widget dimensions
void Widget::setVertData_Widget()
{
	float ox = (2 * (float(transform.boundBox.x1) / WINDOW_WIDTH)) - 1;
	float oy = (2 * (float(WINDOW_HEIGHT - transform.boundBox.y1) / WINDOW_HEIGHT)) - 1;
	float vx = (2 * (float(transform.boundBox.x2) / WINDOW_WIDTH)) - 1;
	float vy = (2 * (float(WINDOW_HEIGHT - transform.boundBox.y2) / WINDOW_HEIGHT)) - 1;
	float ratio = (float)sizeX / (float)sizeY;
	float tx;
	float ty;
	if (ratio >= 1.0f)
	{
		tx = 1.0f;
		ty = tx / ratio;
	}
	else
	{
		ty = 1.0f;
		tx = ty * ratio;
	}
	// Now create a new float array with these bounds
	float newData[20] = {
		// The top and bottom are normal texture coords because this renders in respect
		// to the screen space coordinates
		// positions			// texture coords - 
		vx, oy, 0.0f,			1.0f, 1.0f, // top right
		vx, vy,	0.0f,			1.0f, 0.0f, // bottom right
		ox, vy, 0.0f,			0.0f, 0.0f,  // bottom left
		ox, oy, 0.0f,			0.0f, 1.0f // top left
	};
	// Overwrite the old data with the new data
	for (int i = 0; i < 20; i++)
	{
		vertData[i] = newData[i];
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Build Widget Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////
void Widget::resetSize()
{
	sizeX = 0; sizeY = 0;
}
glm::ivec2 Widget::setSizeByChildren()
{
	if (idAttrib == "tcnp_body")
	{
		int k = 5;
	}
	glm::ivec2 outSize = glm::ivec2(sizeX, sizeY);
	// 1. Establish minimum/maximum value (Must never violate minX/Y sizes)
	// Allow Max values to be INT_MAX to allow fill/stretch to work properly
		// For X
	int potentialMinX = findMinX();
	if (potentialMinX == UI_WVALUE_NONE) { potentialMinX = 0; }
	int potentialMaxX = findMaxX();
	if (potentialMaxX > style.maxX && style.maxX > 0 && style.relMaxX != true) { potentialMaxX = (int)style.maxX; }
	// For Y
	int potentialMinY = findMinY();
	if (potentialMinY == UI_WVALUE_NONE) { potentialMinY = 0; }
	int potentialMaxY = findMaxY();
	if (potentialMaxY > style.maxY && style.maxY > 0 && style.relMaxY != true) { potentialMaxY = (int)style.maxY; }
	// 2. Size widget according to it's children/self if applicable
	if ((style.overflowX == UI_OVERFLOW_AUTO || style.overflowX == UI_OVERFLOW_AUTOBLOCK) && childWidgets.size() != 0)
	{
		int childSizeSumX = 0;
		size_t numChildren = childWidgets.size();
		for (size_t i = 0; i < numChildren; i++)
		{
			if (childWidgets[i].get()->style.position == UI_POSITION_ABSOLUTE ||
				childWidgets[i].get()->style.position == UI_POSITION_FIXED ||
				!childWidgets[i].get()->checkVisibility())
			{
				continue; // Break if this child widget is outside normal flow, do not add index
			}
			else if (childWidgets[i].get()->style.floating == UI_FLOATING_LEFT)
			{
				if (i == 0)
				{
					childSizeSumX += childWidgets[i].get()->getSize().x;
					childSizeSumX += childWidgets[i].get()->style.getMargin().left;
					if (childWidgets[i].get()->style.position == UI_POSITION_RELATIVE)
					{
						childSizeSumX += childWidgets[i].get()->style.getOffset().left - childWidgets[i].get()->style.getOffset().right;
					}
					if (numChildren == 1)
					{
						childSizeSumX += childWidgets[i].get()->style.getMargin().right;
					}
				}
				else
				{
					childSizeSumX += childWidgets[i].get()->getSize().x;
					if (childWidgets[i].get()->style.position == UI_POSITION_RELATIVE)
					{
						childSizeSumX += childWidgets[i].get()->style.getOffset().left - childWidgets[i].get()->style.getOffset().right;
					}
					childSizeSumX += childWidgets[i].get()->style.getBorder().left;
					childSizeSumX += childWidgets[i - 1].get()->style.getBorder().right;
					if (i == numChildren - 1)
					{
						childSizeSumX += childWidgets[i].get()->style.getMargin().right;
					}
				}
			}
			// If any element is right floated, then the widget should size to match it's container
			// because float right would drag the right-bound of the container to it's logical maximum
			else if (childWidgets[i].get()->style.floating == UI_FLOATING_RIGHT)
			{
				
				std::shared_ptr<Widget> dir = (parentWidget.expired()) ? nullptr : parentWidget.lock();
				int findSize = 0;
				while (findSize == 0 && dir != nullptr)
				{
					if (dir.get()->sizeX > 0)
					{
						findSize = dir.get()->sizeX;
					}
					dir = (dir.get()->parentWidget.expired()) ? nullptr : dir.get()->parentWidget.lock();
				}
				childSizeSumX = findSize - style.getMargin().left - style.getMargin().right;
				break;
			}
		}
		outSize.x = (childSizeSumX > potentialMinX) ? childSizeSumX : potentialMinX;
		outSize.x = (outSize.x < potentialMaxX) ? outSize.x : potentialMaxX;
	}
	else
	{
		int currentSizeX = getSizeByChildren().x;
		outSize.x = (currentSizeX > potentialMinX) ? currentSizeX : potentialMinX;
		outSize.x = (outSize.x > potentialMaxX) ? potentialMaxX : outSize.x;
	}
	if ((style.overflowY == UI_OVERFLOW_AUTO || style.overflowY == UI_OVERFLOW_AUTOBLOCK) && childWidgets.size() != 0)
	{
		int childSizeSumY = 0;
		int greatestSizeY = 0;
		size_t numChildren = childWidgets.size();
		int visCount = 0;
		for (size_t i = 0; i < numChildren; i++)
		{
			// For H_BOX, the final size should be the highest sizeY of child Widgets
			if (type == LTokenType::H_BOX)
			{
				childSizeSumY = 0;
				if (childWidgets[i].get()->style.position == UI_POSITION_ABSOLUTE ||
					childWidgets[i].get()->style.position == UI_POSITION_FIXED ||
					!childWidgets[i].get()->checkVisibility())
				{
					continue; // Break if this child widget is outside normal flow, do not add index
				}
				else if (childWidgets[i].get()->style.floating == UI_FLOATING_LEFT)
				{
					childSizeSumY += childWidgets[i].get()->getSize().y;
					childSizeSumY += childWidgets[i].get()->style.getMargin().top;
					childSizeSumY += childWidgets[i].get()->style.getMargin().bottom;
					if (childWidgets[i].get()->style.position == UI_POSITION_RELATIVE)
					{
						childSizeSumY += childWidgets[i].get()->style.getOffset().top - childWidgets[i].get()->style.getOffset().bottom;
					}
				}
				if (childSizeSumY > greatestSizeY) { greatestSizeY = childSizeSumY; }
				childSizeSumY = greatestSizeY;
			}
			// For V_BOX and other vertical-stacking widgets, the highest sizeY should be a sum
			else
			{
				if (childWidgets[i].get()->style.position == UI_POSITION_ABSOLUTE ||
					childWidgets[i].get()->style.position == UI_POSITION_FIXED ||
					!childWidgets[i].get()->checkVisibility())
				{
					continue; // Break if this child widget is outside normal flow, do not add index
				}
				else if (childWidgets[i].get()->style.floating == UI_FLOATING_LEFT)
				{

					if (i == 0)
					{
						childSizeSumY += childWidgets[i].get()->getSize().y;
						childSizeSumY += childWidgets[i].get()->style.getMargin().top;
						if (childWidgets[i].get()->style.position == UI_POSITION_RELATIVE)
						{
							childSizeSumY += childWidgets[i].get()->style.getOffset().top - childWidgets[i].get()->style.getOffset().bottom;
						}
						if (numChildren == 1)
						{
							childSizeSumY += childWidgets[i].get()->style.getMargin().bottom;
						}
					}
					else
					{
						childSizeSumY += childWidgets[i].get()->getSize().y;
						if (childWidgets[i].get()->style.position == UI_POSITION_RELATIVE)
						{
							childSizeSumY += childWidgets[i].get()->style.getOffset().top - childWidgets[i].get()->style.getOffset().bottom;
						}
						childSizeSumY += childWidgets[i].get()->style.getBorder().top;
						childSizeSumY += childWidgets[i - 1].get()->style.getBorder().bottom;
						if (i == numChildren - 1)
						{
							childSizeSumY += childWidgets[i].get()->style.getMargin().bottom;
						}
					}
				}
				// If any element is right floated, then the widget should size to match it's container
				// because float right would drag the right-bound of the container to it's logical maximum
				else if (childWidgets[i].get()->style.floating == UI_FLOATING_RIGHT)
				{
					std::shared_ptr<Widget> dir = (parentWidget.expired()) ? nullptr : parentWidget.lock();
					int findSize = 0;
					while (findSize == 0 && dir != nullptr)
					{
						if (dir.get()->sizeY > 0)
						{
							findSize = dir.get()->sizeY;
						}
						dir = (dir.get()->parentWidget.expired()) ? nullptr : dir.get()->parentWidget.lock();
					}
					childSizeSumY = findSize;
					break;
				}
			}
		}
		outSize.y = (childSizeSumY > potentialMinY) ? childSizeSumY : potentialMinY;
		outSize.y = (outSize.y < potentialMaxY) ? outSize.y : potentialMaxY;
	}
	else
	{
		int currentSizeY = getSizeByChildren().y;
		outSize.y = (currentSizeY > potentialMinY) ? currentSizeY : potentialMinY;
		outSize.y = (outSize.y > potentialMaxY) ? potentialMaxY : outSize.y;
	}
	sizeX = outSize.x;
	sizeY = outSize.y;
	return outSize;
}
glm::ivec2 Widget::setSizeByParent()
{
// Size widget according to it's self/parent if applicable
	// 1. Set base value to previously found sizeByChild as the baseline
	glm::ivec2 outSize = glm::ivec2(sizeX, sizeY);
	// 2. Establish minimum value (Must never violate minX/Y sizes)
		// For X
	int potentialMinX = findMinX();
	if (potentialMinX == UI_WVALUE_NONE) { outSize.x = 0; }
	else if (potentialMinX > style.minX && style.minX > 0) { outSize.x = potentialMinX; }
	else { outSize.x = potentialMinX; }
		// For Y
	int potentialMinY = findMinY();
	if (potentialMinY == UI_WVALUE_NONE) { outSize.y = 0; }
	if (potentialMinY < outSize.y) {}
	else if (potentialMinY > style.minY && style.minY > 0) { outSize.y = potentialMinY; }
	else { outSize.y = potentialMinY; }
	// 3. Let the size of the widget occupy as much as it can, up to it's maximum values
		// For X
	int potentialMaxX = findMaxX();
	//if (potentialMaxX == UI_WVALUE_NONE) { potentialMaxX = 0; }
	if (potentialMaxX == UI_WVALUE_NONE) { potentialMaxX = potentialMinX - (style.getMargin().left + style.getMargin().right); }
	if (potentialMaxX < potentialMinX) { outSize.x = potentialMaxX; }
	else if (potentialMaxX > style.maxX && style.maxX > 0 && style.relMaxX != true) { outSize.x = (int)style.maxX; }
	else { outSize.x = potentialMaxX; }
		// For Y
	int potentialMaxY = findMaxY();
	if (potentialMaxY == UI_WVALUE_NONE) { potentialMaxY = potentialMinY - (style.getMargin().top + style.getMargin().bottom); }
	if (potentialMaxY < potentialMinY) { outSize.y = potentialMaxY; }
	else if (potentialMaxY > style.maxY && style.maxY > 0 && style.relMaxY != true) { outSize.y = (int)style.maxY; }
	else { outSize.y = potentialMaxY; }
		// Return
	sizeX = outSize.x;
	sizeY = outSize.y;
	
	return outSize;
}
void Widget::placeWidget()
{
	if (checkVisibility()) { location = reportLocation(); }
	else if (!isRoot) { location = glm::ivec2(0, 0); }
	//location = reportLocation();
}
void Widget::checkOverflow()
{
	if (!checkVisibility() || style.overrideClipping == true) { applyOverflowMask = false; }
	else
	{
		std::shared_ptr<Widget> dir = (parentWidget.expired()) ? nullptr : parentWidget.lock();

		if (dir == nullptr && (style.overflowX == UI_OVERFLOW_HIDDEN || style.overflowY == UI_OVERFLOW_HIDDEN))
		{
			applyOverflowMask = true;
			overflowTarget = weak_from_this();
			overflowBox = glm::vec4(
				0.0f,
				0.0f,
				sizeX,
				sizeY
			);
			return;
		}
		bool intersectNextBox = false;
		glm::vec2 overflowOffsetOriginal = glm::vec2(0.0f);
		glm::vec4 intersectBox = glm::vec4(0.0f);
		glm::vec4 adj = glm::vec4(0.0f);
		while (dir != nullptr)
		{
			// Allow a widget with a BLOCK set on overflow to prevent all it's childs widgets from receiving overflow above it.
			// Widgets nested deeper into the blocking element, with overflow sandwiched between the block and child, should
			// still receive an overflow mask.
			WidgetStyle* dirStyle = &dir.get()->style;
			if (dirStyle->overrideClipping == true)
			{
				return;
			}
			if (dirStyle->overflowX == UI_OVERFLOW_HIDDEN || dirStyle->overflowY == UI_OVERFLOW_HIDDEN)
			{
				if (!intersectNextBox)
				{
					applyOverflowMask = intersectNextBox = true;
					overflowTarget = dir.get()->weak_from_this();
					overflowBox = glm::vec4(
						0.0f,
						0.0f,
						dir.get()->sizeX,
						dir.get()->sizeY);
					glm::vec2 screenOffset = overflowTarget.lock()->getScreenLocation(0, 0, false);
					screenOffset.y += overflowBox.w;
					overflowOffsetOriginal = screenOffset;
					overflowBox = glm::vec4(
						overflowBox.x + screenOffset.x,
						overflowBox.y + (WINDOW_HEIGHT - screenOffset.y),
						overflowBox.z + screenOffset.x,
						overflowBox.w + (WINDOW_HEIGHT - screenOffset.y)
					);
				}
				else
				{
					intersectBox = glm::vec4(
						0.0f,
						0.0f,
						dir.get()->sizeX,
						dir.get()->sizeY);
					glm::vec2 intersectOffset = dir.get()->getScreenLocation(0, 0, false);
					intersectOffset.y += intersectBox.w;
					intersectBox = glm::vec4(
						intersectBox.x + intersectOffset.x,
						intersectBox.y + (WINDOW_HEIGHT - intersectOffset.y),
						intersectBox.z + intersectOffset.x,
						intersectBox.w + (WINDOW_HEIGHT - intersectOffset.y));

					if (idAttrib == "tsf_fx_power")
					{
						//std::cout << "INTERSECT : " << vec4ToString(intersectBox) << std::endl;
						//std::cout << "OVERFLOW : " << vec4ToString(overflowBox) << std::endl;
						//std::cout << "OFFSET-ORIGINAL : " << vec2ToString(overflowOffsetOriginal) << std::endl;
					}
					if (intersectBox.w < overflowBox.w) { adj.w = overflowBox.w - intersectBox.w; }
					if (intersectBox.y > overflowBox.y) { adj.y = overflowBox.y - intersectBox.y; }

					if (idAttrib == "tsf_fx_power")
					{
						//std::cout << "OVERFLOW : " << vec4ToString(overflowBox) << std::endl;
					}
				}
			}
			dir = (dir.get()->parentWidget.expired()) ? nullptr : dir.get()->parentWidget.lock();
		}

		overflowBox = glm::vec4(
			overflowBox.x - overflowOffsetOriginal.x,
			overflowBox.y - (WINDOW_HEIGHT - overflowOffsetOriginal.y),
			overflowBox.z - overflowOffsetOriginal.x,
			overflowBox.w - (WINDOW_HEIGHT - overflowOffsetOriginal.y)
		);
		overflowOffset.w = adj.w;
		overflowOffset.y = adj.y;

		if (idAttrib == "tsf_fx_power")
		{
			//std::cout << "OUT-OFFSET : " << vec4ToString(overflowOffset) << std::endl;
			//std::cout << "OVERFLOW : " << vec4ToString(overflowBox) << std::endl;
			//sstd::cout << "" << std::endl;
		}
	}
}

std::shared_ptr<Widget> Widget::duplicate(std::shared_ptr<Widget> prototypeRoot, std::shared_ptr<Widget> parent, std::string rootID, bool isRoot)
{
	if (isRoot)
	{
		std::map<std::string, std::string> basicAttribs = {
			{"name", prototypeRoot.get()->nameAttrib},
			{"img", prototypeRoot.get()->imgAttrib.string()},
			{"value", prototypeRoot.get()->valueAttrib},
			{"innerContent", prototypeRoot.get()->innerContent},
		};
		std::shared_ptr<Widget> duplicateWidget = chromaIO.get()->ui.get()->createWidget(
			prototypeRoot.get()->type,
			basicAttribs,
			parent.get()->weak_from_this(),
			prototypeRoot.get()->style.makeCopy(),
			prototypeRoot.get()->shader
		);
		for (std::shared_ptr<Widget> childWidget : prototypeRoot.get()->childWidgets)
		{
			duplicateWidget.get()->childWidgets.push_back(childWidget.get()->duplicate(nullptr, duplicateWidget, rootID, false));
		}
		duplicateWidget.get()->_namespace = prototypeRoot.get()->_namespace;
		duplicateWidget.get()->classAttribs = prototypeRoot.get()->classAttribs;
		duplicateWidget.get()->groupsAttribs = prototypeRoot.get()->groupsAttribs;
		duplicateWidget.get()->isDraggable = prototypeRoot.get()->isDraggable;
		duplicateWidget.get()->isDroppable = prototypeRoot.get()->isDroppable;
		duplicateWidget.get()->dragType = prototypeRoot.get()->dragType;
		duplicateWidget.get()->dropType = prototypeRoot.get()->dropType;
		duplicateWidget.get()->callbackMap = prototypeRoot.get()->callbackMap;
		for (std::string className : duplicateWidget.get()->classAttribs)
		{
			chromaIO.get()->ui.get()->addToClassTable(className, rootID);
		}
		return duplicateWidget;
	}
	else
	{
		std::map<std::string, std::string> basicAttribs = {
			{"name", this->nameAttrib},
			{"img", this->imgAttrib.string()},
			{"value", this->valueAttrib},
			{"innerContent", this->innerContent},
		};
		std::shared_ptr<Widget> duplicateChild = chromaIO.get()->ui.get()->createWidget(
			this->type,
			basicAttribs,
			parent.get()->weak_from_this(),
			this->style.makeCopy(),
			this->shader
		);
		for (std::shared_ptr<Widget> childWidget : this->childWidgets)
		{
			duplicateChild.get()->childWidgets.push_back(childWidget.get()->duplicate(nullptr, childWidget, rootID, false));
		}
		duplicateChild.get()->_namespace = this->_namespace;
		duplicateChild.get()->classAttribs = this->classAttribs;
		duplicateChild.get()->groupsAttribs = this->groupsAttribs;
		duplicateChild.get()->isDraggable = this->isDraggable;
		duplicateChild.get()->isDroppable = this->isDroppable;
		duplicateChild.get()->dragType = this->dragType;
		duplicateChild.get()->dropType = this->dropType;
		duplicateChild.get()->callbackMap = this->callbackMap;
		return duplicateChild;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Widget Property Reporting
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Recursive function to get the minX value
int Widget::findMinX()
{
	int outMinX = 0;
	if (style.relMinX == true)
	{
		if (parentWidget.expired())
		{
			if (style.minX != UI_WVALUE_NONE) { outMinX = int(WINDOW_WIDTH * style.minX); }
			else { outMinX = WINDOW_WIDTH; }
		}
		else
		{
			if (style.minX != UI_WVALUE_NONE) { outMinX = int((parentWidget.lock().get()->findAvailableWidth(shared_from_this())) * style.minX); }
			else { outMinX = int(parentWidget.lock().get()->setSizeByParent().x); }
		}
	}
	else if ((int)style.minX == UI_WVALUE_INHERIT)
	{
		if (parentWidget.expired())
		{
			outMinX = WINDOW_WIDTH;
		}
		else
		{
			outMinX = parentWidget.lock().get()->findMinX();
		}
	}
	else 
	{ 
		//if (style.minX != UI_WVALUE_NONE || sizeX > 0)
		if (style.minX != UI_WVALUE_NONE)
		{ 
			outMinX = int((sizeX > style.minX) ? sizeX : (style.minX > UI_WVALUE_NONE / 2) ? sizeX : style.minX);
		}
		else { outMinX = 0; }
	}
	return outMinX;
}
// Recursive function to get the maxX value
int Widget::findMaxX()
{
	int outMaxX = UI_WVALUE_NONE;
	if (style.relMaxX == true)
	{
		if (parentWidget.expired())
		{
			if (style.maxX != UI_WVALUE_NONE) { outMaxX = int(WINDOW_WIDTH * style.maxX)
				- style.getMargin().left - style.getMargin().right; }
			else { outMaxX = WINDOW_WIDTH
				- style.getMargin().left - style.getMargin().right;}
		}
		else
		{
			if (style.maxX != UI_WVALUE_NONE) { outMaxX = int((parentWidget.lock().get()->findAvailableWidth(shared_from_this())) * style.maxX); }
			else { outMaxX = (parentWidget.lock().get()->setSizeByParent().x); }
		}
	}
	else if ((int)style.maxX == UI_WVALUE_INHERIT)
	{
		if (parentWidget.expired())
		{
			outMaxX = WINDOW_WIDTH - style.getMargin().left - style.getMargin().right;
		}
		else
		{
			//outMaxX = parentWidget.lock().get()->findMaxX() - style.getMargin().left - style.getMargin().right;
			outMaxX = parentWidget.lock().get()->findMaxX();
			if (outMaxX != UI_WVALUE_NONE)
			{
				outMaxX -= (style.getMargin().left + style.getMargin().right);
			}
		}
	}
	else
	{
		
		//if (style.maxX != UI_WVALUE_NONE || sizeX > 0)
		if (style.maxX != UI_WVALUE_NONE)
		{
			//outMaxX = (sizeX > style.maxX) ? sizeX : (style.maxX > UI_WVALUE_NONE / 2) ? sizeX : style.maxX;
			outMaxX = (int)style.maxX;
		}
		else 
		{ 
			outMaxX = UI_WVALUE_NONE;
		}
		
		//outMaxX = (sizeX > style.maxX) ? sizeX : (style.maxX > UI_WVALUE_NONE / 2) ? sizeX : style.maxX;
	}
	return outMaxX;
}
int Widget::findMinY()
{
	int outMinY = 0;
	if (style.relMinY == true)
	{
		if (parentWidget.expired())
		{
			if (style.minY != UI_WVALUE_NONE) { outMinY = int(WINDOW_HEIGHT * style.minY); }
			else { outMinY = WINDOW_HEIGHT; }
		}
		else
		{
			if (style.minY != UI_WVALUE_NONE) { outMinY = int((parentWidget.lock().get()->findAvailableHeight(shared_from_this())) * style.minY); }
			else { outMinY = int(parentWidget.lock().get()->setSizeByParent().y); }
		}
	}
	else if ((int)style.minY == UI_WVALUE_INHERIT)
	{
		if (parentWidget.expired())
		{
			outMinY = WINDOW_HEIGHT;
		}
		else
		{
			outMinY = parentWidget.lock().get()->findMinY();
		}
	}
	else
	{
		if (style.minY != UI_WVALUE_NONE)
		{
			outMinY = int((sizeY > style.minY) ? sizeY : (style.minY > UI_WVALUE_NONE / 2) ? sizeY : style.minY);
		}
		else { outMinY = 0; }
	}
	return outMinY;
}
// Recursive function to get the maxY value
int Widget::findMaxY()
{
	int outMaxY = UI_WVALUE_NONE;
	if (style.relMaxY == true)
	{
		if (parentWidget.expired())
		{
			if (style.maxY != UI_WVALUE_NONE) { outMaxY = int(WINDOW_HEIGHT * style.maxY) 
				- style.getMargin().top - style.getMargin().bottom; }
			else { outMaxY = WINDOW_HEIGHT
				- style.getMargin().top - style.getMargin().bottom; }
		}
		else
		{
			if (style.maxY != UI_WVALUE_NONE) { outMaxY = int((parentWidget.lock().get()->findAvailableHeight(shared_from_this())) * style.maxY); }
			else { outMaxY = (parentWidget.lock().get()->setSizeByParent().y); }
		}
	}
	else if ((int)style.maxY == UI_WVALUE_INHERIT)
	{
		if (parentWidget.expired())
		{
			outMaxY = WINDOW_HEIGHT - style.getMargin().top - style.getMargin().bottom;
		}
		else
		{
			outMaxY = parentWidget.lock().get()->findMaxY();
			if (outMaxY != UI_WVALUE_NONE)
			{
				outMaxY -= (style.getMargin().top + style.getMargin().bottom);
			}
		}
	}
	else
	{
		if (style.maxY != UI_WVALUE_NONE)
		{
			outMaxY = (int)style.maxY;
		}
		else { outMaxY = UI_WVALUE_NONE; }
	}
	return outMaxY;
}

// Find available space for relative-sized widgets to occupy
int Widget::findAvailableWidth(std::shared_ptr<Widget> askingWidget)
{
	int outWidth = 0;
	int firstPositionedWidget = -1;
	for (int i = 0; i < childWidgets.size(); i++)
	{
		Widget* child = childWidgets[i].get();
		if (child != askingWidget.get())
		{
			if (firstPositionedWidget == -1) { firstPositionedWidget = i; }
			if (child->style.position != UI_POSITION_ABSOLUTE)
			{
				outWidth += child->sizeX;
				if (childWidgets.size() == 0)
				{
					outWidth += child->style.getMargin().left;
					outWidth += child->style.getMargin().right;
				}
				else if (i == 0)
				{
					outWidth += child->style.getMargin().left;
					outWidth += child->style.getBorder().right;
				}
				else if (i > 0 && i < childWidgets.size() - 1)
				{
					outWidth += child->style.getBorder().left;
					outWidth += child->style.getBorder().right;
				}
				else if (i == childWidgets.size() - 1)
				{
					outWidth += child->style.getMargin().right;
					outWidth += child->style.getBorder().left;
				}
			}
		}
		else if (child == askingWidget.get())
		{
			if (childWidgets.size() == 0)
			{
				outWidth += askingWidget.get()->style.getMargin().left;
				outWidth += askingWidget.get()->style.getMargin().right;
			}
			else if (i == 0)
			{
				outWidth += askingWidget.get()->style.getMargin().left;
				outWidth += askingWidget.get()->style.getBorder().right;
			}
			else if (i > 0 && i < childWidgets.size() - 1)
			{
				bool isLastPositionedWidget = false;
				int k = i + 1;
				while (k < childWidgets.size() - 1)
				{
					if (childWidgets[k].get()->style.position != UI_POSITION_ABSOLUTE)
					{
						isLastPositionedWidget = true; break;
					}
					k++;
				}
				if (firstPositionedWidget == -1)
				{
					outWidth += askingWidget.get()->style.getMargin().left;
				}
				else
				{
					outWidth += askingWidget.get()->style.getBorder().left;
				}
				if (isLastPositionedWidget)
				{
					outWidth += askingWidget.get()->style.getBorder().right;
				}
				else
				{
					outWidth += askingWidget.get()->style.getMargin().right;
				}
			}
			else if (i == childWidgets.size() - 1)
			{
				outWidth += askingWidget.get()->style.getBorder().left;
				outWidth += askingWidget.get()->style.getMargin().right;
			}
		}
	}
	outWidth = sizeX - outWidth;
	return outWidth;
}

int Widget::findAvailableHeight(std::shared_ptr<Widget> askingWidget)
{
	int outHeight = 0;
	int firstPositionedWidget = -1;
	for (int i = 0; i < childWidgets.size(); i++)
	{
		Widget* child = childWidgets[i].get();
		if (child != askingWidget.get())
		{
			if (firstPositionedWidget == -1) { firstPositionedWidget = i; }
			if (child->style.position != UI_POSITION_ABSOLUTE)
			{
				outHeight += child->sizeY;
				if (childWidgets.size() == 0)
				{
					outHeight += child->style.getMargin().top;
					outHeight += child->style.getMargin().bottom;
				}
				else if (i == 0)
				{
					outHeight += child->style.getMargin().top;
					outHeight += child->style.getBorder().bottom;
				}
				else if (i > 0 && i < childWidgets.size() - 1)
				{
					outHeight += child->style.getBorder().top;
					outHeight += child->style.getBorder().bottom;
				}
				else if (i == childWidgets.size() - 1)
				{
					outHeight += child->style.getMargin().bottom;
					outHeight += child->style.getBorder().top;
				}
			}
		}
		else if (child == askingWidget.get())
		{
			if (childWidgets.size() == 0)
			{
				outHeight += askingWidget.get()->style.getMargin().top;
				outHeight += askingWidget.get()->style.getMargin().bottom;
			}
			else if (i == 0)
			{
				outHeight += askingWidget.get()->style.getBorder().bottom;
				outHeight += askingWidget.get()->style.getMargin().top;
			}
			else if (i > 0 && i < childWidgets.size() - 1)
			{
				bool isLastPositionedWidget = false;
				int k = i + 1;
				while (k < childWidgets.size() - 1)
				{
					if (childWidgets[k].get()->style.position != UI_POSITION_ABSOLUTE)
					{
						isLastPositionedWidget = true; break;
					}
					k++;
				}
				if (firstPositionedWidget == -1)
				{
					outHeight += askingWidget.get()->style.getMargin().top;
				}
				else
				{
					outHeight += askingWidget.get()->style.getBorder().top;
				}
				if (isLastPositionedWidget)
				{
					outHeight += askingWidget.get()->style.getBorder().bottom;
				}
				else
				{
					outHeight += askingWidget.get()->style.getMargin().bottom;
				}
			}
			else if (i == childWidgets.size() - 1)
			{
				outHeight += askingWidget.get()->style.getMargin().bottom;
				outHeight += askingWidget.get()->style.getBorder().top;
			}
		}
	}
	outHeight = sizeY - outHeight;
	return outHeight;
}


// Recursive function to get the fontPath
std::filesystem::path Widget::findFontPath()
{
	std::filesystem::path out = "";
	std::shared_ptr<Widget> parent = (parentWidget.expired()) ? nullptr : parentWidget.lock();
	if (parent = nullptr) { return out; }
	else if (parent.get()->style.fontPath == "")
	{
		out = parent.get()->findFontPath();
	}
	else
	{
		out = parent.get()->style.fontPath;
	}
	return out;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Location Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Report and set the location (absolute position within it's container) for this widget
glm::ivec2 Widget::reportLocation()
{
	glm::ivec2 outLoc;
	// 1. Establish this widget's position
	if (!parentWidget.expired())
	{
		// 2. Ask the parent widget to report back where this widget should
		// be placed. This is a virtual function so per-widget behavior is defined
		outLoc = parentWidget.lock().get()->findWidgetLocation(shared_from_this());
		location = outLoc;
	}
	else
	{
		// 2. This is a root widget, so the location is the base loccation value
		// 2A. Ignore the widget's pxPosition style if it is root. Moving the widget
		// update's it's location directly
		outLoc = location;
	}
	return outLoc;
}
// Iterate up the hierarchy tree to get the sum of the parent widget's locations
glm::ivec2 Widget::chainLocation()
{
	glm::ivec2 out = glm::ivec2(0, 0);
	if (parentWidget.expired())
	{
		return out;
	}
	std::shared_ptr<Widget> dir = (parentWidget.expired()) ? nullptr : parentWidget.lock();
	// Widget is out of normal flow
	if (style.position == UI_POSITION_ABSOLUTE)
	{
		bool firstValidDir = false;
		while (dir != nullptr)
		{
			// Need to find the first widget that's in flow
			if (firstValidDir == false)
			{
				//if (dir.get()->style.position == UI_POSITION_RELATIVE) { firstValidDir = true; }
				if (dir.get()->style.position == UI_POSITION_STATIC) { firstValidDir = true; }
			}
			// Once found, begin collecting location
			if (firstValidDir == true)
			{
				out.x += dir.get()->location.x;
				out.y += dir.get()->location.y;
			}
			dir = (dir.get()->parentWidget.expired()) ? nullptr : dir.get()->parentWidget.lock();
		}
	}
	else
	{
		while (dir != nullptr)
		{
			out.x += dir.get()->location.x;
			out.y += dir.get()->location.y;
			dir = (dir.get()->parentWidget.expired()) ? nullptr : dir.get()->parentWidget.lock();
		}
	}
	
	return out;
}
// Get the absolute screenLocation of the widget in -1 -> 1 coordinates
// Take notice that the Y location must be flipped negative
glm::vec2 Widget::getScreenLocation(int offsetX, int offsetY, bool createNDC)
{
	glm::ivec2 loc = location;
	loc += chainLocation();
	glm::vec2 out = loc;
	if (createNDC)
	{
		out.x = (float((loc.x + offsetX) * 2) / WINDOW_WIDTH);
		out.y = (float((loc.y + offsetY) * 2) / WINDOW_HEIGHT);
		out.y *= -1;
	}
	return out;
}
glm::vec4 Widget::calculateOverflowMask()
{
	glm::vec2 screenOffset = overflowTarget.lock()->getScreenLocation(0, 0, false);
	screenOffset.y += overflowBox.w;
	glm::vec4 outBox = glm::vec4(
		overflowBox.x + screenOffset.x,
		overflowBox.y + (WINDOW_HEIGHT - screenOffset.y),
		overflowBox.z + screenOffset.x,
		overflowBox.w + (WINDOW_HEIGHT - screenOffset.y)
	);
	// Warning : This functionality for intersecting multiple overflow masks is only applied to the top edge currently, for scrolling
	// multiple overflows upward or left/right, this is not implemented yet.
	outBox.w -= overflowOffset.w;
	outBox.y -= overflowOffset.w;
	//outBox.w += -overflowOffset.y;
	//outBox.y += overflowOffset.y;
	return outBox;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Click Handler Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Return single-depth result of position query
bool Widget::mouseSweep(double x, double y)
{
	// First check simple collision for this widget
	glm::ivec2 p1 = getScreenLocation(0, 0, false);
	glm::ivec2 p2 = glm::ivec2(p1.x + sizeX, p1.y + sizeY);
	if (checkVisibility())
	{
		if (!overflowTarget.expired() && style.zIndex <= overflowTarget.lock()->style.zIndex) {
			p1 = overflowTarget.lock()->getScreenLocation(0, 0, false);
			p2 = p1 + overflowTarget.lock()->getSize();
		}
		if (int(x) >= p1.x && int(x) <= p2.x)
		{
			if (int(y) >= p1.y && int(y) <= p2.y)
			{
				return true;
			}
		}
		if (outsideBoundWidgets.size() != 0)
		{
			bool result = false;
			// Only the top-most root widget should have anything in this vector
			//for (std::weak_ptr<Widget> widget : outsideBoundWidgets)
			for (auto const& item : outsideBoundWidgets)
			{
				if (item.second.expired() || item.second.lock().get()->style.visibility == UI_VISIBILITY_HIDE) { continue; }
				if (item.second.lock().get()->mouseSweep(x, y)) { 
					result = true; 
				}
			}
			if (result)
			{
				return true;
			}
		}
	}
	return false;
}

// Return full-depth result of inputEvent
bool Widget::mouseHit(Input* dat, bool dragOnly, bool shouldFocus, 
	std::deque<Widget*>& clickStack, std::deque<Widget*>& focusStack, bool zIndexExists, unsigned int& maxZIndex)
{
	// Focus testing works from the top down, the first widget to have a callback
	// captures the focus
	bool wasFocusStored = false;
	int focusResult = 0;
	if (dat->action != InputAction::release && !chromaIO.get()->getUI().get()->didFocusThisClick && shouldFocus)
	{
		focusResult = selfFocus(dat, zIndexExists, maxZIndex, false);
		if (focusResult != 0)
		{
			if (!zIndexExists) 
			{ 
				chromaIO.get()->getUI().get()->didFocusThisClick = true;
				chromaIO.get()->getUI().get()->updateFocusWidget(weak_from_this());
			}
			else
			{
				// When result is 2, clear the hoverStack first
				if (focusResult == 2) { focusStack.clear(); }
				focusStack.push_front(this); wasFocusStored = true;
			}
		}
	}

	// Hit testing works from the bottom up, only once childWidgets = 0
	// will self-tests begin to run
	bool wasHandled = false;
	bool wasHitStored = false;
	for (int i = (int)childWidgets.size() - 1; i >= 0; i--)
	{
		wasHandled = childWidgets[i].get()->mouseHit(dat, dragOnly, shouldFocus, clickStack, focusStack, zIndexExists, maxZIndex);
		if (wasHandled && !zIndexExists) { return true; }
		else if (wasHandled && zIndexExists) { wasHitStored = true; clickStack.push_front(this); }
	}

	// Do Bottom up events
	int hitResult = selfHit(dat, dragOnly, zIndexExists, maxZIndex, false);
	if (hitResult != 0)
	{
		if (!zIndexExists) { return true; }
		else
		{
			// When result is 2, clear the hoverStack first
			if (hitResult == 2) { clickStack.clear(); }
			clickStack.push_front(this); wasHitStored = true;
		}
	}
	return wasHitStored;
}

// Perform self-hit Test
int Widget::selfHit(Input* dat, bool dragOnly, bool storeHitEvent, unsigned int& maxZIndex, bool ignoreCollision)
{
	int wasTriggered = 0;
	if (checkVisibility() && (ignoreCollision || checkPointCollision_self(glm::ivec2(dat->x, dat->y))))
	{
		// Bypass area for checking against drag events. Desireable to immediate call when a widget
		// has a drag function. Input appears sluggish if drag is not immediate.
		if (dragOnly && !chromaIO.get()->getDragStart())
		{
			// Check for ondragstart trigger
			if (!storeHitEvent && callbackMap.count("ondragstart"))
			{
				if (callbackMap.at("ondragstart").size() != 0)
				{
					chromaIO.get()->getUI().get()->activeWidget = weak_from_this();
					chromaIO.get()->scriptConsole.get()->run(callbackMap.at("ondragstart"), _namespace);
					chromaIO.get()->getUI().get()->activeWidget.reset();
					chromaIO.get()->setDragStart();
				}
				isBeingDragged = true;
				wasTriggered = 1;
			}
			// Notify to begin calling ondrag next time the mouse moves while left click is held
			if (!storeHitEvent && callbackMap.count("ondrag") && callbackMap.at("ondrag").size() != 0)
			{
				chromaIO.get()->ui.get()->dragWidget = weak_from_this();
				chromaIO.get()->setDragStart();
				isBeingDragged = true;
				wasTriggered = 1;
			}
			if (storeHitEvent && style.zIndex >= maxZIndex)
			{
				wasTriggered = 1;
				if (style.zIndex > maxZIndex)
				{
					wasTriggered = 2;
				}
				maxZIndex = style.zIndex;
			}
			return wasTriggered;
		}
		// Regular mouse click check events
		std::string callbackName = "";
		// Only allow a click action trigger on mouse-down
		if (dat->action != InputAction::release)
		{
			switch (dat->button)
			{
			case InputMouseButton::left:
				callbackName = "onclick"; break;
			case InputMouseButton::left_dbl:
				callbackName = "ondblclick"; break;
			case InputMouseButton::right:
				callbackName = "onrightclick"; break;
			case InputMouseButton::right_dbl:
				callbackName = "ondblrightclick"; break;
			case InputMouseButton::middle:
				callbackName = "onmiddleclick"; break;
			case InputMouseButton::middle_dbl:
				callbackName = "ondblmiddleclick"; break;
			default:
				callbackName = ""; break;
			}
			// Call the matched callback function
			if (!storeHitEvent && callbackMap.count(callbackName))
			{
				if (callbackMap.at(callbackName).size() != 0)
				{
					chromaIO.get()->getUI().get()->activeWidget = weak_from_this();
					chromaIO.get()->scriptConsole.get()->run(callbackMap.at(callbackName), _namespace);
					chromaIO.get()->getUI().get()->activeWidget.reset();
				}
				wasTriggered = 1;
			}
			// Also call the dragstart callback if it exists
			if (!storeHitEvent && (dat->button == InputMouseButton::left || dat->button == InputMouseButton::left_dbl) && !chromaIO.get()->getDragStart())
			{
				if (callbackMap.count("ondragstart"))
				{
					if (callbackMap.at("ondragstart").size() != 0)
					{
						chromaIO.get()->getUI().get()->activeWidget = weak_from_this();
						chromaIO.get()->scriptConsole.get()->run(callbackMap.at("ondragstart"), _namespace);
						chromaIO.get()->getUI().get()->activeWidget.reset();
					}
					isBeingDragged = true;
					wasTriggered = 1;
				}
				// Notify to begin calling ondrag next time the mouse moves while left click is held
				if (callbackMap.count("ondrag") && callbackMap.at("ondrag").size() != 0)
				{
					chromaIO.get()->ui.get()->dragWidget = weak_from_this();
					chromaIO.get()->setDragStart();
					isBeingDragged = true;
					wasTriggered = 1;
				}
			}
		}
		if (callbackMap.count("onrelease"))
		{
			if (dat->action == InputAction::release)
			{
				if (!storeHitEvent)
				{
					chromaIO.get()->getUI().get()->clickReleaseGate = false;
					if (callbackMap.at("onrelease").size() != 0)
					{
						chromaIO.get()->getUI().get()->activeWidget = weak_from_this();
						chromaIO.get()->scriptConsole.get()->run(callbackMap.at("onrelease"), _namespace);
						chromaIO.get()->getUI().get()->activeWidget.reset();
					}
				}
			}
			else
			{
				if (!storeHitEvent)
				{
					chromaIO.get()->getUI().get()->clickReleaseGate = true;
					chromaIO.get()->getUI().get()->clickedWidget = weak_from_this();
				}
			}
			if (!storeHitEvent) { wasTriggered = 1; }
		}
		if (storeHitEvent)
		{
			if (style.zIndex >= maxZIndex)
			{
				wasTriggered = 1;
				if (style.zIndex > maxZIndex)
				{
					wasTriggered = 2;
				}
				maxZIndex = style.zIndex;
			}
		}
	}
	return wasTriggered;
}
// Perform self-focus Test
int Widget::selfFocus(Input* dat, bool storeHitEvent, unsigned int& maxZIndex, bool ignoreCollision)
{
	int wasCaptured = 0;
	if (checkVisibility() && (ignoreCollision || checkPointCollision_self(glm::ivec2(dat->x, dat->y))))
	{
		if (!storeHitEvent && callbackMap.count("onfocus"))
		{
			// Only check for the 'onfocus' callback
			// Only fire if the widget is not also the current focusWidget
			if (dat->action != InputAction::release && !chromaIO.get()->ui.get()->interruptFocus && chromaIO.get()->getUI().get()->focusWidget.lock().get() != this)
			{
				if (callbackMap.at("onfocus").size() != 0)
				{
					chromaIO.get()->getUI().get()->activeWidget = weak_from_this();
					chromaIO.get()->scriptConsole.get()->run(callbackMap.at("onfocus"), _namespace);
					chromaIO.get()->getUI().get()->activeWidget.reset();
				}
			}
			// Declare wasCaptured regardless of whether a callback was fired
			wasCaptured = 1;
		}
		// Capture fallthrough when an input-type widget did not have an onfocus callback
		if (!storeHitEvent && isInputType(this->type))
		{
			wasCaptured = 1;
		}
		if (storeHitEvent && style.zIndex >= maxZIndex)
		{
			wasCaptured = 1;
			if (style.zIndex > maxZIndex)
			{
				wasCaptured = 2;
			}
			maxZIndex = style.zIndex;
		}
	}
	return wasCaptured;
}
// Perform self-blur Test
bool Widget::selfBlur()
{
	bool wasBlurred = false;
	if (checkVisibility() && callbackMap.count("onblur"))
	{
		std::cout << "WIDGET-BLURRED::ID,CLASSES= " <<
			idAttrib + ", " << stringVecToString(classAttribs, true) << std::endl;
		if (callbackMap.at("onblur").size() != 0)
		{
			chromaIO.get()->getUI().get()->activeWidget = weak_from_this();
			chromaIO.get()->scriptConsole.get()->run(callbackMap.at("onblur"), _namespace);
			chromaIO.get()->getUI().get()->activeWidget.reset();
		}
		wasBlurred = true;
	}
	// Allow the blur to bubble if this widget didn't blur
	if (!wasBlurred && !parentWidget.expired())
	{
		wasBlurred = parentWidget.lock().get()->selfBlur();
	}
	return wasBlurred;
}


// Hover Test
bool Widget::mouseHover(Input* dat, std::deque<Widget*> &hoverStack, bool zIndexExists, unsigned int &maxZIndex)
{
	// Hit testing works from the bottom up, only once childWidgets = 0
	// will self-tests begin to run
	bool wasHandled = false;
	bool wasStored = false;
	for (int i = (int)childWidgets.size() - 1; i >= 0; i--)
	{
		wasHandled = childWidgets[i].get()->mouseHover(dat, hoverStack, zIndexExists, maxZIndex);
		if (wasHandled && !zIndexExists) { return true; }
		else if (wasHandled && zIndexExists) { wasStored = true; hoverStack.push_front(this); }
	}
	int result = selfHover(dat, zIndexExists, maxZIndex, false);
	if (result != 0) {
		if (!zIndexExists) { return true; }
		else 
		{
			// When result is 2, clear the hoverStack first
			if (result == 2) { hoverStack.clear(); }
			hoverStack.push_front(this); wasStored = true;
		}
	}
	return wasStored;
}
// Perform self-hover Test
int Widget::selfHover(Input* dat, bool storeHitResult, unsigned int& maxZIndex, bool ignoreCollision)
{
	// Do self-test
	int wasTriggered = 0;
	if (checkVisibility() && (ignoreCollision || checkPointCollision_self(glm::ivec2(dat->x, dat->y))))
	{
		// Register this widget into the onmouseleave handler, regardless of whether the leave trigger exists
		if (!isEntered)
		{
			if (!storeHitResult)
			{
				isEntered = true;
				chromaIO.get()->getUI().get()->enteredWidgets.insert(std::pair<int, std::weak_ptr<Widget>>(UEID, weak_from_this()));
			}
			if (callbackMap.count("onmouseenter") && callbackMap.at("onmouseenter").size() != 0)
			{
				if (!storeHitResult)
				{
					chromaIO.get()->getUI().get()->activeWidget = weak_from_this();
					chromaIO.get()->scriptConsole.get()->run(callbackMap.at("onmouseenter"), _namespace); wasTriggered = 1;
					chromaIO.get()->getUI().get()->activeWidget.reset();
				}
			}
		}
		// Trigger onmouseover events if available
		if (callbackMap.count("onmouseover") && callbackMap.at("onmouseover").size() != 0)
		{
			if (!storeHitResult)
			{
				chromaIO.get()->getUI().get()->overWidgets.insert(std::pair<int, std::weak_ptr<Widget>>(UEID, weak_from_this()));

				chromaIO.get()->getUI().get()->activeWidget = weak_from_this();
				chromaIO.get()->scriptConsole.get()->run(callbackMap.at("onmouseover"), _namespace); wasTriggered = 1;
				chromaIO.get()->getUI().get()->activeWidget.reset();
			}
		}
		// If instructed to store the hit result, when ZIndex is active, always return true. Each 'z-index' is
		// essentially a 'layer', which blocks all events from falling through to lower z-indices. This also
		// will trigger a widget onmouseleave event for any entered widget with a lower z-index than the new max
		if (storeHitResult && style.zIndex >= maxZIndex) 
		{ 
			wasTriggered = 1;
			if (style.zIndex > maxZIndex)
			{
				wasTriggered = 2;
				chromaIO.get()->ui.get()->widgetLeaveTest(dat, style.zIndex);
			}
			maxZIndex = style.zIndex;
		}
	}
	return wasTriggered;
}

// Trigger onmouseleave event
bool Widget::selfLeave(Input* dat, unsigned int maxZIndex)
{
	bool didLeave = false;
	if (!checkVisibility() || (!isBeingDragged && !checkPointCollision_self(glm::ivec2(dat->x, dat->y)) || style.zIndex < maxZIndex))
	{
		//std::cout << "LEAVE" << std::endl;
		if (callbackMap.count("onmouseleave") && callbackMap.at("onmouseleave").size() != 0)
		{
			chromaIO.get()->getUI().get()->activeWidget = weak_from_this();
			chromaIO.get()->scriptConsole.get()->run(callbackMap.at("onmouseleave"), _namespace);
			chromaIO.get()->getUI().get()->activeWidget.reset();
		}
		isEntered = false;
		didLeave = true;
	}
	return didLeave;
}

// Trigger a ondrag event
bool Widget::selfDrag(Input* dat)
{
	bool didDrag = false;
	if (callbackMap.count("ondrag"))
	{
		chromaIO.get()->ui.get()->activeWidget = weak_from_this();
		chromaIO.get()->scriptConsole.get()->run(callbackMap.at("ondrag"), _namespace);
		chromaIO.get()->ui.get()->activeWidget.reset();
		didDrag = true;
	}
	return didDrag;
}

// Trigger a ondragend event
bool Widget::selfDragend(Input* dat)
{
	bool didEnd = false;
	if (callbackMap.count("ondragend"))
	{
		chromaIO.get()->ui.get()->activeWidget = weak_from_this();
		chromaIO.get()->scriptConsole.get()->run(callbackMap.at("ondragend"), _namespace);
		chromaIO.get()->ui.get()->activeWidget.reset();
		didEnd = true;
	}
	isBeingDragged = false;
	return didEnd;
}

bool Widget::checkPointCollision_self(glm::ivec2 point)
{
	glm::ivec2 p1 = getScreenLocation(0, 0, false);
	glm::ivec2 p2 = glm::ivec2(p1.x + sizeX, p1.y + sizeY);
	bool result = false;
	if (int(point.x) >= p1.x && int(point.x) <= p2.x)
	{
		if (int(point.y) >= p1.y && int(point.y) <= p2.y)
		{
			result = true;
		}
	}
	return result;
}
bool Widget::checkPointCollision_other(glm::ivec2 point, glm::ivec4 bounds)
{
	glm::ivec2 p1 = glm::ivec2(bounds.x, bounds.y);
	glm::ivec2 p2 = glm::ivec2(bounds.z, bounds.w);
	bool result = false;
	if (int(point.x) >= p1.x && int(point.x) <= p2.x)
	{
		if (int(point.y) >= p1.y && int(point.y) <= p2.y)
		{
			result = true;
		}
	}
	return result;
}

void Widget::checkOutofBoundsWidgets(std::weak_ptr<Widget> root, glm::ivec4 rootBounds)
{
	// This checks the child of root widgets to see if there are any widgets outside the colliding
	// box of the root, to be considered in mouse sweep tests. This is necessary because
	// the sweep test is only one measure deep, and will not be able to see if a child widget
	// is outside those root bounds. This creates a supplement list of additional widgets to
	// check when sweeping.
	// First check thisWidget if any portion is outside the root Widget
	if (!root.expired())
	{
		std::shared_ptr<Widget> rootGet = root.lock();
		int resultOther = checkBoxCollision_complexOther(rootBounds);
		if (resultOther != 2 && style.zIndex > rootGet.get()->style.zIndex && checkVisibility())
		{
			// If the parent of thisWidget is the original root query, push it into the stack
			if (!parentWidget.expired() && parentWidget.lock() == root.lock())
			{
				if (outsideBoundWidgets.count(UEID) == 0)
				{
					rootGet.get()->outsideBoundWidgets.insert(std::pair<int, std::weak_ptr<Widget>>(UEID, weak_from_this()));
				}
			}
			// Check if this is the top most widget for an out of bounds widget
			else if (!parentWidget.expired() && parentWidget.lock()->style.zIndex <= rootGet.get()->style.zIndex)
			{
				if (outsideBoundWidgets.count(UEID) == 0)
				{
					rootGet.get()->outsideBoundWidgets.insert(std::pair<int, std::weak_ptr<Widget>>(UEID, weak_from_this()));
				}
			}
			// Else, check if the widget falls wholly within any one of it's chained parents
			// which are not the original root query.
			else
			{
				glm::ivec4 thisBox = getColliderBox();
				std::weak_ptr<Widget> parent = parentWidget;
				while (!parent.expired() && parent.lock() != rootGet)
				{
					int resultSelf = parent.lock().get()->checkBoxCollision_complexSelf(thisBox);
					if (resultSelf == 2)
					{
						if (rootGet.get()->outsideBoundWidgets.count(UEID) == 1)
						{
							rootGet.get()->outsideBoundWidgets.erase(UEID);
						}
						break;
					}
					else if (!parent.lock().get()->parentWidget.expired() && parent.lock().get()->parentWidget.lock() != rootGet)
					{
						parent = parent.lock().get()->parentWidget;
					}
					else
					{
						if (outsideBoundWidgets.count(UEID) == 0)
						{
							rootGet.get()->outsideBoundWidgets.insert(std::pair<int, std::weak_ptr<Widget>>(UEID, weak_from_this()));
						}
						break;
					}
				}
			}
		}
		else
		{
			if (rootGet.get()->outsideBoundWidgets.count(UEID) == 1)
			{
				rootGet.get()->outsideBoundWidgets.erase(UEID);
			}
		}
	}
	for (std::shared_ptr<Widget> child : childWidgets)
	{
		child.get()->checkOutofBoundsWidgets(root, rootBounds);
	}
}

glm::ivec4 Widget::getColliderBox()
{
	glm::ivec2 p1 = location;
	p1 += chainLocation();
	glm::ivec2 p2 = glm::ivec2(p1.x + sizeX, p1.y + sizeY);
	return glm::ivec4(p1, p2);
}


int Widget::checkBoxCollision_complexSelf(glm::ivec4 testBox)
{
	glm::ivec2 p1, p2, p3, p4;
	glm::bvec4 result = glm::bvec4(false);
	p1 = glm::ivec2(testBox.x, testBox.y);
	p4 = glm::ivec2(testBox.z, testBox.w);
	p2 = glm::ivec2(p4.x, p1.y);
	p3 = glm::ivec2(p1.x, p4.y);
	if (checkPointCollision_self(p1)) { result.x = true; }
	if (checkPointCollision_self(p2)) { result.y = true; }
	if (checkPointCollision_self(p3)) { result.z = true; }
	if (checkPointCollision_self(p4)) { result.w = true; }
	if (result.x && result.y && result.z && result.w) { return 2; } // Full overlap
	if (result.x || result.y || result.z || result.w) { return 1; } // Partial overlap
	return 0;														// No overlap
}

int Widget::checkBoxCollision_complexOther(glm::ivec4 testBox)
{
	glm::ivec4 thisBox = getColliderBox();
	glm::ivec2 p1, p2, p3, p4;
	glm::bvec4 result = glm::bvec4(false);
	p1 = glm::ivec2(thisBox.x, thisBox.y);
	p4 = glm::ivec2(thisBox.z, thisBox.w);
	p2 = glm::ivec2(p4.x, p1.y);
	p3 = glm::ivec2(p1.x, p4.y);
	if (checkPointCollision_other(p1, testBox)) { result.x = true; }
	if (checkPointCollision_other(p2, testBox)) { result.y = true; }
	if (checkPointCollision_other(p3, testBox)) { result.z = true; }
	if (checkPointCollision_other(p4, testBox)) { result.w = true; }
	if (result.x && result.y && result.z && result.w) { return 2; } // Full overlap
	if (result.x || result.y || result.z || result.w) { return 1; } // Partial overlap
	return 0;														// No overlap
}

bool Widget::checkVisibility()
{
	if (style.visibility != UI_VISIBILITY_HIDE)
	{
		bool isVisible = true;
		std::weak_ptr<Widget> parent = parentWidget;
		while (isVisible && !parent.expired())
		{
			isVisible = parent.lock().get()->checkVisibility();
			parent = parent.lock().get()->parentWidget;
		}
		return isVisible;
	}
	else { return false; }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Child Widget Lookup Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::weak_ptr<Widget> Widget::getRoot()
{
	if (isRoot) { return weak_from_this(); }
	std::weak_ptr<Widget> parent = parentWidget;
	while (!parent.expired())
	{
		if (parent.lock().get()->parentWidget.expired() || parent.lock().get()->isRoot)
		{
			break;
		}
		parent = parent.lock().get()->parentWidget;
	}
	if (parent.lock().get()->isRoot) { return parent; }
	else { parent.reset(); return parent; }
}

std::vector<int> Widget::reportParentUEIDChain()
{
	std::vector<int> outVec;

	if (isRoot) { return outVec; }
	std::weak_ptr<Widget> parent = parentWidget;
	while (!parent.expired())
	{
		if (parent.lock().get()->parentWidget.expired() || parent.lock().get()->isRoot)
		{
			outVec.push_back(parent.lock().get()->getUEID());
			break;
		}
		else
		{
			outVec.push_back(parent.lock().get()->getUEID());
			parent = parent.lock().get()->parentWidget;
		}
	}
	return outVec;
}

void Widget::getChild_byClass(std::shared_ptr<std::vector<std::weak_ptr<Widget>>> bucket, std::string className, std::string idExclusion)
{
	for (std::shared_ptr<Widget> child : childWidgets)
	{
		child.get()->getChild_byClass(bucket, className, idExclusion);
	}
	for (std::string classAttrib : classAttribs)
	{
		if (classAttrib == className)
		{
			if (idExclusion == "" || (idExclusion != "" && idAttrib == idExclusion))
			{
				bucket.get()->push_back(weak_from_this());
			}
		}
	}
	return;
}

void Widget::getChild_byType(std::shared_ptr<std::vector<std::weak_ptr<Widget>>> bucket, LTokenType type, std::string idExclusion)
{
	for (std::shared_ptr<Widget> child : childWidgets)
	{
		child.get()->getChild_byType(bucket, type, idExclusion);
	}
	if (this->type == type)
	{
		if (idExclusion == "" || (idExclusion != "" && idAttrib == idExclusion))
		{
			bucket.get()->push_back(weak_from_this());
		}
	}
	return;
}

std::weak_ptr<Widget> Widget::getChild_byID(std::string id)
{
	std::weak_ptr<Widget> selection;
	if (idAttrib == id) { return weak_from_this(); }
	for (std::shared_ptr<Widget> child : childWidgets)
	{
		if (child.get()->idAttrib == id)
		{
			return child;
		}
		selection = child.get()->getChild_byID(id);
		if (!selection.expired()) { return selection; }
	}
	return selection;
}

std::weak_ptr<Widget> Widget::getChild_byName(std::string nameAttrib)
{
	std::weak_ptr<Widget> selection;
	for (std::shared_ptr<Widget> child : childWidgets)
	{
		if (child.get()->nameAttrib == nameAttrib)
		{
			return child;
		}
		selection = child.get()->getChild_byID(nameAttrib);
		if (!selection.expired()) { return selection; }
	}
	return selection;
}

bool Widget::removeChild_byID(std::shared_ptr<CInterpreter> interpreter, std::string childID)
{
	return false;
}

bool Widget::removeChildren_byClass(std::shared_ptr<CInterpreter> interpreter, std::string className)
{
	return false;
}

bool Widget::deleteWidget()
{
	for (std::shared_ptr<Widget> child : childWidgets)
	{
		child.get()->deleteWidget();
	}
	return deleteSelf();
}

bool Widget::deleteSelf()
{
	// Remove listings from UI WidgetIDTable
	if (idAttrib != "" && chromaIO.get()->ui.get()->widgetIDTable.count(idAttrib) == 1)
	{
		chromaIO.get()->ui.get()->widgetIDTable.erase(idAttrib);
	}
	// Empty child widget list
	childWidgets.clear(); childWidgets.shrink_to_fit();
	outsideBoundWidgets.clear();
	// Reset target widgets
	parentWidget.reset();
	overflowTarget.reset();
	// Clear the callbackMap
	for (auto const& item : callbackMap)
	{
		for (std::shared_ptr<CStmt> callFunc : item.second)
		{
			callFunc.reset();
		}
	}
	callbackMap.clear();
	// Reset all pointer and openGL buffers
	clearData(); deleteBuffers();
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Child Location Finding
//
////////////////////////////////////////////////////////////////////////////////////////////////

glm::ivec2 Widget::horizontalArrangement(std::shared_ptr<Widget> childWidget)
{
	// This Widget Properties (Self)
	glm::ivec2 thisSize = glm::ivec2(sizeX, sizeY);
	// Child Widget Properties
	WidgetStyle childStyle = childWidget.get()->style;
	glm::ivec2 childLoc = glm::ivec2(0, 0);
	glm::ivec2 childSize = childWidget.get()->getSize();
	
	// HoirzontalBox will only arrange elements horizontally, even if there is space available at the bottom
	size_t childIndex = 0;
	int nChildren = (int)childWidgets.size() - 1;
	// Get the correct childWidget
	//for (std::shared_ptr<Widget> thisChildren : childWidgets)
	for (int c = 0; c < childWidgets.size(); c++)
	{
		//if (childWidget == thisChildren) { break; }
		if (childWidget == childWidgets[c]) { break; }
		else { childIndex++; }
	}
	// Floating Left
	if ((childIndex == 0 && (childStyle.floating == UI_FLOATING_LEFT || childStyle.floating == UI_FLOATING_NONE))
		|| childStyle.position == UI_POSITION_ABSOLUTE)
	{
		// Assume that both position and margin must be positive values
		//childLoc.x += childStyle.getMargin().left + childStyle.getOffset().left;
		//childLoc.y += childStyle.getMargin().top + childStyle.getOffset().top;
		childLoc.x += childStyle.getMargin().left;
		childLoc.y += childStyle.getMargin().top;
		if (childStyle.position == UI_POSITION_ABSOLUTE)
		{
			// Check the raw offset value in order to find whether to use right or left
			// Use left precedence, unless left value is none, right will not be used
			if (childStyle.offset.left != UI_WVALUE_NONE)
			{
				childLoc.x += childStyle.getOffset().left;
			}
			else if (childStyle.offset.right != UI_WVALUE_NONE)
			{
				childLoc.x = thisSize.x -
					(childSize.x + 
						((childStyle.getMargin().right > childStyle.getOffset().right) ? 
						childStyle.getMargin().right : childStyle.getOffset().right));
			}
			if (childStyle.offset.top != UI_WVALUE_NONE)
			{
				childLoc.y += childStyle.getOffset().top;
			}
			else if (childStyle.offset.bottom != UI_WVALUE_NONE)
			{
				childLoc.y = thisSize.y -
					(childSize.y +
						((childStyle.getMargin().bottom > childStyle.getOffset().bottom) ?
						childStyle.getMargin().bottom : childStyle.getOffset().bottom));
			}
			//childLoc.x += childStyle.getOffset().left - childStyle.getOffset().right;
			//childLoc.y += childStyle.getOffset().top - childStyle.getOffset().bottom;
		}
		return childLoc;
	}
	if (childIndex > 0 && (childStyle.floating == UI_FLOATING_LEFT || childStyle.floating == UI_FLOATING_NONE))
	{
		//childLoc.x += childStyle.getMargin().left + childStyle.getOffset().left;
		//childLoc.y += childStyle.getMargin().top + childStyle.getOffset().top;
		childLoc.x += childStyle.getMargin().left;
		childLoc.y += childStyle.getMargin().top;
		// Check if position is relative for whether or not to apply left/top positioning offsets
		if (childStyle.position == UI_POSITION_RELATIVE)
		{
			childLoc.x += childStyle.getOffset().left - childStyle.getOffset().right;
			childLoc.y += childStyle.getOffset().top - childStyle.getOffset().bottom;
		}

		int previousValidChild = 0;
		bool foundValid = false; int z = 1;
		while (!foundValid && ((int)childIndex - z) >= 0)
		{
			if (childWidgets[childIndex - z].get()->checkVisibility() == true &&
				childWidgets[childIndex - z].get()->style.position != UI_POSITION_ABSOLUTE)
			{
				foundValid = true; previousValidChild = z; break;
			}
			else { z += 1; }
		}

		WidgetStyle leftStyle = childWidgets[childIndex - previousValidChild].get()->style;
		glm::ivec2 leftSize = childWidgets[childIndex - previousValidChild].get()->getSize();
		glm::ivec2 leftLoc = childWidgets[childIndex - previousValidChild].get()->getLocation();

		int k = leftLoc.x + leftSize.x + leftStyle.getBorder().right + childStyle.getBorder().left;
		childLoc.x = (childLoc.x > k) ? childLoc.x : k;
		return childLoc;
	}
	// Floating Right
	if (childStyle.floating == UI_FLOATING_RIGHT)
	{
		// If childWidget is floating right and also the last element in the array
		if (childIndex == nChildren)
		{
			int a = thisSize.x - (childSize.x + childStyle.getMargin().right);
			//int b = childStyle.getMargin().left + childStyle.getOffset().left;
			int b = 0;
			if (childStyle.position == UI_POSITION_RELATIVE)
			{
				b = childStyle.getMargin().left + (childStyle.getOffset().left - childStyle.getOffset().right);
				childLoc.x += (a > b) ? a : b;
				childLoc.y += childStyle.getMargin().top + (childStyle.getOffset().top - childStyle.getOffset().bottom);
			}
			else
			{
				b = childStyle.getMargin().left;
				childLoc.x += (a > b) ? a : b;
				childLoc.y += childStyle.getMargin().top;
			}
			//childLoc.x += (a > b) ? a : b;
			//childLoc.y += childStyle.getMargin().top + childStyle.getOffset().top;
			return childLoc;
		}
		// If childWidget is floating right but not the last in the array
		// Must get the total of every widget rightward, for now assume every rightward widget has float:right as well
		// Evaluation order comes in left to right, so rightward positions are not yet set. 
		if (childIndex < nChildren)
		{
			int nChildrenInFlow = nChildren;
			int rightLocSum = 0;
			for (size_t k = nChildren; k > childIndex; k--)
			{
				// If the widget is outside normal flow, do nothing
				if (childWidgets[k].get()->style.position == UI_POSITION_ABSOLUTE
					|| childWidgets[k].get()->style.position == UI_POSITION_FIXED)
				{
					nChildrenInFlow--;
					// If the widget would've now been the last in the array as normal flow, return early
					if (nChildrenInFlow == childIndex)
					{
						int a = thisSize.x - (childSize.x + childStyle.getMargin().right);
						//int b = childStyle.getMargin().left + childStyle.getOffset().left;
						int b = 0;
						if (childStyle.position == UI_POSITION_RELATIVE)
						{
							b = childStyle.getMargin().left + (childStyle.getOffset().left - childStyle.getOffset().right);
							childLoc.x += (a > b) ? a : b;
							childLoc.y += childStyle.getMargin().top + (childStyle.getOffset().top - childStyle.getOffset().bottom);
						}
						else
						{
							b = childStyle.getMargin().left;
							childLoc.x += (a > b) ? a : b;
							childLoc.y += childStyle.getMargin().top;
						}
						//childLoc.x += (a > b) ? a : b;
						//childLoc.y += childStyle.getMargin().top + childStyle.getOffset().top;
						return childLoc;
					}
					else
					{
						continue;
					}
				}
				if (k == nChildrenInFlow)
				{
					rightLocSum += thisSize.x;
					rightLocSum -= childWidgets[k].get()->getSize().x;
					rightLocSum -= childWidgets[k].get()->style.getBorder().left;
					rightLocSum -= childWidgets[k].get()->style.getMargin().right;
				}
				else if (k + 1 <= nChildrenInFlow) // Sanity check to avoid read-access violation
				{
					rightLocSum -= childWidgets[k].get()->getSize().x;
					rightLocSum -= childWidgets[k + 1].get()->style.getBorder().left;
					rightLocSum -= childWidgets[k].get()->style.getBorder().right;
				}
			}
			rightLocSum -= childSize.x + childStyle.getBorder().right;
			childLoc.x = rightLocSum;
			if (childStyle.position == UI_POSITION_RELATIVE)
			{
				childLoc.y += childStyle.getMargin().top + (childStyle.getOffset().top - childStyle.getOffset().bottom);
			}
			else
			{
				childLoc.y += childStyle.getMargin().top;
			}
			//childLoc.y += childStyle.getMargin().top + childStyle.getOffset().top;
			return childLoc;
		}
	}
	// Floating Center, like left & right, only applies to the horizontal alignment of a widget
	if (childStyle.floating == UI_FLOATING_CENTER)
	{
		// If no the childWidget has no siblings or is the first (F.C.F.S), then calculation is straight forward
		if (childIndex == nChildren || childIndex == 0)
		{
			childLoc.x = (thisSize.x - (childSize.x + childStyle.getMargin().left + childStyle.getMargin().right)) / 2;
			// Because location will otherwise be for virtual margin position, not content position
			childLoc.x += childStyle.getMargin().left;
			// Center first, then add the px offset, regardless of the margin size
			if (childStyle.position == UI_POSITION_RELATIVE)
			{
				childLoc.x += (childStyle.getOffset().left - childStyle.getOffset().right);
				childLoc.y += childStyle.getMargin().top + (childStyle.getOffset().top - childStyle.getOffset().bottom);
			}
			else
			{
				childLoc.y += childStyle.getMargin().top;
			}
			//childLoc.x += childStyle.getOffset().left;
			//childLoc.y += childStyle.getMargin().top + childStyle.getOffset().top;
			return childLoc;
		}
		// If the childWidget has siblings and is not the first, then find the left-most position closest to center
		// Widget locations are updated from begin-end, so the left-child should have correct data
		if (childIndex < nChildren)
		{
			int previousValidChild = 0;
			bool foundValid = false; int z = 1;
			while (!foundValid && ((int)childIndex - z) >= 0)
			{
				if (childWidgets[childIndex - z].get()->checkVisibility() == true &&
					childWidgets[childIndex - z].get()->style.position != UI_POSITION_ABSOLUTE)
				{
					foundValid = true; previousValidChild = z; break;
				}
				else { z += 1; }
			}

			WidgetStyle leftStyle = childWidgets[childIndex - previousValidChild].get()->style;
			glm::ivec2 leftSize = childWidgets[childIndex - previousValidChild].get()->getSize();
			glm::ivec2 leftLoc = childWidgets[childIndex - previousValidChild].get()->getLocation();

			int childLocWantX = (thisSize.x - (childSize.x + childStyle.getMargin().left + childStyle.getMargin().right)) / 2;
			// Because location will otherwise be for virtual margin position, not content position
			childLocWantX += childStyle.getMargin().left;
			// Center first, then add the px offset, regardless of the margin size
			if (childStyle.position == UI_POSITION_RELATIVE)
			{
				childLocWantX += (childStyle.getOffset().left - childStyle.getOffset().right);
				childLoc.y += childStyle.getMargin().top + (childStyle.getOffset().top - childStyle.getOffset().bottom);
			}
			//childLocWantX += childStyle.getOffset().left;
			int leftMostLocX = leftLoc.x + leftSize.x + leftStyle.getBorder().right + childStyle.getBorder().left;
			childLoc.x = (childLocWantX > leftMostLocX) ? childLocWantX : leftMostLocX;
			//childLoc.y += childStyle.getMargin().top + childStyle.getOffset().top;
			return childLoc;
		}
	}
	return childLoc;
}

glm::ivec2 Widget::verticalArrangement(std::shared_ptr<Widget> childWidget)
{
	// This Widget Properties (Self)
	glm::ivec2 thisSize = glm::ivec2(sizeX, sizeY);
	// Child Widget Properties
	glm::ivec2 childLoc = glm::ivec2(0, 0);
	glm::ivec2 childSize = childWidget.get()->getSize();
	WidgetStyle childStyle = childWidget.get()->style;
	// HoirzontalBox will only arrange elements horizontally, even if there is space available at the bottom
	size_t childIndex = 0;
	int nChildren = (int)childWidgets.size() - 1;
	// Get the correct childWidget
	//for (std::shared_ptr<Widget> thisChildren : childWidgets)
	for (int c = 0; c < childWidgets.size(); c++)
	{
		//if (childWidget == thisChildren) { break; }
		if (childWidget == childWidgets[c]) { break; }
		else { childIndex++; }
	}
	// Floating Left
	if ((childIndex == 0 && (childStyle.floating == UI_FLOATING_LEFT || childStyle.floating == UI_FLOATING_NONE))
		|| childStyle.position == UI_POSITION_ABSOLUTE)
	{
		// Assume that both position and margin must be positive values
		childLoc.x += childStyle.getMargin().left;
		childLoc.y += childStyle.getMargin().top;
		if (childStyle.position == UI_POSITION_ABSOLUTE)
		{
			if (childStyle.offset.left != UI_WVALUE_NONE)
			{
				childLoc.x += childStyle.getOffset().left;
			}
			else if (childStyle.offset.right != UI_WVALUE_NONE)
			{
				childLoc.x = thisSize.x -
					(childSize.x +
						((childStyle.getMargin().right > childStyle.getOffset().right) ?
						childStyle.getMargin().right : childStyle.getOffset().right));
			}
			if (childStyle.offset.top != UI_WVALUE_NONE)
			{
				childLoc.y += childStyle.getOffset().top;
			}
			else if (childStyle.offset.bottom != UI_WVALUE_NONE)
			{
				childLoc.y = thisSize.y -
					(childSize.y +
						((childStyle.getMargin().bottom > childStyle.getOffset().bottom) ?
						childStyle.getMargin().bottom : childStyle.getOffset().bottom));
			}
		}
		return childLoc;
	}
	if (childIndex > 0 && (childStyle.floating == UI_FLOATING_LEFT || childStyle.floating == UI_FLOATING_NONE))
	{
		//childLoc.x += childStyle.getMargin().left + childStyle.getOffset().left;
		childLoc.x += childStyle.getMargin().left;
		if (childStyle.position == UI_POSITION_RELATIVE)
		{
			childLoc.x += childStyle.getOffset().left - childStyle.getOffset().right;
		}
		
		int previousValidChild = 0;
		bool foundValid = false; int z = 1;
		while (!foundValid && ((int)childIndex - z) >= 0)
		{
			if (childWidgets[childIndex - z].get()->checkVisibility() == true &&
				childWidgets[childIndex - z].get()->style.position != UI_POSITION_ABSOLUTE)
			{
				foundValid = true; previousValidChild = z; break;
			}
			else { z += 1; }
		}
		
		childLoc.y += childStyle.getMargin().top;
		int k = 0;
		if (childIndex - previousValidChild != childIndex && 
			childWidgets[childIndex - previousValidChild].get()->style.position != UI_POSITION_ABSOLUTE)
		{
			WidgetStyle topStyle = childWidgets[childIndex - previousValidChild].get()->style;
			glm::ivec2 topSize = childWidgets[childIndex - previousValidChild].get()->getSize();
			glm::ivec2 topLoc = childWidgets[childIndex - previousValidChild].get()->getLocation();
			k = topLoc.y + topSize.y + topStyle.getBorder().bottom + childStyle.getBorder().top;
		}
		childLoc.y = (childLoc.y > k) ? childLoc.y : k;
		//childLoc.y += childStyle.getOffset().top;
		if (childStyle.position == UI_POSITION_RELATIVE)
		{
			childLoc.x += childStyle.getOffset().top - childStyle.getOffset().bottom;
		}
		return childLoc;
	}
	// Floating Right
	if (childStyle.floating == UI_FLOATING_RIGHT)
	{
		// If childWidget is floating right and also the last element in the array
		if (childIndex == nChildren)
		{
			int a = thisSize.y - (childSize.y + childStyle.getMargin().bottom);
			//int b = childStyle.getMargin().top + childStyle.getOffset().top;
			int b = 0;
			if (childStyle.position == UI_POSITION_RELATIVE)
			{
				b = childStyle.getMargin().top + (childStyle.getOffset().top - childStyle.getOffset().bottom);
				childLoc.x += childStyle.getMargin().left + (childStyle.getOffset().left - childStyle.getOffset().right);
				childLoc.y += (a > b) ? a : b;
			}
			else
			{
				b = childStyle.getMargin().top;
				childLoc.x += childStyle.getMargin().left + (childStyle.getOffset().left - childStyle.getOffset().right);
				childLoc.y += (a > b) ? a : b;
			}
			return childLoc;
		}
		// If childWidget is floating right but not the last in the array
		// Must get the total of every widget rightward, for now assume every rightward widget has float:right as well
		// Evaluation order comes in left to right, so rightward positions are not yet set. 
		if (childIndex < nChildren)
		{
			int nChildrenInFlow = nChildren;
			int bottomLocSum = 0;

			for (size_t k = nChildren; k > childIndex; k--)
			{
				if (childWidgets[k].get()->style.position == UI_POSITION_ABSOLUTE
					|| childWidgets[k].get()->style.position == UI_POSITION_FIXED)
				{
					// If the widget is outside normal flow, do nothing
					nChildrenInFlow--;
					// If the widget would've now been the last in the array as normal flow, return early
					if (nChildrenInFlow == childIndex)
					{
						int a = thisSize.y - (childSize.y + childStyle.getMargin().bottom);
						int b = 0;
						if (childStyle.position == UI_POSITION_RELATIVE)
						{
							b = childStyle.getMargin().top + (childStyle.getOffset().top - childStyle.getOffset().bottom);
							childLoc.x += childStyle.getMargin().left + (childStyle.getOffset().left - childStyle.getOffset().right);
							childLoc.y += (a > b) ? a : b;
						}
						else
						{
							b = childStyle.getMargin().top;
							childLoc.x += childStyle.getMargin().left + (childStyle.getOffset().left - childStyle.getOffset().right);
							childLoc.y += (a > b) ? a : b;
						}
						return childLoc;
					}
					else
					{
						continue;
					}
				}
				if (k == nChildrenInFlow)
				{
					bottomLocSum += thisSize.y;
					bottomLocSum -= childWidgets[k].get()->getSize().y;
					bottomLocSum -= childWidgets[k].get()->style.getBorder().top;
					bottomLocSum -= childWidgets[k].get()->style.getMargin().bottom;
				}
				else if (k + 1 <= nChildrenInFlow) // Sanity check to avoid read-access violation
				{
					bottomLocSum -= childWidgets[k].get()->getSize().y;
					bottomLocSum -= childWidgets[k + 1].get()->style.getBorder().top;
					bottomLocSum -= childWidgets[k].get()->style.getBorder().bottom;
				}
			}
			bottomLocSum -= childSize.y + childStyle.getBorder().bottom;
			if (childStyle.position == UI_POSITION_RELATIVE)
			{
				childLoc.x = childStyle.getMargin().right + (childStyle.getOffset().left - childStyle.getOffset().right);
			}
			else
			{
				childLoc.x = childStyle.getMargin().right;
			}
			//childLoc.x = childStyle.getMargin().right + childStyle.getOffset().right;
			childLoc.y += bottomLocSum;
			return childLoc;
		}
	}
	// Floating Center, like left & right, only applies to the horizontal alignment of a widget
	if (childStyle.floating == UI_FLOATING_CENTER)
	{
		// If no the childWidget has no siblings or is the first (F.C.F.S), then calculation is straight forward
		if (childIndex == nChildren || childIndex == 0)
		{
			//childLoc.y = (thisSize.y - (childSize.y + childStyle.getMargin().top + childStyle.getMargin().bottom)) / 2;
			// Because location will otherwise be for virtual margin position, not content position
			childLoc.y += childStyle.getMargin().top;
			// Center first, then add the px offset, regardless of the margin size
			if (childStyle.position == UI_POSITION_RELATIVE)
			{
				childLoc.x += childStyle.getMargin().left + (childStyle.getOffset().left - childStyle.getOffset().right);
				childLoc.y += childStyle.getOffset().top;
			}
			else
			{
				childLoc.x += childStyle.getMargin().left;
			}
			return childLoc;
		}
		// If the childWidget has siblings and is not the first, then find the left-most position closest to center
		// Widget locations are updated from begin-end, so the left-child should have correct data
		if (childIndex < nChildren)
		{
			int previousValidChild = 0;
			bool foundValid = false; int z = 1;
			while (!foundValid && ((int)childIndex - z) >= 0)
			{
				if (childWidgets[childIndex - z].get()->checkVisibility() == true &&
					childWidgets[childIndex - z].get()->style.position != UI_POSITION_ABSOLUTE)
				{
					foundValid = true; previousValidChild = z; break;
				}
				else { z += 1; }
			}

			WidgetStyle topStyle = childWidgets[childIndex - previousValidChild].get()->style;
			glm::ivec2 topSize = childWidgets[childIndex - previousValidChild].get()->getSize();
			glm::ivec2 topLoc = childWidgets[childIndex - previousValidChild].get()->getLocation();

			int childLocWantY = (thisSize.y - (childSize.y + childStyle.getMargin().top + childStyle.getMargin().bottom)) / 2;
			// Because location will otherwise be for virtual margin position, not content position
			childLocWantY += childStyle.getMargin().top;
			// Center first, then add the px offset, regardless of the margin size
			if (childStyle.position == UI_POSITION_RELATIVE)
			{
				childLocWantY += childStyle.getOffset().top - childStyle.getOffset().bottom;
				childLoc.x = childStyle.getMargin().left + (childStyle.getOffset().left - childStyle.getOffset().right);
			}
			else
			{
				childLoc.x = childStyle.getMargin().left;
			}
			int topMostLocY = topLoc.y + topSize.y + topStyle.getBorder().bottom + childStyle.getBorder().top;
			childLoc.y += (childLocWantY > topMostLocY) ? childLocWantY : topMostLocY;
			return childLoc;
		}
	}
	return childLoc;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Property Get/Set Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
bool Widget::isInputType(LTokenType type)
{
	switch (type)
	{
	case LTokenType::LINE:
	case LTokenType::PARAGRAPH:
		return true;
	}
	return false;
}

bool Widget::updateChildOrder(std::string idName, int moveNumber, bool ignoreHidden, bool asLiteral, bool setVisible)
{
	std::weak_ptr<Widget> child = getChild_byID(idName);
	if (!child.expired())
	{
		if (asLiteral)
		{
			if (moveNumber >= childWidgets.size()) { return false; }
			if (moveNumber < 0)
			{
				int i = 0;
			}
			else
			{
				int pos = 0;
				std::vector<std::shared_ptr<Widget>>::iterator it = childWidgets.begin();
				for (it; it != childWidgets.end(); ++it)
				{
					if (*it == child.lock()) { break; }
					pos++;
				}
				int newPos = moveNumber;
				std::shared_ptr<Widget> swapTar = childWidgets[newPos];
				childWidgets[newPos] = childWidgets[pos];
				childWidgets[pos] = swapTar;
			}
		}
		else
		{
			int pos = 0;
			std::vector<std::shared_ptr<Widget>>::iterator it = childWidgets.begin();
			for (it; it != childWidgets.end(); ++it)
			{
				if (*it == child.lock()) { break; }
				pos++;
			}
			int newPos = pos + moveNumber;
			if (newPos >= childWidgets.size() || newPos < 0) { return false; }
			if (ignoreHidden && childWidgets[newPos].get()->style.visibility <= 0) { return false; }
			std::shared_ptr<Widget> swapTar = childWidgets[newPos];
			childWidgets[newPos] = childWidgets[pos];
			childWidgets[pos] = swapTar;
		}
		if (setVisible)
		{
			child.lock()->setProperty(
				chromaIO.get()->scriptConsole.get()->getInterpreter(), 
				"visibility", 
				std::make_shared<CObject>("visible"));
		}

		std::weak_ptr<Widget> rebuildTarget = weak_from_this();
		int rebuildCount = 0;
		while (!rebuildTarget.expired() && !rebuildTarget.lock()->parentWidget.expired() && rebuildCount <= 2)
		{
			rebuildTarget = rebuildTarget.lock()->parentWidget; rebuildCount++;
		}
		chromaIO.get()->ui.get()->requestWidgetHierarchyRebuild(rebuildTarget);

		return true;
	}
	return false;
}

std::weak_ptr<Widget> Widget::addChildWidget(std::shared_ptr<CInterpreter> interpreter,
	std::string protoID, std::string childID, std::vector<std::string> extraClasses, int moveNumber)
{
	// Prevent bad lookups where either the prototype ID doesnt exist, or the new ID is overlapping
	// Dynamically created widgets must have a unique ID in order to clean them up easier.
	if (chromaIO.get()->ui.get()->prototypeFactoryTable.count(protoID) == 0) { return std::weak_ptr<Widget>(); }
	if (chromaIO.get()->ui.get()->widgetIDTable.count(childID) == 1) { return std::weak_ptr<Widget>(); }
	
	std::vector<std::shared_ptr<Widget>>::iterator it;
	
	if (moveNumber == 0)
	{
		it = childWidgets.begin();
		childWidgets.insert(it, 
			chromaIO.get()->ui.get()->createWidget_fromPrototype(shared_from_this(), protoID, childID, extraClasses));
		if (childWidgets.back() == nullptr) { childWidgets.pop_back(); return std::weak_ptr<Widget>(); } // Remove null creations

		chromaIO.get()->scriptConsole.get()->insertWidgetTableID(childID, this->getRoot().lock()->rootId);
		std::weak_ptr<Widget> rebuildTarget = weak_from_this();
		int rebuildCount = 0;
		while (!rebuildTarget.expired() && !rebuildTarget.lock()->parentWidget.expired() && rebuildCount <= 3)
		{
			rebuildTarget = rebuildTarget.lock()->parentWidget; rebuildCount++;
		}
		chromaIO.get()->ui.get()->requestWidgetHierarchyRebuild(rebuildTarget);

		return childWidgets.front().get()->weak_from_this();
	}
	else if (moveNumber == -1)
	{
		it = childWidgets.end();
		childWidgets.insert(it,
			chromaIO.get()->ui.get()->createWidget_fromPrototype(shared_from_this(), protoID, childID, extraClasses));
		if (childWidgets.back() == nullptr) { childWidgets.pop_back(); return std::weak_ptr<Widget>(); } // Remove null creations

		chromaIO.get()->scriptConsole.get()->insertWidgetTableID(childID, this->getRoot().lock()->rootId);
		std::weak_ptr<Widget> rebuildTarget = weak_from_this();
		int rebuildCount = 0;
		while (!rebuildTarget.expired() && !rebuildTarget.lock()->parentWidget.expired() && rebuildCount <= 3)
		{
			rebuildTarget = rebuildTarget.lock()->parentWidget; rebuildCount++;
		}
		chromaIO.get()->ui.get()->requestWidgetHierarchyRebuild(rebuildTarget);

		return childWidgets.back().get()->weak_from_this();
	}
	return std::weak_ptr<Widget>();
}

// setChildProperty
bool Widget::setChildProperty(int switchType,
	std::shared_ptr<CInterpreter> interpreter,
	std::string id, std::string name,
	std::shared_ptr<CObject> value)
{
	std::weak_ptr<Widget> selection;
	switch (switchType)
	{
	case 0:
		selection = getChild_byID(id);
		if (!selection.expired())
		{
			selection.lock().get()->setProperty(interpreter, name, value);
			return true;
		}
		break;
	case 1:
		selection = getChild_byName(id);
		if (!selection.expired())
		{
			selection.lock().get()->setProperty(interpreter, name, value);
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

// setProperty
bool Widget::setProperty(std::shared_ptr<CInterpreter> interpreter, std::string name, std::shared_ptr<CObject> value)
{
	if (layoutStringTypeMap.count(name))
	{
		switch (layoutStringTypeMap.at(name))
		{
			// Note: Unfinished program area.
		case LTokenType::ID:
			return false; break;
		case LTokenType::CLASS:
			return false; break;
		case LTokenType::NAME:
			return false; break;
		case LTokenType::STYLE:
			return false; break;
		case LTokenType::VALUE:
			return setProperty_value(interpreter, name, value);
		case LTokenType::IMG:
			return false; break;
		case LTokenType::GROUP:
			return false; break;
		case LTokenType::INNER_CONTENT:
			return false; break;
		case LTokenType::ON_BLUR:
		case LTokenType::ON_FOCUS:
		case LTokenType::ON_CLICK:
		case LTokenType::ON_RIGGHTCLICK:
		case LTokenType::ON_DBL_CLICK:
		case LTokenType::ON_DBL_RIGHTCLICK:
		case LTokenType::ON_MIDDLECLICK:
		case LTokenType::ON_DBL_MIDDLECLICK:
		case LTokenType::ON_DRAG:
		case LTokenType::ON_DRAG_START:
		case LTokenType::ON_DRAG_END:
		case LTokenType::ON_DRAG_ENTER:
		case LTokenType::ON_DRAG_OVER:
		case LTokenType::ON_DRAG_LEAVE:
		case LTokenType::ON_DROP:
		case LTokenType::ON_KEY_DOWN:
		case LTokenType::ON_KEY_PRESS:
		case LTokenType::ON_KEY_UP:
		case LTokenType::ON_LOAD:
		case LTokenType::ON_COPY:
		case LTokenType::ON_PASTE:
		case LTokenType::ON_CUT:
		case LTokenType::ON_MOUSE_OVER:
		case LTokenType::ON_MOUSE_ENTER:
		case LTokenType::ON_MOUSE_LEAVE:
		case LTokenType::ON_MOUSE_WHEELUP:
		case LTokenType::ON_MOUSE_WHEELDOWN:
		case LTokenType::ON_RELEASE:
		case LTokenType::ON_RESIZE:
			return false; break;
		case LTokenType::DRAGGABLE:
		case LTokenType::DROPPABLE:
		case LTokenType::DRAG_TYPE:
		case LTokenType::DROP_TYPE:
			return false; break;
		default:
			return false;
		}
	}
	else if (styleTypeMap.count(name) || (name.find("vertex-") != std::string::npos && name.find("-color") != std::string::npos))
	{
		std::shared_ptr<CObject> rebuild = style.getSetProperty(interpreter, name, value, false);
		if (rebuild != nullptr && rebuild.get()->objType.type == CLiteralTypes::_CBool && std::get<bool>(rebuild.get()->obj) == true)
		{
			std::weak_ptr<Widget> rebuildTarget = weak_from_this();
			int rebuildCount = 0;
			while (!rebuildTarget.expired() && !rebuildTarget.lock()->parentWidget.expired() && rebuildCount <= 3)
			{
				rebuildTarget = rebuildTarget.lock()->parentWidget; rebuildCount++;
			}
			chromaIO.get()->ui.get()->requestWidgetHierarchyRebuild(rebuildTarget);
		}
		if (style.isSizeProperty(name) && callbackMap.count("onresize") == 1 && callbackMap.at("onresize").size() != 0)
		{
			chromaIO.get()->getUI().get()->addResizeEvent(weak_from_this());
		}
		return true;
	}
	else if (name.find("zoom") != std::string::npos)
	{
		if (this->type == LTokenType::T_NOISE && value.get()->objType.type == CLiteralTypes::_CNumber) 
		{
			dynamic_cast<Noise_ToolControl*>(this)->updateZoom((float)std::get<double>(value.get()->obj));
		}
	}
	return false;
}

// Special Set Value Property Logic
bool Widget::setProperty_value(std::shared_ptr<CInterpreter> interpreter, std::string name, std::shared_ptr<CObject> value)
{
	std::string string = "";
	if (value == nullptr) { 
		return false; 
	}
	if (value.get()->objType.type == CLiteralTypes::_CString)
	{
		string = std::get<std::string>(value.get()->obj);
	}
	else
	{
		string = chromaIO.get()->scriptConsole.get()->toString(value);
	}
	switch (style.textFormat)
	{
	case UI_TEXTFORMAT_DEFAULT: valueAttrib = string; break;
	case UI_TEXTFORMAT_UPPERCASE: valueAttrib = stringToUpper(string); break;
	case UI_TEXTFORMAT_LOWERCASE: valueAttrib = stringToLower(string); break;
	case UI_TEXTFORMAT_PERCENT_WHOLE: valueAttrib = stringNumberOnly_truncate(string, 0) + "%"; break;
	case UI_TEXTFORMAT_PERCENT_ALL: valueAttrib = stringNumberOnly(string) + "%"; break;
	case UI_TEXTFORMAT_PERCENT_1D: valueAttrib = stringNumberOnly_truncate(string, 1) + "%"; break;
	case UI_TEXTFORMAT_PERCENT_2D: valueAttrib = stringNumberOnly_truncate(string, 2) + "%"; break;
	case UI_TEXTFORMAT_PERCENT_3D: valueAttrib = stringNumberOnly_truncate(string, 3) + "%"; break;
	case UI_TEXTFORMAT_PERCENT_4D: valueAttrib = stringNumberOnly_truncate(string, 4) + "%"; break;
	case UI_TEXTFORMAT_PERCENT_5D: valueAttrib = stringNumberOnly_truncate(string, 5) + "%"; break;
	case UI_TEXTFORMAT_PERCENT_6D: valueAttrib = stringNumberOnly_truncate(string, 6) + "%"; break;
	case UI_TEXTFORMAT_NUMBER_WHOLE: valueAttrib = stringNumberOnly_truncate(string, 0); break;
	case UI_TEXTFORMAT_NUMBER_ALL: valueAttrib = stringNumberOnly(string); break;
	case UI_TEXTFORMAT_NUMBER_1D: valueAttrib = stringNumberOnly_truncate(string, 1); break;
	case UI_TEXTFORMAT_NUMBER_2D: valueAttrib = stringNumberOnly_truncate(string, 2); break;
	case UI_TEXTFORMAT_NUMBER_3D: valueAttrib = stringNumberOnly_truncate(string, 3); break;
	case UI_TEXTFORMAT_NUMBER_4D: valueAttrib = stringNumberOnly_truncate(string, 4); break;
	case UI_TEXTFORMAT_NUMBER_5D: valueAttrib = stringNumberOnly_truncate(string, 5); break;
	case UI_TEXTFORMAT_NUMBER_6D: valueAttrib = stringNumberOnly_truncate(string, 6); break;
	case UI_TEXTFORMAT_DEGREE_WHOLE: valueAttrib = stringNumberOnly_truncate(string, 0) + "a"; break; // "�" Not available currently.
	case UI_TEXTFORMAT_DEGREE_ALL: valueAttrib = stringNumberOnly(string) + "a"; break;
	case UI_TEXTFORMAT_DEGREE_1D: valueAttrib = stringNumberOnly_truncate(string, 1) + "a"; break;
	case UI_TEXTFORMAT_DEGREE_2D: valueAttrib = stringNumberOnly_truncate(string, 2) + "a"; break;
	case UI_TEXTFORMAT_DEGREE_3D: valueAttrib = stringNumberOnly_truncate(string, 3) + "a"; break;
	case UI_TEXTFORMAT_DEGREE_4D: valueAttrib = stringNumberOnly_truncate(string, 4) + "a"; break;
	case UI_TEXTFORMAT_DEGREE_5D: valueAttrib = stringNumberOnly_truncate(string, 5) + "a"; break;
	case UI_TEXTFORMAT_DEGREE_6D: valueAttrib = stringNumberOnly_truncate(string, 6) + "a"; break;
	}
	return true;
}


// setProperty
bool Widget::resetProperty(std::shared_ptr<CInterpreter> interpreter, std::string name)
{
	if (layoutStringTypeMap.count(name))
	{
		switch (layoutStringTypeMap.at(name))
		{
			// Note: Unfinished program area.
		case LTokenType::ID:
			return false; break;
		case LTokenType::CLASS:
			return false; break;
		case LTokenType::NAME:
			return false; break;
		case LTokenType::STYLE:
			return false; break;
		case LTokenType::VALUE:
			return false; break;
		case LTokenType::IMG:
			return false; break;
		case LTokenType::GROUP:
			return false; break;
		case LTokenType::INNER_CONTENT:
			return false; break;
		case LTokenType::ON_BLUR:
		case LTokenType::ON_FOCUS:
		case LTokenType::ON_CLICK:
		case LTokenType::ON_RIGGHTCLICK:
		case LTokenType::ON_DBL_CLICK:
		case LTokenType::ON_DBL_RIGHTCLICK:
		case LTokenType::ON_MIDDLECLICK:
		case LTokenType::ON_DBL_MIDDLECLICK:
		case LTokenType::ON_DRAG:
		case LTokenType::ON_DRAG_START:
		case LTokenType::ON_DRAG_END:
		case LTokenType::ON_DRAG_ENTER:
		case LTokenType::ON_DRAG_OVER:
		case LTokenType::ON_DRAG_LEAVE:
		case LTokenType::ON_DROP:
		case LTokenType::ON_KEY_DOWN:
		case LTokenType::ON_KEY_PRESS:
		case LTokenType::ON_KEY_UP:
		case LTokenType::ON_LOAD:
		case LTokenType::ON_COPY:
		case LTokenType::ON_PASTE:
		case LTokenType::ON_CUT:
		case LTokenType::ON_MOUSE_OVER:
		case LTokenType::ON_MOUSE_ENTER:
		case LTokenType::ON_MOUSE_LEAVE:
		case LTokenType::ON_MOUSE_WHEELUP:
		case LTokenType::ON_MOUSE_WHEELDOWN:
		case LTokenType::ON_RELEASE:
		case LTokenType::ON_RESIZE:
			return false; break;
		case LTokenType::DRAGGABLE:
		case LTokenType::DROPPABLE:
		case LTokenType::DRAG_TYPE:
		case LTokenType::DROP_TYPE:
			return false; break;
		default:
			return false;
		}
	}
	else if (styleTypeMap.count(name))
	{
		style.resetProperty(interpreter, name, savedStyle);
		return true;
	}
	else
	{
		return false;
	}
}

// getProperty
std::shared_ptr<CObject> Widget::getProperty(std::shared_ptr<CInterpreter> interpreter, std::string name)
{
	if (layoutStringTypeMap.count(name))
	{
		switch (layoutStringTypeMap.at(name))
		{
			// Note: Unfinished program area.
		case LTokenType::ID:
			return std::make_shared<CObject>(idAttrib);
		case LTokenType::CLASS:
			return std::make_shared<CObject>(classAttribs);
		case LTokenType::NAME:
			return std::make_shared<CObject>(nameAttrib);
		case LTokenType::STYLE:
			return std::make_shared<CObject>(std::make_shared<WidgetStyle>(style));
		case LTokenType::VALUE:
			return std::make_shared<CObject>(valueAttrib);
		case LTokenType::IMG:
			return std::make_shared<CObject>(imgAttrib.string());
		case LTokenType::GROUP:
			return std::make_shared<CObject>(groupsAttribs);
		case LTokenType::INNER_CONTENT:
			return std::make_shared<CObject>(innerContent);
		case LTokenType::ON_BLUR:
		case LTokenType::ON_FOCUS:
		case LTokenType::ON_CLICK:
		case LTokenType::ON_RIGGHTCLICK:
		case LTokenType::ON_DBL_CLICK:
		case LTokenType::ON_DBL_RIGHTCLICK:
		case LTokenType::ON_MIDDLECLICK:
		case LTokenType::ON_DBL_MIDDLECLICK:
		case LTokenType::ON_DRAG:
		case LTokenType::ON_DRAG_START:
		case LTokenType::ON_DRAG_END:
		case LTokenType::ON_DRAG_ENTER:
		case LTokenType::ON_DRAG_OVER:
		case LTokenType::ON_DRAG_LEAVE:
		case LTokenType::ON_DROP:
		case LTokenType::ON_KEY_DOWN:
		case LTokenType::ON_KEY_PRESS:
		case LTokenType::ON_KEY_UP:
		case LTokenType::ON_LOAD:
		case LTokenType::ON_COPY:
		case LTokenType::ON_PASTE:
		case LTokenType::ON_CUT:
		case LTokenType::ON_MOUSE_OVER:
		case LTokenType::ON_MOUSE_ENTER:
		case LTokenType::ON_MOUSE_LEAVE:
		case LTokenType::ON_MOUSE_WHEELUP:
		case LTokenType::ON_MOUSE_WHEELDOWN:
		case LTokenType::ON_RELEASE:
		case LTokenType::ON_RESIZE:
			return std::make_shared<CObject>(nullptr);
		case LTokenType::DRAGGABLE:
		case LTokenType::DROPPABLE:
		case LTokenType::DRAG_TYPE:
		case LTokenType::DROP_TYPE:
			return std::make_shared<CObject>(nullptr);
		default:
			return std::make_shared<CObject>(nullptr);
		}
	}
	else if (styleTypeMap.count(name))
	{
		return style.getSetProperty(interpreter, name, nullptr, true);
	}
	else if (name.find("size") != std::string::npos)
	{
		if (name == "size")
		{
			std::vector<std::shared_ptr<CObject>> sizeOut = { std::make_shared<CObject>((double)sizeX), std::make_shared<CObject>((double)sizeY) };
			return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(sizeOut));
		}
		else if (name == "size-X")
		{
			return std::make_shared<CObject>((double)sizeX);
		}
		else if (name == "size-Y")
		{
			return std::make_shared<CObject>((double)sizeY);
		}
	}
	else if (name == "location")
	{
		std::vector<std::shared_ptr<CObject>> posOut = { std::make_shared<CObject>((double)location.x), std::make_shared<CObject>((double)location.y) };
		return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array,
			std::make_shared<std::vector<std::shared_ptr<CObject>>>(posOut));
	}
	else if (name.find("zoom") != std::string::npos)
	{
		if (this->type == LTokenType::T_NOISE)
		{
			return std::make_shared<CObject>(double(dynamic_cast<Noise_ToolControl*>(this)->zoom));
		}
	}
	else 
	{ 
		return std::make_shared<CObject>(nullptr); 
	}
	return std::make_shared<CObject>(nullptr);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Render Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Render Functions
void Widget::draw(ShaderTransform* xform)
{
	if (childWidgets.size() == 0 && style.visibility == UI_VISIBILITY_SHOW)
	{
		if (style.zIndex == 0) { drawSelf(xform); }
		else { chromaIO.get()->ui.get()->addZIndexEntry(this, style.zIndex); }
	}
	else
	{
		if (style.visibility == UI_VISIBILITY_SHOW)
		{
			wasVisible = true;
			if (style.zIndex == 0) { drawSelf(xform); }
			else { chromaIO.get()->ui.get()->addZIndexEntry(this, style.zIndex); }
			for (std::shared_ptr<Widget> child : childWidgets)
			{
				child.get()->draw(xform);
			}
		}
		else if (wasVisible)
		{
			notifyVisibilityChanged();
			for (std::shared_ptr<Widget> child : childWidgets)
			{
				child.get()->notifyVisibilityChanged();
			}
			wasVisible = false;
		}
	}
}

void Widget::notifyVisibilityChanged()
{
	UI* ui = &*chromaIO.get()->getUI();
	if (!ui->activeInputWidget.expired() && 
		ui->activeInputWidget.lock().get() == this)
	{
		ui->putActiveInputWidget(ui->activeInputWidget, true, false, UI_WEVENT_CANCEL);
	}
	for (std::shared_ptr<Widget> child : childWidgets)
	{
		child.get()->notifyVisibilityChanged();
	}
}