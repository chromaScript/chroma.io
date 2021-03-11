#include "../include/entities/WidgetStyle.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CObject.h"
#include "../include/cscript/CCallable.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CStmt.h"
#include "../include/cstyle/SEnums.h"
#include "../include/cstyle/SToken.h"
#include "../include/Color.h"

#include "../include/structs.h"

#include <variant>
#include <memory>
//
WidgetStyle::WidgetStyle()
{
	this->name = "unassociated_empty";
	this->minX = float(UI_WVALUE_NONE);
	this->relMinX = false;
	this->maxX = float(UI_WVALUE_NONE);
	this->relMaxX = false;
	this->minY = float(UI_WVALUE_NONE);
	this->relMinY = false;
	this->maxY = float(UI_WVALUE_NONE);
	this->relMaxY = false;
	this->innerRadius = UI_WVALUE_NONE;
	this->outerRadius = UI_WVALUE_NONE;
	this->position = UI_POSITION_STATIC;
	this->offset.left = UI_WVALUE_NONE;
	this->offset.right = UI_WVALUE_NONE;
	this->offset.top = UI_WVALUE_NONE;
	this->offset.bottom = UI_WVALUE_NONE;
	this->border.top = UI_WVALUE_NONE;
	this->border.bottom = UI_WVALUE_NONE;
	this->border.left = UI_WVALUE_NONE;
	this->border.right = UI_WVALUE_NONE;
	this->margin.top = UI_WVALUE_NONE;
	this->margin.bottom = UI_WVALUE_NONE;
	this->margin.left = UI_WVALUE_NONE;
	this->margin.right = UI_WVALUE_NONE;
	this->anchoring = UI_ANCHORING_NONE;
	this->floating = UI_FLOATING_LEFT;
	this->fillColor = CColor(-1.0f);
	this->fillAlpha = -1.0f;
	this->focusColor = CColor(-1.0f);
	this->highlightColor = CColor(-1.0f);
	this->backgroundColor = CColor(-1.0f);
	this->backgroundAlpha = -1.0f;
	this->boxShadowColor = CColor(-1.0f);
	this->boxShadowAlpha = -1.0f;
	this->boxShadowSizeX = 0;
	this->boxShadowSizeY = 0;
	this->backgroundTexture = "";
	this->overrideClipping = true;
	this->visibility = UI_VISIBILITY_SHOW;
	this->zIndex = 0;
	this->overflowX = UI_OVERFLOW_NONE;
	this->overflowY = UI_OVERFLOW_NONE;
	this->fontPath = "";
	this->fontSize = 0;
}

std::shared_ptr<WidgetStyle> WidgetStyle::makeCopy()
{
	std::shared_ptr<WidgetStyle> obj = std::make_shared<WidgetStyle>();
	*obj = *this;
	return obj;
}

//
COffset WidgetStyle::getOffset()
{
	// Note: Added rules brokering to the struct here to make some of the widget logic
	// more compact. Revisit this choice later. Consider adding more rules brokering
	// functions to the WidgetStyle struct
	COffset out = this->offset;
	if (out.left == UI_WVALUE_NONE || position == UI_POSITION_STATIC) { out.left = 0; }
	if (out.right == UI_WVALUE_NONE || position == UI_POSITION_STATIC) { out.right = 0; }
	if (out.top == UI_WVALUE_NONE || position == UI_POSITION_STATIC) { out.top = 0; }
	if (out.bottom == UI_WVALUE_NONE || position == UI_POSITION_STATIC) { out.bottom = 0; }
	return out;
}

//
CMargin WidgetStyle::getMargin()
{
	CMargin out = this->margin;
	if (out.top == UI_WVALUE_NONE) { out.top = 0; }
	if (out.bottom == UI_WVALUE_NONE) { out.bottom = 0; }
	if (out.left == UI_WVALUE_NONE) { out.left = 0; }
	if (out.right == UI_WVALUE_NONE) { out.right = 0; }
	return out;
}

//
CBorder WidgetStyle::getBorder()
{
	CBorder out = this->border;
	if (out.top == UI_WVALUE_NONE) { out.top = 0; }
	if (out.bottom == UI_WVALUE_NONE) { out.bottom = 0; }
	if (out.left == UI_WVALUE_NONE) { out.left = 0; }
	if (out.right == UI_WVALUE_NONE) { out.right = 0; }
	return out;
}

bool WidgetStyle::isSizeProperty(std::string name)
{
	SToken type = stringToPropertyType(name);
	switch (type.type)
	{
	case STokenType::BOUNDS:
	case STokenType::WIDTH:
	case STokenType::MIN_WIDTH:
	case STokenType::MAX_WIDTH:
	case STokenType::HEIGHT:
	case STokenType::MIN_HEIGHT:
	case STokenType::MAX_HEIGHT:
	case STokenType::DIMENSIONS:
		return true;
	}
	return false;
}

