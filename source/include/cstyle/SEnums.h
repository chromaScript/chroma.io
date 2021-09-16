#ifndef SENUMS_H
#define SENUMS_H

#include <string>
#include <vector>
#include <map>

enum class STokenType
{
	// Literals
	NIL, IDENTIFIER, STRING, BOOL, NUM,

	// Scripts
	SCRIPT_TAG, SCRIPT,

	// Syntax bits
	CURLY_LEFT, CURLY_RIGHT,
	PAREN_LEFT, PAREN_RIGHT,
	SQUARE_LEFT, SQUARE_RIGHT,
	ANGLE_LEFT, ANGLE_RIGHT,
	COLON, SEMICOLON, COMMA, AND,
	SCOPE,

	// Selectors
	DOT, POUND, WILDCARD,

	// Property Names/Types
	BOUNDS,
	WIDTH, HEIGHT,
	MIN_WIDTH, MIN_HEIGHT, MAX_WIDTH, MAX_HEIGHT,
	POSITION, LEFT, RIGHT, TOP, BOTTOM,
	ANCHOR, FLOAT, VISIBILITY, ZINDEX,
	$OVERFLOW, OVERFLOW_X, OVERFLOW_Y,
	CLIPPING, DIMENSIONS,
	INNER_RADIUS, OUTER_RADIUS,
	VERTEX_COLOR,
	VERTEX_COLOR_RED,
	VERTEX_COLOR_GREEN,
	VERTEX_COLOR_BLUE,
	VERTEX_COLOR_ALPHA,
	FILL_COLOR,
	FILL_COLOR_RED,
	FILL_COLOR_GREEN,
	FILL_COLOR_BLUE,
	FILL_COLOR_ALPHA,
	FOCUS_COLOR,
	FOCUS_COLOR_RED,
	FOCUS_COLOR_GREEN,
	FOCUS_COLOR_BLUE,
	FOCUS_COLOR_ALPHA,
	HIGHLIGHT_COLOR,
	HIGHLIGHT_COLOR_RED,
	HIGHLIGHT_COLOR_GREEN,
	HIGHLIGHT_COLOR_BLUE,
	HIGHLIGHT_COLOR_ALPHA,
	BACKGROUND_COLOR,
	BACKGROUND_COLOR_RED,
	BACKGROUND_COLOR_GREEN,
	BACKGROUND_COLOR_BLUE,
	BACKGROUND_COLOR_ALPHA,
	BACKGROUND_TEXTURE,
	BOX_SHADOW_COLOR,
	BOX_SHADOW_COLOR_RED,
	BOX_SHADOW_COLOR_GREEN,
	BOX_SHADOW_COLOR_BLUE,
	BOX_SHADOW_COLOR_ALPHA,
	BOX_SHADOW_SIZE,
	BOX_SHADOW_SIZE_X,
	BOX_SHADOW_SIZE_Y,
	BORDER, BORDER_TOP, BORDER_BOTTOM, BORDER_LEFT, BORDER_RIGHT,
	MARGIN, MARGIN_TOP, MARGIN_BOTTOM, MARGIN_LEFT, MARGIN_RIGHT,
	FONT_PATH, FONT_SIZE, TEXT_FORMAT,


	// Named Property Values
	INHERIT, FILL, MIN, MAX, NOREPORT, 
	_ABSOLUTE, _RELATIVE, FIXED,
	UPPER_LEFT, UPPER_CENTER, UPPER_RIGHT,
	CENTER_LEFT, CENTER, CENTER_RIGHT,
	LOWER_LEFT, LOWER_CENTER, LOWER_RIGHT,
	VISIBILE, SHOW, HIDDEN, HIDE,
	SCROLL, AUTO,
	NONE, DEFAULT, UPPERCASE, LOWERCASE,
	PERCENT_WHOLE, PERCENT_ALL, PERCENT_1D, PERCENT_2D, PERCENT_3D, PERCENT_4D, PERCENT_5D, PERCENT_6D,
	NUMBER_WHOLE, NUMBER_ALL, NUMBER_1D, NUMBER_2D, NUMBER_3D, NUMBER_4D, NUMBER_5D, NUMBER_6D,
	DEGREE_WHOLE, DEGREE_ALL, DEGREE_1D, DEGREE_2D, DEGREE_3D, DEGREE_4D, DEGREE_5D, DEGREE_6D,

	END
};

inline std::vector<std::string> STokenTypeNames =
{
	// Literals
	"nil", "identifier", "string", "bool", "num",

	// Scripts
	"script tag", "script",

	// Syntax bits
	"{", "}",
	"(", ")",
	"[", "]",
	"<", ">",
	":", ";", ",", "&",
	"::",

	// Selectors
	".", "#", "*",

	// Property Names/Types
	"bounds",
	"width", "height",
	"min-width", "min-height", "max-width", "max-height",
	"position", "left", "right", "top", "bottom",
	"anchor", "float", "visibility", "z-index",
	"overflow", "overflow-X", "overflow-Y",
	"clipping", "dimensions",
	"inner-radius", "outer-radius",
	"vertex-color",
	"vertex-color-red",
	"vertex-color-green",
	"vertex-color-blue",
	"vertex-color-alpha",
	"fill-color",
	"fill-color-red",
	"fill-color-green",
	"fill-color-blue",
	"fill-color-alpha",
	"focus-color",
	"focus-color-red",
	"focus-color-green",
	"focus-color-blue",
	"focus-color-alpha",
	"highlight-color",
	"highlight-color-red",
	"highlight-color-green",
	"highlight-color-blue",
	"highlight-color-alpha",
	"background-color",
	"background-color-red",
	"background-color-green",
	"background-color-blue",
	"background-color-alpha",
	"background-texture",
	"box-shadow-color",
	"box-shadow-color-red",
	"box-shadow-color-green",
	"box-shadow-color-blue",
	"box-shadow-color-alpha",
	"box-shadow-size",
	"box-shadow-size-X",
	"box-shadow-size-Y",
	"border", "border-top", "border-bottom", "border-left", "border-right",
	"margin", "margin-top", "margin-bottom", "margin-left", "margin-right",
	"font-path", "font-size", "text-format",

	// Named Property Values
	"inherit", "fill", "min", "max", "noreport",
	"absolute", "relative", "fixed",
	"upper-left", "upper-center", "upper-right",
	"center-left", "center", "center-right",
	"lower-left", "lower-center", "lower-right",
	"visible", "show", "hidden", "hide",
	"scroll", "auto",
	"none", "default", "uppercase", "lowercase",
	"percent-whole", "percent-all", "percent-1d", "percent-2d", "percent-3d", "percent-4d", "percent-5d", "percent-6d",
	"number-whole", "number-all", "number-1d", "number-2d", "number-3d", "number-4d", "number-5d", "number-6d",
	"degree-whole", "degree-all", "degree-1d", "degree-2d", "degree-3d", "degree-4d", "degree-5d", "degree-6d",

	"end"
};

inline std::map<std::string, STokenType> styleTypeMap;

#endif