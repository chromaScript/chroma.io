#ifndef CSTYLE_H
#define CSTYLE_H

#include "../../cscript/CForward.h"
#include "../../cstyle/SEnums.h"
#include "../../cstyle/SForward.h"
#include "../../math/Color.h"

#include <string>
#include <vector>
#include <filesystem>
#include <map>

// Widget Event Types
#define UI_WEVENT_NULL 0
#define UI_WEVENT_CANCEL 1
#define UI_WEVENT_ENTRY 2

// Numeric Property Values for reporting desired size to parent object
#define UI_WVALUE_INHERIT -9 // Defer to parent
#define UI_WVALUE_FILL -8
#define UI_WVALUE_MIN -7 // Not currently in use
#define UI_WVALUE_MAX -6 // Not currently in use
#define UI_WVALUE_NOREPORT -5 // Defer to children
#define UI_WVALUE_NONE 2147483647

// Anchoring Property Values
#define UI_ANCHORING_NONE -1 // Should not be used in execution, widgets should always have an anchoring value
#define UI_ANCHORING_UPPERLEFT 0
#define UI_ANCHORING_UPPERCENTER 1
#define UI_ANCHORING_UPPERRIGHT 2
#define UI_ANCHORING_CENTERLEFT 3
#define UI_ANCHORING_CENTER 4
#define UI_ANCHORING_CENTERRIGHT 5
#define UI_ANCHORING_LOWERLEFT 6
#define UI_ANCHORING_LOWERCENTER 7
#define UI_ANCHORING_LOWERRIGHT 8

// Position Property Values
#define UI_POSITION_STATIC 0 // The default value
#define UI_POSITION_ABSOLUTE 1
#define UI_POSITION_RELATIVE 2
#define UI_POSITION_FIXED 3

// Arrangement Property Values
#define UI_FLOATING_NONE -1
#define UI_FLOATING_LEFT 0
#define UI_FLOATING_CENTER 1
#define UI_FLOATING_RIGHT 2

// Rendering Property Values
#define UI_OVERFLOW_NONE -1
#define UI_OVERFLOW_VISIBLE 0
#define UI_OVERFLOW_HIDDEN 1
#define UI_OVERFLOW_SCROLL 2
#define UI_OVERFLOW_AUTO 3
#define UI_OVERFLOW_BLOCK 4
#define UI_OVERFLOW_AUTOBLOCK 5

// Visibility Property Values
#define UI_VISIBILITY_NONE -1
#define UI_VISIBILITY_HIDE 0
#define UI_VISIBILITY_SHOW 1

// Format Property Values
#define UI_TEXTFORMAT_DEFAULT 0
#define UI_TEXTFORMAT_NONE 0
#define UI_TEXTFORMAT_PERCENT_WHOLE 1
#define UI_TEXTFORMAT_PERCENT_ALL 2
#define UI_TEXTFORMAT_PERCENT_1D 3
#define UI_TEXTFORMAT_PERCENT_2D 4
#define UI_TEXTFORMAT_PERCENT_3D 5
#define UI_TEXTFORMAT_PERCENT_4D 6
#define UI_TEXTFORMAT_PERCENT_5D 7
#define UI_TEXTFORMAT_PERCENT_6D 8
#define UI_TEXTFORMAT_NUMBER_WHOLE 20
#define UI_TEXTFORMAT_NUMBER_ALL 21
#define UI_TEXTFORMAT_NUMBER_1D 22
#define UI_TEXTFORMAT_NUMBER_2D 23
#define UI_TEXTFORMAT_NUMBER_3D 24
#define UI_TEXTFORMAT_NUMBER_4D 25
#define UI_TEXTFORMAT_NUMBER_5D 26
#define UI_TEXTFORMAT_NUMBER_6D 27
#define UI_TEXTFORMAT_DEGREE_WHOLE 30
#define UI_TEXTFORMAT_DEGREE_ALL 31
#define UI_TEXTFORMAT_DEGREE_1D 32
#define UI_TEXTFORMAT_DEGREE_2D 33
#define UI_TEXTFORMAT_DEGREE_3D 34
#define UI_TEXTFORMAT_DEGREE_4D 35
#define UI_TEXTFORMAT_DEGREE_5D 36
#define UI_TEXTFORMAT_DEGREE_6D 37
#define UI_TEXTFORMAT_UPPERCASE 81
#define UI_TEXTFORMAT_LOWERCASE 82