void WidgetStyle::resetProperty(std::shared_ptr<CInterpreter> interpreter,
	std::string name,
	WidgetStyle origStyle)
{
	switch (stringToPropertyType(name).type)
	{
	case STokenType::BOUNDS:
		minX = origStyle.minX;
		maxX = origStyle.maxX;
		minY = origStyle.minY;
		maxY = origStyle.maxY;
		relMinX = origStyle.relMinX;
		relMaxX = origStyle.relMaxX;
		relMinY = origStyle.relMinY;
		relMaxY = origStyle.relMaxY;
		break;
	case STokenType::WIDTH:
		minX = origStyle.minX;
		relMinX = origStyle.relMinX;

		break;
	case STokenType::MIN_WIDTH:
		minX = origStyle.relMaxY;
		relMinX = origStyle.relMaxY;
		break;
	case STokenType::MAX_WIDTH:
		maxX = origStyle.maxX;
		relMaxX = origStyle.relMaxX;
		break;
	case STokenType::HEIGHT:
		minY = origStyle.minY;
		relMinY = origStyle.relMinY;
		break;
	case STokenType::MIN_HEIGHT:
		minY = origStyle.minY;
		relMinY = origStyle.relMinY;
		break;
	case STokenType::MAX_HEIGHT:
		maxY = origStyle.relMaxY;
		relMaxY = origStyle.relMaxY;
		break;
	case STokenType::INNER_RADIUS:
		innerRadius = origStyle.innerRadius;
		break;
	case STokenType::OUTER_RADIUS:
		outerRadius = origStyle.outerRadius;
		break;
	case STokenType::POSITION:
		position = origStyle.position;
		break;
	case STokenType::LEFT:
		offset.left = origStyle.offset.left;
		break;
	case STokenType::RIGHT:
		offset.right = origStyle.offset.right;
		break;
	case STokenType::TOP:
		offset.top = origStyle.offset.top;
		break;
	case STokenType::BOTTOM:
		offset.bottom = origStyle.offset.bottom;
		break;
	case STokenType::ANCHOR:
		anchoring = origStyle.anchoring;
		break;
	case STokenType::FLOAT:
		floating = origStyle.floating;
		break;
	case STokenType::VISIBILITY:
		visibility = origStyle.visibility;
		break;
	case STokenType::ZINDEX:
		zIndex = origStyle.zIndex;
		break;
	case STokenType::$OVERFLOW:
		overflowX = origStyle.overflowX;
		overflowY = origStyle.overflowY;
		break;
	case STokenType::OVERFLOW_X:
		overflowX = origStyle.overflowX;
		break;
	case STokenType::OVERFLOW_Y:
		overflowY = origStyle.overflowY;
		break;
	case STokenType::CLIPPING:
		overrideClipping = origStyle.overrideClipping;
		break;
	case STokenType::DIMENSIONS:
		minX = origStyle.minX;
		maxX = origStyle.maxX;
		relMinX = origStyle.relMinX;
		relMaxX = origStyle.relMaxX;
		minY = origStyle.minY;
		maxY = origStyle.maxY;
		relMinY = origStyle.relMinY;
		relMaxY = origStyle.relMaxY;
		break;
	// Fill Color
	case STokenType::FILL_COLOR:
		fillColor = origStyle.fillColor;
		break;
	case STokenType::FILL_COLOR_RED:
		fillColor.r = origStyle.fillColor.r;
		break;
	case STokenType::FILL_COLOR_GREEN:
		fillColor.g = origStyle.fillColor.g;
		break;
	case STokenType::FILL_COLOR_BLUE:
		fillColor.b = origStyle.fillColor.b;
		break;
	case STokenType::FILL_COLOR_ALPHA:
		fillAlpha = origStyle.fillAlpha;
		break;
	// Focus Color
	case STokenType::FOCUS_COLOR:
		fillColor = origStyle.fillColor;
		break;
	case STokenType::FOCUS_COLOR_RED:
		fillColor.r = origStyle.fillColor.r;
		break;
	case STokenType::FOCUS_COLOR_GREEN:
		fillColor.g = origStyle.fillColor.g;
		break;
	case STokenType::FOCUS_COLOR_BLUE:
		fillColor.b = origStyle.fillColor.b;
		break;
	case STokenType::FOCUS_COLOR_ALPHA:
		fillAlpha = origStyle.fillAlpha;
		break;
	// Highlight Color
	case STokenType::HIGHLIGHT_COLOR:
		fillColor = origStyle.fillColor;
		break;
	case STokenType::HIGHLIGHT_COLOR_RED:
		fillColor.r = origStyle.fillColor.r;
		break;
	case STokenType::HIGHLIGHT_COLOR_GREEN:
		fillColor.g = origStyle.fillColor.g;
		break;
	case STokenType::HIGHLIGHT_COLOR_BLUE:
		fillColor.b = origStyle.fillColor.b;
		break;
	case STokenType::HIGHLIGHT_COLOR_ALPHA:
		fillAlpha = origStyle.fillAlpha;
		break;
	// Background Color
	case STokenType::BACKGROUND_COLOR:
		backgroundColor = origStyle.backgroundColor;
		break;
	case STokenType::BACKGROUND_COLOR_RED:
		backgroundColor.r = origStyle.backgroundColor.r;
		break;
	case STokenType::BACKGROUND_COLOR_GREEN:
		backgroundColor.g = origStyle.backgroundColor.g;
		break;
	case STokenType::BACKGROUND_COLOR_BLUE:
		backgroundColor.b = origStyle.backgroundColor.b;
		break;
	case STokenType::BACKGROUND_COLOR_ALPHA:
		backgroundAlpha = origStyle.backgroundAlpha;
		break;
	case STokenType::BACKGROUND_TEXTURE:
		backgroundTexture = origStyle.backgroundTexture;
		break;
	// Box Shadow Color
	case STokenType::BOX_SHADOW_COLOR:
		boxShadowColor = origStyle.boxShadowColor;
		break;
	case STokenType::BOX_SHADOW_COLOR_RED:
		boxShadowColor.r = origStyle.boxShadowColor.r;
		break;
	case STokenType::BOX_SHADOW_COLOR_GREEN:
		boxShadowColor.g = origStyle.boxShadowColor.g;
		break;
	case STokenType::BOX_SHADOW_COLOR_BLUE:
		boxShadowColor.b = origStyle.boxShadowColor.b;
		break;
	case STokenType::BOX_SHADOW_COLOR_ALPHA:
		boxShadowAlpha = origStyle.boxShadowAlpha;
		break;
	case STokenType::BOX_SHADOW_SIZE:
		boxShadowSizeX = origStyle.boxShadowSizeX;
		boxShadowSizeY = origStyle.boxShadowSizeY;
		break;
	case STokenType::BOX_SHADOW_SIZE_X:
		boxShadowSizeX = origStyle.boxShadowSizeX;
		break;
	case STokenType::BOX_SHADOW_SIZE_Y:
		boxShadowSizeY = origStyle.boxShadowSizeY;
		break;
	case STokenType::BORDER:
		border = origStyle.border;
		break;
	case STokenType::BORDER_TOP:
		border.top = origStyle.border.top;
		break;
	case STokenType::BORDER_BOTTOM:
		border.bottom = origStyle.border.bottom;
		break;
	case STokenType::BORDER_LEFT:
		border.left = origStyle.border.left;
		break;
	case STokenType::BORDER_RIGHT:
		border.right = origStyle.border.right;
		break;
	case STokenType::MARGIN:
		margin = origStyle.margin;
		break;
	case STokenType::MARGIN_TOP:
		margin.top = origStyle.margin.top;
		break;
	case STokenType::MARGIN_BOTTOM:
		margin.bottom = origStyle.margin.bottom;
		break;
	case STokenType::MARGIN_LEFT:
		margin.left = origStyle.margin.left;
		break;
	case STokenType::MARGIN_RIGHT:
		margin.right = origStyle.margin.right;
		break;
	case STokenType::FONT_PATH:
		fontPath = origStyle.fontPath;
		break;
	case STokenType::FONT_SIZE:
		fontSize = origStyle.fontSize;
		break;
	default:
		interpreter.get()->console.get()->warning(
			"[interpreter:6103] Invalid property name '" + name + "'passed to native function 'resetProperty' or 'resetClassProperty'.");
		return;
	}
}

std::shared_ptr<CObject> WidgetStyle::getSetProperty(
	std::shared_ptr<CInterpreter> interpreter,
	SToken name,
	std::shared_ptr<CObject> obj,
	bool isGet)
{
	// Deconvert Obj to string, in order to re-parse into value
	std::string fieldValue = interpreter.get()->console.get()->toString(obj);
	return getSetProperty(interpreter, name, fieldValue, isGet);
}

//
std::shared_ptr<CObject> WidgetStyle::getSetProperty(
	std::shared_ptr<CInterpreter> interpreter,
	std::string name,
	std::shared_ptr<CObject> obj,
	bool isGet)
{
	return getSetProperty(interpreter, stringToPropertyType(name), obj, isGet);
}

