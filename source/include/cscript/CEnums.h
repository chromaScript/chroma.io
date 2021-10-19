#ifndef CENUMS_H
#define CENUMS_H

#include <string>
#include <vector>
#include <map>

enum class CTokenType
{
// Single-Character Tokens
	// Enclosures
	PAREN_LEFT, PAREN_RIGHT, CURLY_LEFT, CURLY_RIGHT, SQUARE_LEFT, SQUARE_RIGHT,
	// Member Access
	DOT,
	// Seperator
	COMMA, COLON,
	// Explicit Terminator
	SEMICOLON,

// Operators
	// Single Character
	PLUS, MINUS, DIVIDE, MULTIPLY, MODULO, FALSIFY, NEGATE, VALIDATE,
	// Two characters
	ASSIGN, ASSIGN_ADD, ASSIGN_SUBTRACT, ASSIGN_DIVIDE, ASSIGN_MULTIPLY,
	INCREMENT, DECREMENT,
	POWER, // **
	SCOPE, // ::

	// Conditionals
	MORE, MORE_EQUAL, LESS, LESS_EQUAL, EQUAL, NOT_EQUAL,
	LOGIC_AND, LOGIC_OR, APPROXIMATE, // && // || // ~=

// Literals
IDENTIFIER, _THIS, NIL,

// Keywords
	// Variable Types
	ARRAY, NUM, VECTOR2, VECTOR3, VECTOR4, STRING, BOOL, _VOID, FUNCTION, ANY,
	// Array Types
	ANY_ARRAY, NUM_ARRAY, VECTOR2_ARRAY, VECTOR3_ARRAY, VECTOR4_ARRAY,
	STRING_ARRAY, BOOL_ARRAY, VOID_ARRAY, FUNCTION_ARRAY, IDENTIFIER_ARRAY,
	// Flow Control
	IF, ELSE, FOR, WHILE, DO, FOREACH, BREAK, CONTINUE, TRY, CATCH,
	// Functions
	NAMESPACE, RETURN,
	// Classes
	CLASS, INHERIT, CONSTRUCTOR, CAST,
	// Reserved Names
		// Standard Function Namespace
		PRINT, SUPER, BIND, _DELETE,
		// Standard Class Literals
		STYLE, WIDGET, UI, APP,

		END
};

inline std::string CTokenTypeNames[] = {
// Single-Character Tokens
	// Enclosures
	"(", ")", "{", "}", "[", "]",
	// Member Access
	".",
	// Seperator
	",", ":",
	// Explicit Terminator
	";",

// Operators
	// Single Character
	"+", "-", "/", "*", "%", "!", "-", "?",
	// Two characters
	"=", "+=", "-=", "/=", "*=",
	"++", "--",
	"**", // **
	"::", // ::

	// Conditionals
	">", ">=", "<", "<=", "==", "!=",
	"&&", "||", "~=",

// Literals
"identifier", "this", "nil",

// Keywords
	// Variable Types
	"array", "num", "vec2", "vec3", "vec4", "string", "bool", "void", "function", "any",
	// Array Types
	"array any", "array num", "array vec2", "array vec3", "array vec4",
	"array string", "array bool", "array void", "array function", "array identifier",
	// Flow Control
	"if", "else", "for", "while", "do", "foreach", "break", "continue", "try", "catch",
	// Functions
	"namespace", "return",
	// Classes
	"class", "inherit", "constructor", "cast",
	// Reserved Names
		// Standard Function Namespace
		"print", "super", "bind", "delete",
		"Style", "Widget", "Ui", "App",

		"end"
};

enum class CLiteralTypes
{
	_CNil,
	_CClass,
	_CInstance,
	_CInstance_Array,
	_CFunction,
	_CFunction_Array,
	_CThis,
	_CNumber,
	_CNumber_Array,
	_CVec2,
	_CVec2_Array,
	_CVec3,
	_CVec3_Array,
	_CVec4,
	_CVec4_Array,
	_CString,
	_CString_Array,
	_CBool,
	_CBool_Array,
	_CApp,
	_CUi,
	_CToolbox,
	_CWidget,
	_CStyle,
};

