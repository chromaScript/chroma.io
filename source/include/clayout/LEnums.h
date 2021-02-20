#ifndef LENUMS_H
#define LENUMS_H

#include <string>
#include <vector>
#include <map>

enum class LTokenType
{
	// Literals
	NIL, IDENTIFIER, STRING, BOOL, NUM,

	// Syntax bits
	ANGLE_LEFT, ANGLE_RIGHT, CLOSE, EQUAL,

	// Root Attrib Types
	DEFAULT_POSITION,
	SCRIPT,

	// Property Names/Types
	ID,
	CLASS,
	NAME,
	STYLE,
	VALUE,
	IMG,
	GROUP,
	INNER_CONTENT,
	ON_BLUR,
	ON_FOCUS,
	ON_CLICK,
	ON_RIGGHTCLICK,
	ON_DBL_CLICK,
	ON_DBL_RIGHTCLICK,
	ON_MIDDLECLICK,
	ON_DBL_MIDDLECLICK,
	ON_DRAG,
	ON_DRAG_START,
	ON_DRAG_END,
	ON_DRAG_ENTER,
	ON_DRAG_OVER,
	ON_DRAG_LEAVE,
	ON_DROP,
	ON_KEY_DOWN,
	ON_KEY_PRESS,
	ON_KEY_UP,
	ON_LOAD,
	ON_COPY,
	ON_PASTE,
	ON_CUT,
	ON_MOUSE_OVER,
	ON_MOUSE_ENTER,
	ON_MOUSE_LEAVE,
	ON_MOUSE_WHEELUP,
	ON_MOUSE_WHEELDOWN,
	ON_RELEASE,
	ON_RESIZE,
	DRAGGABLE,
	DROPPABLE,
	DRAG_TYPE,
	DROP_TYPE,

	// Widget Names/Types
	ROOT,
	PROTO,
	H_BOX,
	V_BOX,
	TILE_BOX,
	COLOR_BOX,
	HUE_STRIP,
	HUE_RADIUS,
	GRADIENT_BOX,
	LIST,
	LIST_ITEM,
	PANEL,
	IMAGE,
	TEXT,
	TEXT_ENTRY,

	END
};

inline std::vector<std::string> LTokenTypeNames =
{
	// Literals
	"nil", "identifier", "string", "bool", "num",

	// Syntax bits
	"<", ">", "/", "=",

	// Root Attrib Types
	"defaultPosition",
	"script",

	// Property Names/Types
	"id",
	"class",
	"name",
	"style",
	"value",
	"img",
	"group",
	"innerContent",
	"onblur",
	"onfocus",
	"onclick",
	"onrightclick",
	"ondblclick",
	"ondblrightclick",
	"onmiddleclick",
	"ondblmiddleclick",
	"ondrag",
	"ondragstart",
	"ondragend",
	"ondragenter",
	"ondragover",
	"ondragleave",
	"ondrop",
	"onkeydown",
	"onkeypress",
	"onkeyup",
	"onload",
	"oncopy",
	"onpaste",
	"oncut",
	"onmouseover",
	"onmouseenter",
	"onmouseleave",
	"onmousewheeldown",
	"onmousewheelup",
	"onrelease",
	"onresize",
	"draggable",
	"droppable",
	"dragType",
	"dropType",

	// Widget Names/Types
	"ROOT",
	"PROTO",
	"H_BOX",
	"V_BOX",
	"TILE_BOX",
	"COLOR_BOX",
	"HUE_STRIP",
	"HUE_RADIUS",
	"GRADIENT_BOX",
	"LIST",
	"LIST_ITEM",
	"PANEL",
	"IMAGE",
	"TEXT",
	"TEXT_ENTRY",

	"end"
};

inline std::map<std::string, LTokenType> layoutTypeMap;

#endif