std::shared_ptr<CObject> WidgetStyle::getSetProperty(
	std::shared_ptr<CInterpreter> interpreter,
	SToken name,
	std::string fieldValue,
	bool isGet)
{
	std::vector<std::shared_ptr<CObject>> values;
	
	// For now assume good values
	bool triggerRebuild = false;
	switch (name.type)
	{
	case STokenType::BOUNDS:
		if (isGet)
		{
			values.push_back(std::make_shared<CObject>((double)minX));
			values.push_back(std::make_shared<CObject>((double)maxX));
			values.push_back(std::make_shared<CObject>((double)minY));
			values.push_back(std::make_shared<CObject>((double)maxY));
			return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(values));
		}
		evaluateQuadRelativeProperty(minX, maxX, minY, maxY, relMinX, relMaxX, relMinY, relMaxY, &fieldValue); triggerRebuild = true; break;
	case STokenType::WIDTH:
		if (isGet)
		{
			values.push_back(std::make_shared<CObject>((double)minX));
			values.push_back(std::make_shared<CObject>((double)maxX));
			return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(values));
		}
		evaluateRelativeStyleProperty(minX, relMinX, &fieldValue); maxX = minX; relMaxX = relMinX; triggerRebuild = true; break;
	case STokenType::MIN_WIDTH:
		if (isGet)
		{
			return std::make_shared<CObject>((double)minX);
		}
		evaluateRelativeStyleProperty(minX, relMinX, &fieldValue); triggerRebuild = true; break;
	case STokenType::MAX_WIDTH:
		if (isGet)
		{
			return std::make_shared<CObject>((double)maxX);
		}
		evaluateRelativeStyleProperty(maxX, relMaxX, &fieldValue); triggerRebuild = true; break;
	case STokenType::HEIGHT:
		if (isGet)
		{
			values.push_back(std::make_shared<CObject>((double)minY));
			values.push_back(std::make_shared<CObject>((double)maxY));
			return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(values));
		}
		evaluateRelativeStyleProperty(minY, relMinY, &fieldValue); maxY = minY; relMaxY = relMinY; triggerRebuild = true; break;
	case STokenType::MIN_HEIGHT:
		if (isGet)
		{
			return std::make_shared<CObject>((double)minY);
		}
		evaluateRelativeStyleProperty(minY, relMinY, &fieldValue); triggerRebuild = true; break;
	case STokenType::MAX_HEIGHT:
		if (isGet)
		{
			return std::make_shared<CObject>((double)maxY);
		}
		evaluateRelativeStyleProperty(maxY, relMaxY, &fieldValue); triggerRebuild = true; break;
	case STokenType::INNER_RADIUS:
		if (isGet)
		{
			return std::make_shared<CObject>((double)innerRadius);
		}
		evaluateNumericStyleProperty(innerRadius, &fieldValue); triggerRebuild = true; break;
	case STokenType::OUTER_RADIUS:
		if (isGet)
		{
			return std::make_shared<CObject>((double)outerRadius);
		}
		evaluateNumericStyleProperty(outerRadius, &fieldValue); triggerRebuild = true; break;
	case STokenType::POSITION:
		if (isGet)
		{
			return std::make_shared<CObject>(propertyEnumToString("position", position));
		}
		evaluatePositionProperty(position, &fieldValue); triggerRebuild = true; break;
	case STokenType::LEFT:
		if (isGet)
		{
			return std::make_shared<CObject>((double)offset.left);
		}
		evaluateNumericStyleProperty(offset.left, &fieldValue); triggerRebuild = true; break;
	case STokenType::RIGHT:
		if (isGet)
		{
			return std::make_shared<CObject>((double)offset.right);
		}
		evaluateNumericStyleProperty(offset.right, &fieldValue); triggerRebuild = true; break;
	case STokenType::TOP:
		if (isGet)
		{
			return std::make_shared<CObject>((double)offset.top);
		}
		evaluateNumericStyleProperty(offset.top, &fieldValue); triggerRebuild = true; break;
	case STokenType::BOTTOM:
		if (isGet)
		{
			return std::make_shared<CObject>((double)offset.bottom);
		}
		evaluateNumericStyleProperty(offset.bottom, &fieldValue); triggerRebuild = true; break;
	case STokenType::ANCHOR:
		if (isGet)
		{
			return std::make_shared<CObject>(propertyEnumToString("anchoring", anchoring));
		}
		evaluateAnchoringProperty(anchoring, &fieldValue); break;
	case STokenType::FLOAT:
		if (isGet)
		{
			return std::make_shared<CObject>(propertyEnumToString("float", floating));
		}
		evaluateFloatingProperty(floating, &fieldValue); break;
	case STokenType::VISIBILITY:
		if (isGet)
		{
			return std::make_shared<CObject>(propertyEnumToString("visibility", visibility));
		}
		evaluateVisibilityProperty(visibility, &fieldValue); triggerRebuild = true; break;
	case STokenType::ZINDEX:
		if (isGet)
		{
			return std::make_shared<CObject>((double)zIndex);
		}
		evaluateNumericStyleProperty(zIndex, &fieldValue); triggerRebuild = false; break;
	case STokenType::$OVERFLOW:
		if (isGet)
		{
			values.push_back(std::make_shared<CObject>(propertyEnumToString("overflow", overflowX)));
			values.push_back(std::make_shared<CObject>(propertyEnumToString("overflow", overflowY)));
			return std::make_shared<CObject>(CLiteralTypes::_CString_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(values));
		}
		evaluateDoubleOverflowProperty(overflowX, overflowY, &fieldValue); break;
	case STokenType::OVERFLOW_X:
		if (isGet)
		{
			return std::make_shared<CObject>(propertyEnumToString("overflow", overflowX));
		}
		evaluateOverflowProperty(overflowX, &fieldValue); break;
	case STokenType::OVERFLOW_Y:
		if (isGet)
		{
			return std::make_shared<CObject>(propertyEnumToString("overflow", overflowY));
		}
		evaluateOverflowProperty(overflowY, &fieldValue); break;
	case STokenType::CLIPPING:
		if (isGet)
		{
			return std::make_shared<CObject>(overrideClipping);
		}
		evaluateBoolProperty(overrideClipping, &fieldValue, false); break;
	case STokenType::DIMENSIONS:
		if (isGet)
		{
			values.push_back(std::make_shared<CObject>((double)minX));
			values.push_back(std::make_shared<CObject>((double)minY));
			values.push_back(std::make_shared<CObject>((double)maxX));
			values.push_back(std::make_shared<CObject>((double)maxY));
			return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(values));
		}
		evaluateDoubleRelativeStyleProperty(minX, maxX, minY, maxY, relMinX, relMaxX, relMinY, relMaxY, &fieldValue); break;
	case STokenType::FILL_COLOR:
		if (isGet)
		{
			return std::make_shared<CObject>(glm::dvec4(
				(double)fillColor.r,
				(double)fillColor.g,
				(double)fillColor.b,
				(double)fillAlpha));
		}
		evaluateColorStyleProperty(fillColor, fillAlpha, &fieldValue); break;
	case STokenType::FILL_COLOR_RED:
		if (isGet)
		{
			return std::make_shared<CObject>((double)fillColor.r);
		}
		evaluateNumericStyleProperty(fillColor.r, &fieldValue); break;
	case STokenType::FILL_COLOR_GREEN:
		if (isGet)
		{
			return std::make_shared<CObject>((double)fillColor.g);
		}
		evaluateNumericStyleProperty(fillColor.g, &fieldValue); break;
	case STokenType::FILL_COLOR_BLUE:
		if (isGet)
		{
			return std::make_shared<CObject>((double)fillColor.b);
		}
		evaluateNumericStyleProperty(fillColor.b, &fieldValue); break;
	case STokenType::FILL_COLOR_ALPHA:
		if (isGet)
		{
			return std::make_shared<CObject>((double)fillAlpha);
		}
		evaluateNumericStyleProperty(fillAlpha, &fieldValue); break;
	// Focus Color
	case STokenType::FOCUS_COLOR:
		if (isGet)
		{
			return std::make_shared<CObject>(glm::dvec4(
				(double)focusColor.r,
				(double)focusColor.g,
				(double)focusColor.b,
				(double)focusColor.a));
		}
		evaluateColorStyleProperty(focusColor, focusColor.a, &fieldValue); break;
	case STokenType::FOCUS_COLOR_RED:
		if (isGet)
		{
			return std::make_shared<CObject>((double)focusColor.r);
		}
		evaluateNumericStyleProperty(focusColor.r, &fieldValue); break;
	case STokenType::FOCUS_COLOR_GREEN:
		if (isGet)
		{
			return std::make_shared<CObject>((double)focusColor.g);
		}
		evaluateNumericStyleProperty(focusColor.g, &fieldValue); break;
	case STokenType::FOCUS_COLOR_BLUE:
		if (isGet)
		{
			return std::make_shared<CObject>((double)focusColor.b);
		}
		evaluateNumericStyleProperty(focusColor.b, &fieldValue); break;
	case STokenType::FOCUS_COLOR_ALPHA:
		if (isGet)
		{
			return std::make_shared<CObject>((double)focusColor.a);
		}
		evaluateNumericStyleProperty(focusColor.a, &fieldValue); break;
	// Highlight Color
	case STokenType::HIGHLIGHT_COLOR:
		if (isGet)
		{
			return std::make_shared<CObject>(glm::dvec4(
				(double)highlightColor.r,
				(double)highlightColor.g,
				(double)highlightColor.b,
				(double)highlightColor.a));
		}
		evaluateColorStyleProperty(highlightColor, highlightColor.a, &fieldValue); break;
	case STokenType::HIGHLIGHT_COLOR_RED:
		if (isGet)
		{
			return std::make_shared<CObject>((double)highlightColor.r);
		}
		evaluateNumericStyleProperty(highlightColor.r, &fieldValue); break;
	case STokenType::HIGHLIGHT_COLOR_GREEN:
		if (isGet)
		{
			return std::make_shared<CObject>((double)highlightColor.g);
		}
		evaluateNumericStyleProperty(highlightColor.g, &fieldValue); break;
	case STokenType::HIGHLIGHT_COLOR_BLUE:
		if (isGet)
		{
			return std::make_shared<CObject>((double)highlightColor.b);
		}
		evaluateNumericStyleProperty(highlightColor.b, &fieldValue); break;
	case STokenType::HIGHLIGHT_COLOR_ALPHA:
		if (isGet)
		{
			return std::make_shared<CObject>((double)highlightColor.a);
		}
		evaluateNumericStyleProperty(highlightColor.a, &fieldValue); break;
	// Background Color
	case STokenType::BACKGROUND_COLOR:
		if (isGet)
		{
			return std::make_shared<CObject>(glm::dvec4(
				(double)backgroundColor.r,
				(double)backgroundColor.g,
				(double)backgroundColor.b,
				(double)backgroundAlpha));
		}
		evaluateColorStyleProperty(backgroundColor, backgroundAlpha, &fieldValue); break;
	case STokenType::BACKGROUND_COLOR_RED:
		if (isGet)
		{
			return std::make_shared<CObject>((double)backgroundColor.r);
		}
		evaluateNumericStyleProperty(backgroundColor.r, &fieldValue); break;
	case STokenType::BACKGROUND_COLOR_GREEN:
		if (isGet)
		{
			return std::make_shared<CObject>((double)backgroundColor.g);
		}
		evaluateNumericStyleProperty(backgroundColor.g, &fieldValue); break;
	case STokenType::BACKGROUND_COLOR_BLUE:
		if (isGet)
		{
			return std::make_shared<CObject>((double)backgroundColor.b);
		}
		evaluateNumericStyleProperty(backgroundColor.b, &fieldValue); break;
	case STokenType::BACKGROUND_COLOR_ALPHA:
		if (isGet)
		{
			return std::make_shared<CObject>((double)backgroundAlpha);
		}
		evaluateNumericStyleProperty(backgroundAlpha, &fieldValue); break;
	case STokenType::BACKGROUND_TEXTURE:
		if (isGet)
		{
			return std::make_shared<CObject>(backgroundTexture.string());
		}
		backgroundTexture = fieldValue; break;
	case STokenType::BOX_SHADOW_COLOR:
		if (isGet)
		{
			return std::make_shared<CObject>(glm::dvec4(
				(double)boxShadowColor.r,
				(double)boxShadowColor.g,
				(double)boxShadowColor.b,
				(double)boxShadowAlpha));
		}
		evaluateColorStyleProperty(boxShadowColor, boxShadowAlpha, &fieldValue); break;
	case STokenType::BOX_SHADOW_COLOR_RED:
		if (isGet)
		{
			return std::make_shared<CObject>((double)boxShadowColor.r);
		}
		evaluateNumericStyleProperty(boxShadowColor.r, &fieldValue); break;
	case STokenType::BOX_SHADOW_COLOR_GREEN:
		if (isGet)
		{
			return std::make_shared<CObject>((double)boxShadowColor.r);
		}
		evaluateNumericStyleProperty(boxShadowColor.g, &fieldValue); break;
	case STokenType::BOX_SHADOW_COLOR_BLUE:
		if (isGet)
		{
			return std::make_shared<CObject>((double)boxShadowColor.r);
		}
		evaluateNumericStyleProperty(boxShadowColor.b, &fieldValue); break;
	case STokenType::BOX_SHADOW_COLOR_ALPHA:
		if (isGet)
		{
			return std::make_shared<CObject>((double)boxShadowAlpha);
		}
		evaluateNumericStyleProperty(boxShadowAlpha, &fieldValue); break;
	case STokenType::BOX_SHADOW_SIZE:
		if (isGet)
		{
			values.push_back(std::make_shared<CObject>((double)boxShadowSizeX));
			values.push_back(std::make_shared<CObject>((double)boxShadowSizeY));
			return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(values));
		}
		evaluateDoubleNumericProperty(boxShadowSizeX, boxShadowSizeY, &fieldValue); break;
	case STokenType::BOX_SHADOW_SIZE_X:
		if (isGet)
		{
			return std::make_shared<CObject>((double)boxShadowSizeX);
		}
		evaluateNumericStyleProperty(boxShadowSizeX, &fieldValue); break;
	case STokenType::BOX_SHADOW_SIZE_Y:
		if (isGet)
		{
			return std::make_shared<CObject>((double)boxShadowSizeY);
		}
		evaluateNumericStyleProperty(boxShadowSizeY, &fieldValue); break;
	case STokenType::BORDER:
		if (isGet)
		{
			values.push_back(std::make_shared<CObject>((double)border.left));
			values.push_back(std::make_shared<CObject>((double)border.right));
			values.push_back(std::make_shared<CObject>((double)border.top));
			values.push_back(std::make_shared<CObject>((double)border.bottom));
			return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(values));
		}
		evaluateQuadNumericProperty(border.left, border.right, border.top, border.bottom, &fieldValue); triggerRebuild = true; break;
	case STokenType::BORDER_TOP:
		if (isGet)
		{
			return std::make_shared<CObject>((double)border.top);
		}
		evaluateNumericStyleProperty(border.top, &fieldValue); triggerRebuild = true; break;
	case STokenType::BORDER_BOTTOM:
		if (isGet)
		{
			return std::make_shared<CObject>((double)border.bottom);
		}
		evaluateNumericStyleProperty(border.bottom, &fieldValue); triggerRebuild = true; break;
	case STokenType::BORDER_LEFT:
		if (isGet)
		{
			return std::make_shared<CObject>((double)border.left);
		}
		evaluateNumericStyleProperty(border.left, &fieldValue); triggerRebuild = true; break;
	case STokenType::BORDER_RIGHT:
		if (isGet)
		{
			return std::make_shared<CObject>((double)border.right);
		}
		evaluateNumericStyleProperty(border.right, &fieldValue); triggerRebuild = true; break;
	case STokenType::MARGIN:
		if (isGet)
		{
			values.push_back(std::make_shared<CObject>((double)margin.left));
			values.push_back(std::make_shared<CObject>((double)margin.right));
			values.push_back(std::make_shared<CObject>((double)margin.top));
			values.push_back(std::make_shared<CObject>((double)margin.bottom));
			return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array,
				std::make_shared<std::vector<std::shared_ptr<CObject>>>(values));
		}
		evaluateQuadNumericProperty(margin.left, margin.right, margin.top, margin.bottom, &fieldValue); triggerRebuild = true; break;
	case STokenType::MARGIN_TOP:
		if (isGet)
		{
			return std::make_shared<CObject>((double)margin.top);
		}
		evaluateNumericStyleProperty(margin.top, &fieldValue); triggerRebuild = true; break;
	case STokenType::MARGIN_BOTTOM:
		if (isGet)
		{
			return std::make_shared<CObject>((double)margin.bottom);
		}
		evaluateNumericStyleProperty(margin.bottom, &fieldValue); triggerRebuild = true; break;
	case STokenType::MARGIN_LEFT:
		if (isGet)
		{
			return std::make_shared<CObject>((double)margin.left);
		}
		evaluateNumericStyleProperty(margin.left, &fieldValue); triggerRebuild = true; break;
	case STokenType::MARGIN_RIGHT:
		if (isGet)
		{
			return std::make_shared<CObject>((double)margin.right);
		}
		evaluateNumericStyleProperty(margin.right, &fieldValue); break;
	case STokenType::FONT_PATH:
		if (isGet)
		{
			return std::make_shared<CObject>(fontPath.string());
		}
		fontPath = fieldValue; break;
	case STokenType::FONT_SIZE:
		if (isGet)
		{
			return std::make_shared<CObject>((double)fontSize);
		}
		evaluateNumericStyleProperty(fontSize, &fieldValue); triggerRebuild = true; break;
	case STokenType::VERTEX_COLOR:
		if (isGet && vertexColor.count(name.special) == 1)
		{
			return std::make_shared<CObject>(glm::dvec4(
				(double)vertexColor.at(name.special).r,
				(double)vertexColor.at(name.special).g,
				(double)vertexColor.at(name.special).b,
				(double)vertexColor.at(name.special).a));
		}
		else if (isGet) { break; }
		if (vertexColor.count(name.special) == 0)
		{
			CColor newColor = CColor();
			evaluateColorStyleProperty(newColor, newColor.a, &fieldValue);
			vertexColor.insert(std::pair<unsigned char, CColor>(
				name.special, newColor));
		}
		else
		{
			evaluateColorStyleProperty(vertexColor.at(name.special), vertexColor.at(name.special).a, &fieldValue);
		}
		triggerRebuild = true; break;
	case STokenType::VERTEX_COLOR_RED:
		if (isGet && vertexColor.count(name.special) == 1)
		{
			return std::make_shared<CObject>((double)vertexColor.at(name.special).r);
		}
		else if (isGet) { break; }
		if (vertexColor.count(name.special) == 0)
		{
			CColor newColor = CColor();
			evaluateNumericStyleProperty(newColor.r, &fieldValue);
			vertexColor.insert(std::pair<unsigned char, CColor>(
				name.special, newColor));
		}
		else
		{
			evaluateNumericStyleProperty(vertexColor.at(name.special).r, &fieldValue);
		}
		triggerRebuild = true; break;
	case STokenType::VERTEX_COLOR_GREEN:
		if (isGet && vertexColor.count(name.special) == 1)
		{
			return std::make_shared<CObject>((double)fillColor.g);
		}
		else if (isGet) { break; }
		if (vertexColor.count(name.special) == 0)
		{
			CColor newColor = CColor();
			evaluateNumericStyleProperty(newColor.g, &fieldValue);
			vertexColor.insert(std::pair<unsigned char, CColor>(
				name.special, newColor));
		}
		else
		{
			evaluateNumericStyleProperty(vertexColor.at(name.special).g, &fieldValue);
		}
		triggerRebuild = true; break;
	case STokenType::VERTEX_COLOR_BLUE:
		if (isGet && vertexColor.count(name.special) == 1)
		{
			return std::make_shared<CObject>((double)vertexColor.at(name.special).b);
		}
		else if (isGet) { break; }
		if (vertexColor.count(name.special) == 0)
		{
			CColor newColor = CColor();
			evaluateNumericStyleProperty(newColor.b, &fieldValue);
			vertexColor.insert(std::pair<unsigned char, CColor>(
				name.special, newColor));
		}
		else
		{
			evaluateNumericStyleProperty(vertexColor.at(name.special).b, &fieldValue);
		}
		triggerRebuild = true; break;
	case STokenType::VERTEX_COLOR_ALPHA:
		if (isGet && vertexColor.count(name.special) == 1)
		{
			return std::make_shared<CObject>((double)vertexColor.at(name.special).a);
		}
		else if (isGet) { break; }
		if (vertexColor.count(name.special) == 0)
		{
			CColor newColor = CColor();
			evaluateNumericStyleProperty(newColor.a, &fieldValue);
			vertexColor.insert(std::pair<unsigned char, CColor>(
				name.special, newColor));
		}
		else
		{
			evaluateNumericStyleProperty(vertexColor.at(name.special).a, &fieldValue);
		}
		triggerRebuild = true; break;
	default:
		if (isGet)
		{
			interpreter.get()->console.get()->warning(
				"[interpreter:6103] Invalid property name '" + STokenTypeNames[size_t(name.type)] + "'passed to native function 'getProperty_byName'.");
			return std::make_shared<CObject>(nullptr);
		}
		else
		{
			interpreter.get()->console.get()->warning(
				"[interpreter:6103] Invalid property name '" + STokenTypeNames[size_t(name.type)] + "' passed to native function 'setProperty_byName'.");
			break;
		}

	}
	if (triggerRebuild) { return std::make_shared<CObject>(true); }
	return nullptr;
}


