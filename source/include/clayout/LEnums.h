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
	ON_CANCEL,
	ON_ENTRY,
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
	LINE,
	BLOCK,
	PARAGRAPH,
	TEXT_ENTRY,
	T_GRAPH,
	R_GRAPH,
	C_GRAPH,
	EDIT_GRADIENT,
	T_NOISE,

	END
};

inline std::map<std::string, LTokenType> layoutStringTypeMap = {
	{ "nil", LTokenType::NIL },
	{ "identifier", LTokenType::IDENTIFIER },
	{ "string", LTokenType::STRING },
	{ "bool", LTokenType::BOOL },
	{"num",  LTokenType::NUM },

	{ "<", LTokenType::ANGLE_LEFT },
	{ ">", LTokenType::ANGLE_RIGHT, },
	{ "/", LTokenType::CLOSE },
	{ "=", LTokenType::EQUAL },

	{ "defaultPosition", LTokenType::DEFAULT_POSITION },
	{ "script", LTokenType::SCRIPT },

	{ "id", LTokenType::ID },
	{ "class", LTokenType::CLASS },
	{ "name", LTokenType::NAME },
	{ "style", LTokenType::STYLE },
	{ "value", LTokenType::VALUE },
	{ "img", LTokenType::IMG },
	{ "group", LTokenType::GROUP },
	{ "innerContent", LTokenType::INNER_CONTENT },
	{ "onblur", LTokenType::ON_BLUR },
	{ "onfocus", LTokenType::ON_FOCUS },
	{ "onclick", LTokenType::ON_CLICK },
	{ "onrightclick", LTokenType::ON_RIGGHTCLICK, },
	{ "ondblclick", LTokenType::ON_DBL_CLICK },
	{ "ondblrightclick", LTokenType::ON_DBL_RIGHTCLICK },
	{ "onmiddleclick", LTokenType::ON_MIDDLECLICK },
	{ "ondblmiddleclick", LTokenType::ON_DBL_MIDDLECLICK },
	{ "ondrag", LTokenType::ON_DRAG },
	{ "ondragstart", LTokenType::ON_DRAG_START },
	{ "ondragend", LTokenType::ON_DRAG_END },
	{ "ondragenter", LTokenType::ON_DRAG_ENTER },
	{ "ondragover", LTokenType::ON_DRAG_OVER },
	{ "ondragleave", LTokenType::ON_DRAG_LEAVE },
	{ "ondrop", LTokenType::ON_DROP },
	{ "onkeydown", LTokenType::ON_KEY_DOWN },
	{ "onkeypress", LTokenType::ON_KEY_PRESS },
	{ "onkeyup", LTokenType::ON_KEY_UP },
	{ "onload", LTokenType::ON_LOAD },
	{ "oncopy", LTokenType::ON_COPY },
	{ "onpaste", LTokenType::ON_PASTE },
	{ "oncut", LTokenType::ON_CUT },
	{ "onmouseover", LTokenType::ON_MOUSE_OVER },
	{ "onmouseenter", LTokenType::ON_MOUSE_ENTER },
	{ "onmouseleave", LTokenType::ON_MOUSE_LEAVE },
	{ "onmousewheeldown", LTokenType::ON_MOUSE_WHEELUP },
	{ "onmousewheelup", LTokenType::ON_MOUSE_WHEELDOWN },
	{ "onrelease", LTokenType::ON_RELEASE },
	{ "onresize", LTokenType::ON_RESIZE },
	{ "oncancel", LTokenType::ON_CANCEL },
	{ "onentry", LTokenType::ON_ENTRY },
	{ "draggable", LTokenType::DRAGGABLE },
	{ "droppable", LTokenType::DROPPABLE },
	{ "dragType", LTokenType::DRAG_TYPE },
	{ "dropType", LTokenType::DROP_TYPE },

	{ "ROOT", LTokenType::ROOT, },
	{ "PROTO", LTokenType::PROTO },
	{ "H_BOX", LTokenType::H_BOX },
	{ "V_BOX", LTokenType::V_BOX },
	{ "TILE_BOX", LTokenType::TILE_BOX, },
	{ "COLOR_BOX", LTokenType::COLOR_BOX },
	{ "HUE_STRIP", LTokenType::HUE_STRIP },
	{ "HUE_RADIUS", LTokenType::HUE_RADIUS },
	{ "GRADIENT_BOX", LTokenType::GRADIENT_BOX },
	{ "LIST", LTokenType::LIST },
	{ "LIST_ITEM", LTokenType::LIST_ITEM, },
	{ "PANEL", LTokenType::PANEL },
	{ "IMAGE", LTokenType::IMAGE },
	{ "TEXT", LTokenType::TEXT },
	{ "LINE", LTokenType::LINE },
	{ "BLOCK", LTokenType::BLOCK, },
	{ "PARAGRAPH", LTokenType::PARAGRAPH, },
	{ "TEXT_ENTRY", LTokenType::TEXT_ENTRY },
	{ "T_GRAPH", LTokenType::T_GRAPH },
	{ "R_GRAPH", LTokenType::R_GRAPH },
	{ "C_GRAPH", LTokenType::C_GRAPH },
	{ "EDIT_GRADIENT", LTokenType::EDIT_GRADIENT },
	{ "T_NOISE", LTokenType::T_NOISE },

	{ "end", LTokenType::END }
};