enum class CCallableTypes
{
	_CUndefined,
	_CClass,
	_CClassInstance,
	_CFunction,
	// Internal Built-In Widget Functions
	_CInt_TextClick,
	_CInt_TGraph_Drag,
	_CInt_TGraph_Hover,
	// Standard Lib Utility Functions
	_CStd_fClock,
	_CStd_fVecSize,
	_CStd_fToString,
	_CStd_fToNum,
	_CStd_fKeyToString,
	_CStd_fKeybindToString,
	_CStd_fMakeKeySig,
	_CStd_fBindTimerEvent,
	_CStd_fCancelTimerEvent,
	_CStd_fBindCallbackEvent,
	_CStd_fUnbindCallbackEvent,
	_CStd_fTriggerStoredEvent,
	// Standard Lib Collision Functions
	_CStd_fGetMousePos,
	_CStd_fGetMousePos_relative,
	_CStd_fHitTestWidget_byID,
	_CStd_fSweepTestRoot_byID,
	// Standard Lib Math Functions
	_CStd_fFibonacci,
	_CStd_fLineIntersect2D,
	_CStd_fFloor,
	_CStd_fClamp,
	_CStd_fLerp,
	_CStd_fRGB_toHSL,
	_CStd_fHSL_toRGB,
	_CStd_fHueByDegree_HSL,
	_CStd_fSaturationByPercentDegree_HSL,
	_CStd_fValueByPercent_HSL,
	// Standard Lib Global Widget Operator Functions
	_CStd_fGetParent,
	_CStd_fGetRoot,
	_CStd_fSetProperty,
	_CStd_fGetProperty,
	_CStd_fSetChildProperty_byName,
	_CStd_fSetChildProperty_byID,
	_CStd_fSetChildProperty_byType,
	_CStd_fGetChildProperty_byType,
	_CStd_fSetChildProperty_byClass,
	_CStd_fSetClassProperty,
	_CStd_fSetClassProperty_excludingActive,
	_CStd_fSetClassProperty_excludingID,
	_CStd_fResetProperty,
	_CStd_fResetClassProperty,
	// App Class & Functions
	_CStd_cApp,
	_CStd_cfIsValidKeybind_Tool,
	_CStd_cfIsValidKeybind_AlphaOnly,
	_CStd_cfIsValidKeybind_SymbolOrChar,
	_CStd_cfIsValidKeybind_ModOnly,
	_CStd_cfIsValidKeybind_Menu,
	_CStd_cfExit,
	_CStd_cfSaveBMP,
	_CStd_cfGetWindowSize,
	_CStd_cfGetWindowPosition,
	_CStd_cfMaximize,
	_CStd_cfMinimize,
	_CStd_cfSetCursor,
	_CStd_cfMoveAppWindow,
	_CStd_cfResizeAppWindow,
	_CStd_cfZoomExtents,
	// Ui Class & functions
	_CStd_cUi,
	_CStd_cfCheckWidgetIDTable,
	_CStd_cfMoveRootToFront,
	_CStd_cfSetActivePopup,
	_CStd_cfClearPopup,
	_CStd_cfPreventBlurCallback,
	_CStd_cfPreventFocusCallback,
	_CStd_cfSortTargetWidgetChildren_byMacro,
	_CStd_cfSetFocus_byID,
	_CStd_cfClearFocus,
	_CStd_cfGetWidget_byID,
	_CStd_cfDeleteWidget_byID,
	_CStd_cfSetFGColor_HSL,
	_CStd_cfSetBGColor_HSL,
	_CStd_cfResetFGBGColor,
	_CStd_cfSwapFGBGColor,
	_CStd_cfCreateNewDocument,
	_CStd_cfCloseDocument,
	// Document Class & Functions
	_CStd_cDocument,
	// Toolbox Class & Functions
	_CStd_cToolbox,
	_CStd_cfCheckValidIOCombination,
	_CStd_cfCheckValidControlScheme,
	_CStd_cfCreateNewTool,
	_CStd_cfCheckActiveToolSettingsMask,
	_CStd_cfSetActiveToolProp,
	_CStd_cfGetActiveToolProp,
	_CStd_cfSetActiveToolInterest,
	_CStd_cfGetActiveToolInterest,
	_CStd_cfExportAllTools,
	// Select Class & Functions
	_CStd_cSelect,
	// Effect Class & Functions
	_CStd_cEffect,
	// Canvas Class & Functions
	_CStd_cCanvas,
	// Layer Class & Functions
	_CStd_cLayer,
	// Plugin Class & Functions
	_CStd_cPlugin,
	// Widget Class & Functions
	_CStd_cWidget,
	_CStd_cfRebuildOutofBoundsWidgets,
	_CStd_cfSetProperty,
	_CStd_cfSetChildProperty_byClass,
	_CStd_cfGetChildrenProperties,
	_CStd_cfGetProperty,
	_CStd_cfSetPosition,
	_CStd_cfSetChildOrder_byID,
	_CStd_cfAddChildWidget_Prototype,
	_CStd_cfRemoveChildWidget_byID,
	_CStd_cfRemoveChildWidgets_byClass,
	_CStd_cfBindActiveToolSetting,
	_CStd_cfCallSpecialFunction,
	// Cursor Class & Functions
	_CStd_cCursor,
	// Stylus Class & Functions
	_CStd_cStylus,
	// Style Class & Functions
	_CStd_cStyle,
	_CStd_cfSetProperty_byName,
	_CStd_cfGetProperty_byName,
};

enum class CCallbackType
{
	storedEventListener,
	keyListenerAll_blocking,
	keyListenerAll,
	keyListener_blocking,
	keyListener,
	updateFGColor,
	updateBGColor,
	colorpickStart,
	colorpickMove,
	colorpickEnd,
	canvasRightClickPress,
	toolSwitch,
	null
};

inline std::vector<std::string> CCallbackNames = {
	"storedEventListener",
	"keyListenerAll_blocking",
	"keyListenerAll",
	"keyListener_blocking",
	"keyListener",
	"updateFGColor",
	"updateBGColor",
	"colorpickStart",
	"colorpickMove",
	"colorpickEnd",
	"canvasRightClickPress",
	"toolSwitch",
	"null"
};

inline std::map<std::string, CCallbackType> CCallbackMap;

inline CCallbackType stringToCCallbackType(std::string query)
{
	for (std::string name : CCallbackNames)
	{
		if (query == name)
		{
			return CCallbackMap.at(name);
		}
	}
	return CCallbackType::null;
}

#endif