SToken WidgetStyle::stringToPropertyType(std::string name)
{
	size_t index = std::string::npos;
	if (styleTypeMap.count(name) == 1)
	{
		return SToken(styleTypeMap.at(name), 0);
	}
	else if (name.find("vertex-") != std::string::npos && name.find("-color") != std::string::npos)
	{
		size_t pos = name.find("vertex-") + 7;
		int i = 0;
		std::string str = "";
		while (isdigit(name[pos + i]))
		{
			str.push_back(name[pos + i]);
			i++;
		}
		unsigned char special = (unsigned char)std::stoi(str);
		if (name[pos + 0 + i] == '-' &&
			name[pos + 1 + i] == 'c' &&
			name[pos + 2 + i] == 'o' &&
			name[pos + 3 + i] == 'l' &&
			name[pos + 4 + i] == 'o' &&
			name[pos + 5 + i] == 'r')
		{
			if (name[pos + 6 + i] != '-')
			{
				return SToken(STokenType::VERTEX_COLOR, "", 0, special);
			}
			else
			{
				if (name[pos + 6 + i] == '-' &&
					name[pos + 7 + i] == 'a' &&
					name[pos + 8 + i] == 'l' &&
					name[pos + 9 + i] == 'p' &&
					name[pos + 10 + i] == 'h' &&
					name[pos + 11 + i] == 'a' &&
					(!isalpha(name[pos + 12 + i])))
				{
					return SToken(STokenType::VERTEX_COLOR_ALPHA, "", 0, special);
				}
				if (name[pos + 6 + i] == '-' &&
					name[pos + 7 + i] == 'b' &&
					name[pos + 8 + i] == 'l' &&
					name[pos + 9 + i] == 'u' &&
					name[pos + 10 + i] == 'e' &&
					(!isalpha(name[pos + 11 + i])))
				{
					return SToken(STokenType::VERTEX_COLOR_BLUE, "", 0, special);
				}
				if (name[pos + 6 + i] == '-' &&
					name[pos + 7 + i] == 'r' &&
					name[pos + 8 + i] == 'e' &&
					name[pos + 9 + i] == 'd' &&
					(!isalpha(name[pos + 10 + i])))
				{
					return SToken(STokenType::VERTEX_COLOR_RED, "", 0, special);
				}
				if (name[pos + 6 + i] == '-' &&
					name[pos + 7 + i] == 'g' &&
					name[pos + 8 + i] == 'r' &&
					name[pos + 9 + i] == 'e' &&
					name[pos + 10 + i] == 'e' &&
					name[pos + 11 + i] == 'n' &&
					(!isalpha(name[pos + 12 + i])))
				{
					return SToken(STokenType::VERTEX_COLOR_GREEN, "", 0, special);
				}
			}
		}
	}
	return SToken(STokenType::NIL, 0);
}