inline std::map<LTokenType, std::string> layoutTypeStringMap = {
	{ LTokenType::NIL, "nil" },
	{LTokenType::IDENTIFIER, "identifier" },
	{LTokenType::STRING, "string" },
	{LTokenType::BOOL, "bool" },
	{LTokenType::NUM, "num" },

	{LTokenType::ANGLE_LEFT, "<" },
	{LTokenType::ANGLE_RIGHT, ">" },
	{LTokenType::CLOSE, "/"  },
	{ LTokenType::EQUAL, "=" },

	{LTokenType::DEFAULT_POSITION, "defaultPosition" },
	{LTokenType::SCRIPT, "script" },

	{LTokenType::ID, "id" },
	{LTokenType::CLASS, "class" },
	{LTokenType::NAME, "name" },
	{LTokenType::STYLE, "style" },
	{LTokenType::VALUE, "value" },
	{LTokenType::IMG , "img" },
	{LTokenType::GROUP, "group" },
	{LTokenType::INNER_CONTENT, "innerContent" },
	{ LTokenType::ON_BLUR, "onblur" },
	{LTokenType::ON_FOCUS, "onfocus"  },
	{LTokenType::ON_CLICK, "onclick" },
	{LTokenType::ON_RIGGHTCLICK, "onrightclick" },
	{ LTokenType::ON_DBL_CLICK , "ondblclick" },
	{LTokenType::ON_DBL_RIGHTCLICK, "ondblrightclick" },
	{LTokenType::ON_MIDDLECLICK, "onmiddleclick" },
	{LTokenType::ON_DBL_MIDDLECLICK, "ondblmiddleclick" },
	{LTokenType::ON_DRAG, "ondrag" },
	{LTokenType::ON_DRAG_START, "ondragstart" },
	{LTokenType::ON_DRAG_END, "ondragend" },
	{LTokenType::ON_DRAG_ENTER, "ondragenter" },
	{LTokenType::ON_DRAG_OVER, "ondragover" },
	{LTokenType::ON_DRAG_LEAVE, "ondragleave" },
	{LTokenType::ON_DROP, "ondrop" },
	{LTokenType::ON_KEY_DOWN, "onkeydown" },
	{LTokenType::ON_KEY_PRESS, "onkeypress" },
	{LTokenType::ON_KEY_UP, "onkeyup" },
	{LTokenType::ON_LOAD, "onload" },
	{LTokenType::ON_COPY, "oncopy" },
	{LTokenType::ON_PASTE, "onpaste" },
	{LTokenType::ON_CUT, "oncut" },
	{LTokenType::ON_MOUSE_OVER, "onmouseover" },
	{LTokenType::ON_MOUSE_ENTER, "onmouseenter" },
	{LTokenType::ON_MOUSE_LEAVE, "onmouseleave" },
	{LTokenType::ON_MOUSE_WHEELUP, "onmousewheeldown" },
	{LTokenType::ON_MOUSE_WHEELDOWN, "onmousewheelup" },
	{LTokenType::ON_RELEASE, "onrelease", },
	{LTokenType::ON_RESIZE, "onresize" },
	{LTokenType::ON_CANCEL, "oncancel" },
	{LTokenType::ON_ENTRY, "onentry" },
	{LTokenType::DRAGGABLE, "draggable" },
	{LTokenType::DROPPABLE, "droppable" },
	{LTokenType::DRAG_TYPE, "dragType" },
	{LTokenType::DROP_TYPE, "dropType" },

	{LTokenType::ROOT, "ROOT" },
	{LTokenType::PROTO, "PROTO" },
	{LTokenType::H_BOX, "H_BOX" },
	{LTokenType::V_BOX, "V_BOX" },
	{LTokenType::TILE_BOX, "TILE_BOX" },
	{LTokenType::COLOR_BOX, "COLOR_BOX" },
	{LTokenType::HUE_STRIP, "HUE_STRIP" },
	{LTokenType::HUE_RADIUS, "HUE_RADIUS" },
	{LTokenType::GRADIENT_BOX, "GRADIENT_BOX" },
	{LTokenType::LIST, "LIST" },
	{LTokenType::LIST_ITEM, "LIST_ITEM" },
	{LTokenType::PANEL, "PANEL" },
	{LTokenType::IMAGE, "IMAGE" },
	{LTokenType::TEXT, "TEXT" },
	{LTokenType::LINE, "LINE" },
	{LTokenType::BLOCK, "BLOCK" },
	{LTokenType::PARAGRAPH, "PARAGRAPH" },
	{LTokenType::TEXT_ENTRY, "TEXT_ENTRY" },
	{LTokenType::T_GRAPH, "T_GRAPH" },
	{LTokenType::R_GRAPH, "R_GRAPH" },
	{LTokenType::C_GRAPH, "C_GRAPH" },
	{LTokenType::EDIT_GRADIENT, "EDIT_GRADIENT" },
	{LTokenType::T_NOISE, "T_NOISE" },

	{LTokenType::END, "end" },
};

#endif