struct CBorder
{
	int left;
	int right;
	int top;
	int bottom;
	CBorder()
	{
		left = UI_WVALUE_NONE;
		right = UI_WVALUE_NONE;
		top = UI_WVALUE_NONE;
		bottom = UI_WVALUE_NONE;
	}
	CBorder(int top, int bottom, int left, int right)
	{
		this->left = left;
		this->right = right;
		this->top = top;
		this->bottom = bottom;
	}
	void clear()
	{
		left = UI_WVALUE_NONE;
		right = UI_WVALUE_NONE;
		top = UI_WVALUE_NONE;
		bottom = UI_WVALUE_NONE;
	}
};

// WOffset is used to condense the left/right/top/bottom offsets
struct COffset
{
	int left;
	int right;
	int top;
	int bottom;
	COffset()
	{
		left = UI_WVALUE_NONE;
		right = UI_WVALUE_NONE;
		top = UI_WVALUE_NONE;
		bottom = UI_WVALUE_NONE;
	}
	COffset(int left, int right, int top, int bottom)
	{
		this->left = left;
		this->right = right;
		this->top = top;
		this->bottom = bottom;
	}
	void clear()
	{
		left = UI_WVALUE_NONE;
		right = UI_WVALUE_NONE;
		top = UI_WVALUE_NONE;
		bottom = UI_WVALUE_NONE;
	}
};

// WMargin is used to hold widget-specific border information
// Uses the same struct signature as WBorder, but under a different name so as to avoid cross contamination
// Margin dictates space between a child widget and parent container, but ignores sibling widgets
struct CMargin
{
	int left;
	int right;
	int top;
	int bottom;
	CMargin()
	{
		left = UI_WVALUE_NONE;
		right = UI_WVALUE_NONE;
		top = UI_WVALUE_NONE;
		bottom = UI_WVALUE_NONE;
	}
	CMargin(int top, int bottom, int left, int right)
	{
		this->left = left;
		this->right = right;
		this->top = top;
		this->bottom = bottom;

	}
	void clear()
	{
		left = UI_WVALUE_NONE;
		right = UI_WVALUE_NONE;
		top = UI_WVALUE_NONE;
		bottom = UI_WVALUE_NONE;
	}
};