// Merge style data from another WidgetStyle object into this one
void WidgetStyle::mergeStyle(std::shared_ptr<WidgetStyle> style)
{
	// Create a comparator style with default values (Gets destructed at end of func)
	WidgetStyle defaultStyle = WidgetStyle();
	// Each value overwrites this style, and avoids overwriting if the new value = default
	
	// Warning: Because there is no "unused" value for several WidgetStyle properties, if a style should erase
	// overwrite a value with 0 or a default boolean value, then it will not do so.
	if (style.get()->name != defaultStyle.name)
	{
		this->name = style.get()->name;
	}
	// Size
	if (style.get()->minX != defaultStyle.minX)
	{
		this->minX = style.get()->minX;
		this->relMinX = style.get()->relMinX;
	}
	if (style.get()->maxX != defaultStyle.maxX)
	{
		this->maxX = style.get()->maxX;
		this->relMaxX = style.get()->relMaxX;
	}
	if (style.get()->minY != defaultStyle.minY)
	{
		this->minY = style.get()->minY;
		this->relMinY = style.get()->relMinY;
	}
	if (style.get()->maxY != defaultStyle.maxY)
	{
		this->maxY = style.get()->maxY;
		this->relMaxY = style.get()->relMaxY;
	}
	// Position
	if (style.get()->position != defaultStyle.position)
	{
		this->position = style.get()->position;
	}
	// Radius
	if (style.get()->innerRadius != defaultStyle.innerRadius)
	{
		this->innerRadius = style.get()->innerRadius;
	}
	if (style.get()->outerRadius != defaultStyle.outerRadius)
	{
		this->outerRadius = style.get()->outerRadius;
	}
	// Offset
	if (style.get()->offset.left != defaultStyle.offset.left)
	{
		this->offset.left = style.get()->offset.left;
	}
	if (style.get()->offset.right != defaultStyle.offset.right)
	{
		this->offset.right = style.get()->offset.right;
	}
	if (style.get()->offset.top != defaultStyle.offset.top)
	{
		this->offset.top = style.get()->offset.top;
	}
	if (style.get()->offset.bottom != defaultStyle.offset.bottom)
	{
		this->offset.bottom = style.get()->offset.bottom;
	}
	// Border
	if (style.get()->border.top != defaultStyle.border.top)
	{
		this->border.top = style.get()->border.top;
	}
	if (style.get()->border.bottom != defaultStyle.border.bottom)
	{
		this->border.bottom = style.get()->border.bottom;
	}
	if (style.get()->border.left != defaultStyle.border.left)
	{
		this->border.left = style.get()->border.left;
	}
	if (style.get()->border.right != defaultStyle.border.right)
	{
		this->border.right = style.get()->border.right;
	}
	// Margin
	if (style.get()->margin.top != defaultStyle.margin.top)
	{
		this->margin.top = style.get()->margin.top;
	}
	if (style.get()->margin.bottom != defaultStyle.margin.bottom)
	{
		this->margin.bottom = style.get()->margin.bottom;
	}
	if (style.get()->margin.left != defaultStyle.margin.left)
	{
		this->margin.left = style.get()->margin.left;
	}
	if (style.get()->margin.right != defaultStyle.margin.right)
	{
		this->margin.right = style.get()->margin.right;
	}
	// Anchoring / Floating
	if (style.get()->anchoring != defaultStyle.anchoring)
	{
		this->anchoring = style.get()->anchoring;
	}
	if (style.get()->floating != defaultStyle.floating)
	{
		this->floating = style.get()->floating;
	}
	// Fill Color / Alpha
	if (style.get()->fillColor.r != defaultStyle.fillColor.r)
	{
		this->fillColor.r = style.get()->fillColor.r;
	}
	if (style.get()->fillColor.g != defaultStyle.fillColor.g)
	{
		this->fillColor.g = style.get()->fillColor.g;
	}
	if (style.get()->fillColor.b != defaultStyle.fillColor.b)
	{
		this->fillColor.b = style.get()->fillColor.b;
	}
	if (style.get()->fillAlpha != defaultStyle.fillAlpha)
	{
		this->fillAlpha = style.get()->fillAlpha;
	}
	// Focus Color / Alpha
	if (style.get()->focusColor.r != defaultStyle.focusColor.r)
	{
		this->focusColor.r = style.get()->focusColor.r;
	}
	if (style.get()->focusColor.g != defaultStyle.focusColor.g)
	{
		this->focusColor.g = style.get()->focusColor.g;
	}
	if (style.get()->focusColor.b != defaultStyle.focusColor.b)
	{
		this->focusColor.b = style.get()->focusColor.b;
	}
	if (style.get()->focusColor.a != defaultStyle.focusColor.a)
	{
		this->focusColor.a = style.get()->focusColor.a;
	}
	// Highlight Color / Alpha
	if (style.get()->highlightColor.r != defaultStyle.highlightColor.r)
	{
		this->highlightColor.r = style.get()->highlightColor.r;
	}
	if (style.get()->highlightColor.g != defaultStyle.highlightColor.g)
	{
		this->highlightColor.g = style.get()->highlightColor.g;
	}
	if (style.get()->highlightColor.b != defaultStyle.highlightColor.b)
	{
		this->highlightColor.b = style.get()->highlightColor.b;
	}
	if (style.get()->highlightColor.a != defaultStyle.highlightColor.a)
	{
		this->highlightColor.a = style.get()->highlightColor.a;
	}
	// Background Color / Alpha / Texture
	if (style.get()->backgroundColor.r != defaultStyle.backgroundColor.r)
	{
		this->backgroundColor.r = style.get()->backgroundColor.r;
	}
	if (style.get()->backgroundColor.g != defaultStyle.backgroundColor.g)
	{
		this->backgroundColor.g = style.get()->backgroundColor.g;
	}
	if (style.get()->backgroundColor.b != defaultStyle.backgroundColor.b)
	{
		this->backgroundColor.b = style.get()->backgroundColor.b;
	}
	if (style.get()->backgroundAlpha != defaultStyle.backgroundAlpha)
	{
		this->backgroundAlpha = style.get()->backgroundAlpha;
	}
	if (style.get()->backgroundTexture != defaultStyle.backgroundTexture)
	{
		this->backgroundTexture = style.get()->backgroundTexture;
	}
	// Box Shadow Color
	if (style.get()->boxShadowColor.r != defaultStyle.boxShadowColor.r)
	{
		this->boxShadowColor.r = style.get()->boxShadowColor.r;
	}
	if (style.get()->boxShadowColor.g != defaultStyle.boxShadowColor.g)
	{
		this->boxShadowColor.g = style.get()->boxShadowColor.g;
	}
	if (style.get()->boxShadowColor.b != defaultStyle.boxShadowColor.b)
	{
		this->boxShadowColor.b = style.get()->boxShadowColor.b;
	}
	if (style.get()->boxShadowAlpha != defaultStyle.boxShadowAlpha)
	{
		this->boxShadowAlpha = style.get()->boxShadowAlpha;
	}
	// Box Shadow Size
	if (style.get()->boxShadowSizeX != defaultStyle.boxShadowSizeX)
	{
		this->boxShadowSizeX = style.get()->boxShadowSizeX;
	}
	if (style.get()->boxShadowSizeY != defaultStyle.boxShadowSizeY)
	{
		this->boxShadowSizeY = style.get()->boxShadowSizeY;
	}
	// Clipping / Visibility / Overflow / ZIndex
	if (style.get()->overrideClipping != defaultStyle.overrideClipping)
	{
		this->overrideClipping = style.get()->overrideClipping;
	}
	if (style.get()->visibility != defaultStyle.visibility)
	{
		this->visibility = style.get()->visibility;
	}
	if (style.get()->overflowX != defaultStyle.overflowX)
	{
		this->overflowX = style.get()->overflowX;
	}
	if (style.get()->overflowY != defaultStyle.overflowY)
	{
		this->overflowY = style.get()->overflowY;
	}
	if (style.get()->zIndex != defaultStyle.zIndex)
	{
		this->zIndex = style.get()->zIndex;
	}
	// FontPath / Size
	if (style.get()->fontPath != defaultStyle.fontPath)
	{
		this->fontPath = style.get()->fontPath;
	}
	if (style.get()->fontSize != defaultStyle.fontSize)
	{
		this->fontSize = style.get()->fontSize;
	}
	// Vertex Color
	// Warning: Incorrect copy method, using for testing only
	if (style.get()->vertexColor.size() != 0)
	{
		this->vertexColor = style.get()->vertexColor;
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Style String to Value Parse Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Int Enum to String
std::string WidgetStyle::propertyEnumToString(std::string propSelect, int p)
{
	if (propSelect == "anchoring")
	{
		switch (p)
		{
		case -1: return "none";
		case 0: return "upper-left";
		case 1: return "upper-center";
		case 2: return "upper-right";
		case 3: return "center-left";
		case 4: return "center";
		case 5: return "center-right";
		case 6: return "lower-left";
		case 7: return "lower-center";
		case 8: return "lower-right";
		default: return "none";
		}
	}
	if (propSelect == "position")
	{
		switch (p)
		{
		case 0: return "static";
		case 1: return "absolute";
		case 2: return "relative";
		case 3: return "fixed";
		default: return "none";
		}
	}
	if (propSelect == "float")
	{
		switch (p)
		{
		case -1: return "none";
		case 0: return "left";
		case 1: return "center";
		case 2: return "right";
		default: return "none";
		}
	}
	if (propSelect == "overflow")
	{
		switch (p)
		{
		case -1: return "none";
		case 0: return "visible";
		case 1: return "hidden";
		case 2: return "scroll";
		case 3: return "auto";
		default: return "none";
		}
	}
	if (propSelect == "visibility")
	{
		switch (p)
		{
		case -1: return "none";
		case 0: return "hide";
		case 1: return "show";
		default: return "none";
		}
	}
	return "none";
}

// Evaluate integer-type properties
void WidgetStyle::evaluateNumericStyleProperty(unsigned int& prop, std::string* value)
{
	if (*value == "inherit") { prop = UI_WVALUE_INHERIT; return; }
	else if (*value == "fill") { prop = UI_WVALUE_FILL; return; }
	else if (*value == "min") { prop = UI_WVALUE_MIN; return; }
	else if (*value == "max") { prop = UI_WVALUE_MAX; return; }
	else if (*value == "noreport") { prop = UI_WVALUE_INHERIT; return; }
	else { prop = (unsigned int)std::stoi(*value); }
}
void WidgetStyle::evaluateNumericStyleProperty(int& prop, std::string* value)
{
	if (*value == "inherit") { prop = UI_WVALUE_INHERIT; return; }
	else if (*value == "fill") { prop = UI_WVALUE_FILL; return; }
	else if (*value == "min") { prop = UI_WVALUE_MIN; return; }
	else if (*value == "max") { prop = UI_WVALUE_MAX; return; }
	else if (*value == "noreport") { prop = UI_WVALUE_INHERIT; return; }
	else { prop = std::stoi(*value); }
}
void WidgetStyle::evaluateNumericStyleProperty(float& prop, std::string* value)
{
	if (*value == "inherit") { prop = UI_WVALUE_INHERIT; return; }
	else if (*value == "fill") { prop = UI_WVALUE_FILL; return; }
	else if (*value == "min") { prop = UI_WVALUE_MIN; return; }
	else if (*value == "max") { prop = UI_WVALUE_MAX; return; }
	else if (*value == "noreport") { prop = UI_WVALUE_INHERIT; return; }
	else { prop = std::stof(*value); }
}
void WidgetStyle::evaluateDoubleNumericProperty(int& a, int& b, std::string* value)
{
	if (*value == "inherit") { a = UI_WVALUE_INHERIT; b = UI_WVALUE_INHERIT; return; }
	else if (*value == "fill") { a = UI_WVALUE_FILL; b = UI_WVALUE_FILL; return; }
	else if (*value == "min") { a = UI_WVALUE_MIN; b = UI_WVALUE_MIN; return; }
	else if (*value == "max") { a = UI_WVALUE_MAX; b = UI_WVALUE_MAX; return; }
	else if (*value == "noreport") { a = UI_WVALUE_NOREPORT; b = UI_WVALUE_NOREPORT; return; }
	std::string modString = *value;
	std::string checkString = modString;
	// Assume that this function always receives 2 values in the string, and contains no other characters
	a = std::stoi(splitRegularString(modString, ","));
	if (modString == checkString) { return; }
	else { checkString = modString; }
	b = std::stoi(modString);
}
void WidgetStyle::evaluateQuadNumericProperty(int& a, int& b, int& c, int& d, std::string* value)
{
	if (*value == "inherit") { a = UI_WVALUE_INHERIT; b = UI_WVALUE_INHERIT; c = UI_WVALUE_INHERIT; d = UI_WVALUE_INHERIT; return; }
	else if (*value == "fill") { a = UI_WVALUE_FILL; b = UI_WVALUE_FILL; c = UI_WVALUE_FILL; d = UI_WVALUE_FILL; return; }
	else if (*value == "min") { a = UI_WVALUE_MIN; b = UI_WVALUE_MIN; c = UI_WVALUE_MIN; d = UI_WVALUE_MIN; return; }
	else if (*value == "max") { a = UI_WVALUE_MAX; b = UI_WVALUE_MAX; c = UI_WVALUE_MAX; d = UI_WVALUE_MAX; return; }
	else if (*value == "noreport") { a = UI_WVALUE_NOREPORT; b = UI_WVALUE_NOREPORT; c = UI_WVALUE_NOREPORT; d = UI_WVALUE_NOREPORT; return; }
	std::string modString = *value;
	std::string checkString = modString;
	// Assume that this function always receives 4 values in the string, and contains no other characters
	a = std::stoi(splitRegularString(modString, ","));
	if (modString == checkString) { return; }
	else { checkString = modString; }
	b = std::stoi(splitRegularString(modString, ","));
	if (modString == checkString) { return; }
	else { checkString = modString; }
	c = std::stoi(splitRegularString(modString, ","));
	if (modString == checkString) { return; }
	else { checkString = modString; }
	d = std::stoi(modString);
}
// Evaluate float-type properties, handling for % and 0.0-1.0 values
void WidgetStyle::evaluateRelativeStyleProperty(float& prop, bool& makeRelative, std::string* value)
{
	if (*value == "inherit") { prop = (float)UI_WVALUE_INHERIT; makeRelative = false; return; }
	else if (*value == "fill") { prop = 1.0f; makeRelative = true; return; }
	else if (*value == "min") { prop = (float)UI_WVALUE_MIN; makeRelative = false; return; }
	else if (*value == "max") { prop = (float)UI_WVALUE_MAX; makeRelative = false; return; }
	else if (*value == "noreport") { prop = (float)UI_WVALUE_INHERIT; makeRelative = false; return; }
	else
	{
		if (value->find("%") != std::string::npos)
		{
			prop = clampf((std::stof(*value) / 100), 0.0f, 1.0f);
			makeRelative = true;
			return;
		}
		/*
		else if (std::stof(*value) <= 1.0f && std::stof(*value) >= 0.0f)
		{
			prop = clampf(std::stof(*value), 0.0f, 1.0f);
			makeRelative = true;
			return;
		}
		*/
		else
		{
			prop = floor(std::stof(*value));
			makeRelative = false;
		}
	}
}
void WidgetStyle::evaluateDoubleRelativeStyleProperty(float& a, float& b, float& c, float& d, bool& boolA, bool& boolB, bool& boolC, bool& boolD, std::string* value)
{
	if (*value == "inherit") {
		a = (float)UI_WVALUE_INHERIT; b = (float)UI_WVALUE_INHERIT; c = (float)UI_WVALUE_INHERIT; d = (float)UI_WVALUE_INHERIT;
		boolA = false; boolB = false; boolC = false; boolD = false; return;
	}
	else if (*value == "fill") {
		a = 1.0f; b = 1.0f; c = 1.0f; d = 1.0f;
		boolA = true; boolB = true; boolC = true; boolD = true; return;
	}
	else if (*value == "min") {
		a = (float)UI_WVALUE_MIN; b = (float)UI_WVALUE_MIN; c = (float)UI_WVALUE_MIN; d = (float)UI_WVALUE_MIN;
		boolA = false; boolB = false; boolC = false; boolD = false; return;
	}
	else if (*value == "max") {
		a = (float)UI_WVALUE_MAX; b = (float)UI_WVALUE_MAX; c = (float)UI_WVALUE_MAX; d = (float)UI_WVALUE_MAX;
		boolA = false; boolB = false; boolC = false; boolD = false; return;
	}
	else if (*value == "noreport") {
		a = (float)UI_WVALUE_NOREPORT; b = (float)UI_WVALUE_NOREPORT; c = (float)UI_WVALUE_NOREPORT; d = (float)UI_WVALUE_NOREPORT;
		boolA = false; boolB = false; boolC = false; boolD = false; return;
	}
	// Assume that this function always receives 4 values in the string, and contains no other characters
	std::string modString = *value;
	std::string checkString = modString;
	std::string eval;
	eval = splitRegularString(modString, ",");
	evaluateRelativeStyleProperty(a, boolA, &eval);
	b = a; boolB = boolA;
	if (modString == checkString) { return; }
	else { checkString = modString; }
	evaluateRelativeStyleProperty(c, boolC, &modString);
	d = c; boolD = boolC;
}
void WidgetStyle::evaluateQuadRelativeProperty(float& a, float& b, float& c, float& d, bool& boolA, bool& boolB, bool& boolC, bool& boolD, std::string* value)
{
	if (*value == "inherit") {
		a = (float)UI_WVALUE_INHERIT; b = (float)UI_WVALUE_INHERIT; c = (float)UI_WVALUE_INHERIT; d = (float)UI_WVALUE_INHERIT;
		boolA = false; boolB = false; boolC = false; boolD = false; return;
	}
	else if (*value == "fill") {
		a = 1.0f; b = 1.0f; c = 1.0f; d = 1.0f;
		boolA = true; boolB = true; boolC = true; boolD = true; return;
	}
	else if (*value == "min") {
		a = (float)UI_WVALUE_MIN; b = (float)UI_WVALUE_MIN; c = (float)UI_WVALUE_MIN; d = (float)UI_WVALUE_MIN;
		boolA = false; boolB = false; boolC = false; boolD = false; return;
	}
	else if (*value == "max") {
		a = (float)UI_WVALUE_MAX; b = (float)UI_WVALUE_MAX; c = (float)UI_WVALUE_MAX; d = (float)UI_WVALUE_MAX;
		boolA = false; boolB = false; boolC = false; boolD = false; return;
	}
	else if (*value == "noreport") {
		a = (float)UI_WVALUE_NOREPORT; b = (float)UI_WVALUE_NOREPORT; c = (float)UI_WVALUE_NOREPORT; d = (float)UI_WVALUE_NOREPORT;
		boolA = false; boolB = false; boolC = false; boolD = false; return;
	}
	// Assume that this function always receives 4 values in the string, and contains no other characters
	std::string modString = *value;
	std::string checkString = modString;
	std::string eval;
	eval = splitRegularString(modString, ",");
	evaluateRelativeStyleProperty(a, boolA, &eval);
	if (modString == checkString) { return; }
	else { checkString = modString; }
	eval = splitRegularString(modString, ",");
	evaluateRelativeStyleProperty(b, boolB, &eval);
	if (modString == checkString) { return; }
	else { checkString = modString; }
	eval = splitRegularString(modString, ",");
	evaluateRelativeStyleProperty(c, boolC, &eval);
	if (modString == checkString) { return; }
	else { checkString = modString; }
	evaluateRelativeStyleProperty(d, boolD, &modString);
}
// Evaluate Color Property
void WidgetStyle::evaluateColorStyleProperty(CColor& backgroundColor, float& alpha, std::string* value)
{
	if (*value == "inherit") { backgroundColor.setUniformly((float)UI_WVALUE_INHERIT); alpha = UI_WVALUE_INHERIT; return; }
	if (*value == "max" || *value == "fill") { backgroundColor.setUniformly(1.0f); alpha = 1.0f; return; }
	std::string modString = *value;
	CColor checkColor = findColor(modString);
	if (!isNullColor(checkColor)) { modString = checkColor.makeString(); }
	std::string checkString = modString;
	// Assume that this function always receives 4 values in the string, and contains no other characters
	backgroundColor.r = std::stof(splitRegularString(modString, ","));
	if (modString == checkString) { return; }
	else { checkString = modString; }
	backgroundColor.g = std::stof(splitRegularString(modString, ","));
	if (modString == checkString) { return; }
	else { checkString = modString; }
	backgroundColor.b = std::stof(splitRegularString(modString, ","));
	if (modString == checkString) { return; }
	else { checkString = modString; }
	alpha = std::stof(modString);
}
// Evaluate Special/Named Property Types
void WidgetStyle::evaluatePositionProperty(int& position, std::string* value)
{
	if (*value == "inherit") { position = UI_WVALUE_INHERIT; return; }
	else if (*value == "noreport") { position = UI_WVALUE_NOREPORT; return; }
	else if (*value == "static") { position = UI_POSITION_STATIC; return; }
	else if (*value == "absolute") { position = UI_POSITION_ABSOLUTE; return; }
	else if (*value == "relative") { position = UI_POSITION_RELATIVE; return; }
	else if (*value == "fixed") { position = UI_POSITION_FIXED; return; }
	else { position = UI_POSITION_STATIC; }
}
void WidgetStyle::evaluateAnchoringProperty(int& anchoring, std::string* value)
{
	if (*value == "inherit") { anchoring = UI_WVALUE_INHERIT; return; }
	else if (*value == "no-report") { anchoring = UI_WVALUE_NOREPORT; return; }
	else if (*value == "none") { anchoring = UI_ANCHORING_NONE; return; }
	else if (*value == "upper-left") { anchoring = UI_ANCHORING_UPPERLEFT; return; }
	else if (*value == "upper-center") { anchoring = UI_ANCHORING_UPPERCENTER; return; }
	else if (*value == "upper-right") { anchoring = UI_ANCHORING_UPPERRIGHT; return; }
	else if (*value == "center-left") { anchoring = UI_ANCHORING_CENTERLEFT; return; }
	else if (*value == "center") { anchoring = UI_ANCHORING_CENTER; return; }
	else if (*value == "center-right") { anchoring = UI_ANCHORING_CENTERRIGHT; return; }
	else if (*value == "lower-left") { anchoring = UI_ANCHORING_LOWERLEFT; return; }
	else if (*value == "lower-center") { anchoring = UI_ANCHORING_LOWERCENTER; return; }
	else if (*value == "lower-right") { anchoring = UI_ANCHORING_LOWERRIGHT; return; }
	else { anchoring = UI_ANCHORING_NONE; }
}
void WidgetStyle::evaluateFloatingProperty(int& floating, std::string* value)
{
	if (*value == "inherit") { floating = UI_WVALUE_INHERIT; return; }
	else if (*value == "noreport") { floating = UI_WVALUE_NOREPORT; return; }
	else if (*value == "left") { floating = UI_FLOATING_LEFT; return; }
	else if (*value == "center") { floating = UI_FLOATING_CENTER; return; }
	else if (*value == "right") { floating = UI_FLOATING_RIGHT; return; }
	else { floating = UI_FLOATING_LEFT; }
}
void WidgetStyle::evaluateVisibilityProperty(int& visibility, std::string* value)
{
	if (*value == "inherit") { visibility = UI_WVALUE_INHERIT; return; }
	else if (*value == "noreport") { visibility = UI_WVALUE_NOREPORT; return; }
	else if (*value == "hidden" || *value == "hide" || *value == "false") { visibility = UI_VISIBILITY_HIDE; return; }
	else if (*value == "visible" || *value == "show" || *value == "true") { visibility = UI_VISIBILITY_SHOW; return; }
	else { visibility = UI_VISIBILITY_NONE; }
}
void WidgetStyle::evaluateBoolProperty(bool& prop, std::string* value, bool defaultValue)
{
	if (*value == "true") { prop = true; return; }
	else if (*value == "false") { prop = false; return; }
	else if (value->size() < 5) { prop = true; return; }
	else { prop = defaultValue; }
}
void WidgetStyle::evaluateOverflowProperty(int& overflow, std::string* value)
{
	if (*value == "inherit") { overflow = UI_WVALUE_INHERIT; return; }
	else if (*value == "noreport") { overflow = UI_WVALUE_NOREPORT; return; }
	else if (*value == "visible") { overflow = UI_OVERFLOW_VISIBLE; return; }
	else if (*value == "hidden") { overflow = UI_OVERFLOW_HIDDEN; return; }
	else if (*value == "scroll") { overflow = UI_OVERFLOW_SCROLL; return; }
	else if (*value == "auto") { overflow = UI_OVERFLOW_AUTO; return; }
	else { overflow = UI_OVERFLOW_NONE; }
}
void WidgetStyle::evaluateDoubleOverflowProperty(int& overflowX, int& overflowY, std::string* value)
{
	std::string modString = *value;
	std::string xValue;
	std::string yValue;
	// Assume that this function always receives 2 values in the string, and contains no other characters
	xValue = splitRegularString(modString, ",");
	yValue = modString;
	// Evaluate the two strings
	if (xValue == "inherit") { overflowX = UI_WVALUE_INHERIT; }
	else if (xValue == "noreport") { overflowX = UI_WVALUE_NOREPORT; }
	else if (xValue == "visible") { overflowX = UI_OVERFLOW_VISIBLE; }
	else if (xValue == "hidden") { overflowX = UI_OVERFLOW_HIDDEN; }
	else if (xValue == "scroll") { overflowX = UI_OVERFLOW_SCROLL; }
	else if (xValue == "auto") { overflowX = UI_OVERFLOW_AUTO; }
	else { overflowX = UI_OVERFLOW_NONE; }
	if (yValue == "inherit") { overflowY = UI_WVALUE_INHERIT; }
	else if (yValue == "noreport") { overflowY = UI_WVALUE_NOREPORT; }
	else if (yValue == "visible") { overflowY = UI_OVERFLOW_VISIBLE; }
	else if (yValue == "hidden") { overflowY = UI_OVERFLOW_HIDDEN; }
	else if (yValue == "scroll") { overflowY = UI_OVERFLOW_SCROLL; }
	else if (yValue == "auto") { overflowY = UI_OVERFLOW_AUTO; }
	else { overflowY = UI_OVERFLOW_NONE; }
}