// WidgetStyle is a struct for holding name-associated style data read from .style (similar to .css) files
// When the Layout (similar to .xml) parser encounters a "style" declaration, it will reference structs of this type
class WidgetStyle : public std::enable_shared_from_this<WidgetStyle>
{
public:
	std::string name; // Name of the style as read from the .style file
	float minX; // Absolute minimum widget size // 
	float maxX; // Absolute maximum widget size // These all allow relative values to take a %
	float minY; // Absolute minimum widget size // of the parent widget's size.
	float maxY; // Absolute maximum widget size //
	bool relMinX;
	bool relMaxX;
	bool relMinY;
	bool relMaxY; // Boolean vector to set whether to read min/max/X/Y as relative or absolute
	float innerRadius;
	float outerRadius;
	int position;
	COffset offset;
	CBorder border; // Additional spacing between sibling widgets (Interpreted additively, adjacent borders are added)
	CMargin margin; // Spacing from the parent widget's bounds, considered as part of widget's requested size
	int anchoring; // Nine point anchor system for what is considered the widget's 0,0 coordinate
	int floating; // To what side of a container should the widget prefer to occupy. Left float has right of way
	CColor fillColor; // Primary color for text, also used as a tint or fill on images when fillAlpha != 0
	float fillAlpha;
	CColor focusColor; // Color of text when text is selected
	CColor highlightColor; // Color of background when text is selected
	CColor backgroundColor; // Background color for widget if not reporting a texture
	std::filesystem::path backgroundTexture; // Texture path for the widget
	float backgroundAlpha; // Alpha value for the background color, does not affect a texture currently
	CColor boxShadowColor;
	float boxShadowAlpha;
	int boxShadowSizeX;
	int boxShadowSizeY;
	bool overrideClipping; // Whether rendering should ignore overflow settings
	unsigned int zIndex;
	int visibility; // Used to show/hide widgets by disabling the render call. Does not accept inheritance, always hides children
	int overflowX; // Whether overflow should be clipped to the widget in rendering
	int overflowY; // Whether overflow should be clipped to the widget in rendering
	std::filesystem::path fontPath;
	int fontSize;
	int textFormat;
	std::map<unsigned char, CColor> vertexColor;
	// Constructor
	WidgetStyle();
	//
	std::shared_ptr<WidgetStyle> makeCopy();
	//
	SToken stringToPropertyType(std::string name);
	bool isSizeProperty(std::string name);
	// So as to return 0 if the value is INT_MIN
	CBorder getBorder();
	CMargin getMargin();
	COffset getOffset();
	void resetProperty(std::shared_ptr<CInterpreter> interpreter,
		std::string name,
		WidgetStyle origStyle);
	std::shared_ptr<CObject> getSetProperty(
		std::shared_ptr<CInterpreter> interpreter, 
		std::string name, 
		std::shared_ptr<CObject> obj,
		bool isGet);
	std::shared_ptr<CObject> getSetProperty(
		std::shared_ptr<CInterpreter> interpreter,
		SToken name,
		std::shared_ptr<CObject> obj,
		bool isGet);
	std::shared_ptr<CObject> getSetProperty(
		std::shared_ptr<CInterpreter> interpreter,
		SToken name,
		std::string fieldValue,
		bool isGet);
	void mergeStyle(std::shared_ptr<WidgetStyle> style);
	// Style Property Evaluator Functions
	std::string propertyEnumToString(std::string propSelect, int p);
	// Evaluate integer-type properties
	void evaluateNumericStyleProperty(unsigned int& prop, std::string* value);
	void evaluateNumericStyleProperty(int& prop, std::string* value);
	void evaluateNumericStyleProperty(float& prop, std::string* value);
	void evaluateDoubleNumericProperty(int& a, int& b, std::string* value);
	void evaluateQuadNumericProperty(int& a, int& b, int& c, int& d, std::string* value);
	// Evaluate float-type properties, handling for % and 0.0-1.0 values
	void evaluateRelativeStyleProperty(float& prop, bool& makeRelative, std::string* value);
	void evaluateDoubleRelativeStyleProperty(float& a, float& b, float& c, float& d, bool& boolA, bool& boolB, bool& boolC, bool& boolD, std::string* value);
	void evaluateQuadRelativeProperty(float& a, float& b, float& c, float& d, bool& boolA, bool& boolB, bool& boolC, bool& boolD, std::string* value);
	// Evaluate Color Property
	void evaluateColorStyleProperty(CColor& backgroundColor, float& alpha, std::string* value);
	// Evaluate Special/Named Property Types
	void evaluatePositionProperty(int& position, std::string* value);
	void evaluateAnchoringProperty(int& anchoring, std::string* value);
	void evaluateFloatingProperty(int& floating, std::string* value);
	void evaluateVisibilityProperty(int& visibility, std::string* value);
	void evaluateTextFormatProperty(int& position, std::string* value);
	void evaluateOverflowProperty(int& overflow, std::string* value);
	void evaluateDoubleOverflowProperty(int& overflowX, int& overflowY, std::string* value);
	void evaluateBoolProperty(bool& prop, std::string* value, bool defaultValue);
};

#endif // !CSTYLE_H
