#include "../include/cscript/CObject.h"
#include "../include/cscript/CForward.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CCallable.h"
#include "../include/cscript/CCallable_Lib.h"
#include "../include/cscript/CStmt.h"
#include "../include/cscript/CExpr.h"
#include "../include/cscript/CToken.h"
#include "../include/cscript/CLiteral.h"
#include "../include/entities/WidgetStyle.h"

#include "../include/Application.h"
#include "../include/entities/UserInterface.h"
#include "../include/Toolbox.h"
class Widget;

#include <glm.hpp>

#include <string>
#include <vector>
#include <memory>
#include <variant>

CObject::CObject() {}
CObject::CObject(const CObject& cobject) : objType(cobject.objType)
{
	this->objType = cobject.objType;
	this->objName = cobject.objName;
	this->obj = cobject.obj;
}
// Virtualize Copy Constructor
CObject::CObject(std::shared_ptr<CObject> object)
{
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> newArr;
	this->objName = object.get()->objName;
	this->objType = object.get()->objType;
	switch (object.get()->objType.type)
	{
	case CLiteralTypes::_CBool:
	case CLiteralTypes::_CNumber:
	case CLiteralTypes::_CString:
		this->obj = object.get()->obj;
		break;
	case CLiteralTypes::_CBool_Array:
	case CLiteralTypes::_CNumber_Array:
	case CLiteralTypes::_CString_Array:
		this->obj = newArr;
		for (std::shared_ptr<CObject> item :
			*std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(object.get()->obj))
		{
			std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(this->obj).get()->push_back(item);
		}
		break;
	default:
		this->obj = object.get()->obj;
	}
}
// Empty Type Constructor
CObject::CObject(std::shared_ptr<CToken> typeToken, std::string objName)
{
	this->objType = CLiteral(typeToken);
	this->objName = objName;
}
CObject::CObject(CLiteral type, std::string objName)
{
	this->objType = type;
	this->objName = objName;
}
// All Array Constructor
CObject::CObject(CLiteralTypes type, std::shared_ptr<std::vector<std::shared_ptr<CObject>>> value)
{
	std::string typeName = "";
	std::string objectName = "";
	switch (type)
	{
	case CLiteralTypes::_CBool_Array:
		typeName = objectName = "BOOL_ARRAY"; break;
	case CLiteralTypes::_CNumber_Array:
		typeName = objectName = "NUM_ARRAY"; break;
	case CLiteralTypes::_CString_Array:
		typeName = objectName = "STRING_ARRAY"; break;
	case CLiteralTypes::_CVec2_Array:
		typeName = objectName = "VEC2_ARRAY"; break;
	case CLiteralTypes::_CVec3_Array:
		typeName = objectName = "VEC2_ARRAY"; break;
	case CLiteralTypes::_CVec4_Array:
		typeName = objectName = "VEC2_ARRAY"; break;
	case CLiteralTypes::_CFunction_Array:
		typeName = objectName = "FUNCTION_ARRAY"; break;
	case CLiteralTypes::_CInstance_Array:
		typeName = "IDENTIFIER_ARRAY"; break;
	default:
		typeName = objectName = "VOID_ARRAY"; break;
	}
	this->objType = CLiteral(type, typeName);
	if (objName != "") { this->objName = objectName; }
	this->obj = value;
}
// Numbers
// Direct Number Constructor
CObject::CObject(double value)
{
	this->objType = CLiteral(CLiteralTypes::_CNumber, "NUM");
	this->objName = "NUM";
	this->obj = (value == UI_WVALUE_NONE) ? 0.0 : value;
}

// Strings
// Direct String Constructor
CObject::CObject(std::string value)
{
	this->objType = CLiteral(CLiteralTypes::_CString, "STRING");
	this->objName = "STRING";
	this->obj = std::string(value);
}
CObject::CObject(std::vector<std::string> values)
{
	this->objType = CLiteral(CLiteralTypes::_CString_Array, "STRING_ARRAY");
	this->objName = "STRING_ARRAY";
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> arr = std::make_shared<std::vector<std::shared_ptr<CObject>>>();
	for (std::string value : values)
	{
		arr.get()->push_back(std::make_shared<CObject>(value));
	}
	this->obj = arr;
}

// Booleans
// Direct Bool Constructor
CObject::CObject(bool value)
{
	this->objType = CLiteral(CLiteralTypes::_CBool, "BOOL");
	this->objName = "BOOL";
	this->obj = value;
}

// Vectors
// Direct Vector2 Constructor
CObject::CObject(glm::dvec2 value)
{
	this->objType = CLiteral(CLiteralTypes::_CVec2, "VEC2");
	this->objName = "VEC2";
	this->obj = value;
}
// Direct Vector3 Constructor
CObject::CObject(glm::dvec3 value)
{
	this->objType = CLiteral(CLiteralTypes::_CVec3, "VEC3");
	this->objName = "VEC3";
	this->obj = value;
}
// Direct Vector4 Constructor
CObject::CObject(glm::dvec4 value)
{
	this->objType = CLiteral(CLiteralTypes::_CVec4, "VEC4");
	this->objName = "VEC4";
	this->obj = value;
}
// Direct Application Constructor (Hidden literal type)
CObject::CObject(std::shared_ptr<Application> app)
{
	this->objType = CLiteral(CLiteralTypes::_CApp, "APP");
	this->objName = "APP";
	this->obj = app;
}
// Direct Ui Constructor (Hidden literal type)
CObject::CObject(std::shared_ptr<UI> ui)
{
	this->objType = CLiteral(CLiteralTypes::_CApp, "UI");
	this->objName = "UI";
	this->obj = ui;
}
// Direct Toolbox Constructor (Hidden literal type)
CObject::CObject(std::shared_ptr<Toolbox> toolbox)
{
	this->objType = CLiteral(CLiteralTypes::_CToolbox, "TOOLBOX");
	this->objName = "TOOLBOX";
	this->obj = toolbox;
}
// Direct Widget Constructor (Hidden literal type)
CObject::CObject(std::weak_ptr<Widget> widget)
{
	this->objType = CLiteral(CLiteralTypes::_CWidget, "WIDGET");
	this->objName = "WIDGET";
	this->obj = widget;
}
// Direct Style Constructor (Hidden literal type)
CObject::CObject(std::shared_ptr<WidgetStyle> style)
{
	this->objType = CLiteral(CLiteralTypes::_CStyle, "STYLE");
	this->objName = "STYLE";
	this->obj = style;
}
// Direct Nil Constructor
CObject::CObject(nullptr_t ptr)
{
	this->objType = CLiteral(CLiteralTypes::_CNil, "NIL");
	this->objName = "NIL";
	this->obj = nullptr;
}
//
// Construct User Function
CObject::CObject(std::shared_ptr<CStmt_Function> functionStmt, std::shared_ptr<CEnvironment> funcEnv)
{
	this->objType = CLiteral(CLiteralTypes::_CFunction, functionStmt.get()->name.get()->lexeme);
	this->objName = functionStmt.get()->name.get()->lexeme;
	this->obj = std::make_shared<CUser_Function>(functionStmt, CCallableTypes::_CFunction, funcEnv);
}
// Construct User Class
CObject::CObject(
	std::shared_ptr<CStmt_Class> classStmt, 
	std::string name,
	std::shared_ptr<CEnvironment> classEnv,
	std::shared_ptr<CClass> superClass)
{
	this->objType = CLiteral(CLiteralTypes::_CClass, classStmt.get()->name.get()->lexeme);
	this->objName = name;
	this->obj = std::make_shared<CUser_Class>(classStmt, name, CCallableTypes::_CClass, classEnv, superClass);
}

// CInstance constructors
CObject::CObject(
	std::shared_ptr<CEnvironment> currentEnv,
	std::string instanceName, 
	std::shared_ptr<CClass> classTemplate)
{
	this->objType = CLiteral(CLiteralTypes::_CInstance, classTemplate.get()->name);
	this->objName = instanceName;
	// Copy classTemplate into the new instanceEnv
	std::shared_ptr<CEnvironment> instanceEnv = currentEnv.get()->lookupEnvironment(instanceName, true);
	instanceEnv.get()->copyData(instanceEnv, classTemplate.get()->classEnv);
	std::shared_ptr<CClass> superClass = classTemplate.get()->superClass;
	while (superClass != nullptr)
	{
		std::shared_ptr<CEnvironment> superEnv =
			currentEnv.get()->fetchEnvironment(superClass.get()->name);
		instanceEnv.get()->copyData(instanceEnv, superEnv);
		superClass = superClass.get()->superClass;
	}
	this->obj = std::make_shared<CInstance>(instanceName, instanceEnv);
}
CObject::CObject(
	std::shared_ptr<CEnvironment> currentEnv,
	std::string instanceName,
	std::shared_ptr<CInstance> instanceCopy)
{
	this->objType = CLiteral(CLiteralTypes::_CInstance, instanceCopy.get()->className);
	this->objName = instanceName;
	std::shared_ptr<CEnvironment> instanceEnv = currentEnv.get()->lookupEnvironment(instanceName, true);
	instanceEnv.get()->copyData(instanceEnv, instanceCopy.get()->instanceEnv);
	this->obj = std::make_shared<CInstance>(instanceName, instanceEnv);
}

// Standard Library Non-Trivial Class/Function Constructor
CObject::CObject(CCallableTypes libClassName, std::shared_ptr<CEnvironment> env, std::shared_ptr<Application> app)
{
	switch (libClassName)
	{
	case CCallableTypes::_CInt_TextClick:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "@textClick");
		this->objName = "@textClick";
		this->obj = std::make_shared<CInt_TextClick>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fBindTimerEvent:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "bindTimerEvent");
		this->objName = "bindTimerEvent";
		this->obj = std::make_shared<CStd_fBindTimerEvent>(env, app);
		break;
	case CCallableTypes::_CStd_fCancelTimerEvent:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "cancelTimerEvent");
		this->objName = "cancelTimerEvent";
		this->obj = std::make_shared<CStd_fCancelTimerEvent>(env, app);
		break;
	case CCallableTypes::_CStd_fBindCallbackEvent:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "bindCallbackEvent");
		this->objName = "bindCallbackEvent";
		this->obj = std::make_shared<CStd_fBindCallbackEvent>(env, app);
		break;
	case CCallableTypes::_CStd_cApp:
		this->objType = CLiteral(CLiteralTypes::_CClass, "App");
		this->objName = "App";
		this->obj = std::make_shared<CStd_cApp>(env, app);
		break;
	case CCallableTypes::_CStd_cUi:
		this->objType = CLiteral(CLiteralTypes::_CClass, "Ui");
		this->objName = "Ui";
		this->obj = std::make_shared<CStd_cUi>(env, app);
		break;
	case CCallableTypes::_CStd_cToolbox:
		this->objType = CLiteral(CLiteralTypes::_CClass, "Toolbox");
		this->objName = "Toolbox";
		this->obj = std::make_shared<CStd_cToolbox>(env, app);
		break;
	case CCallableTypes::_CStd_fGetMousePos:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "getMousePos");
		this->objName = "getMousePos";
		this->obj = std::make_shared<CStd_fGetMousePos>(env, app);
		break;
	case CCallableTypes::_CStd_fGetMousePos_relative:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "getMousePos_relative");
		this->objName = "getMousePos_relative";
		this->obj = std::make_shared<CStd_fGetMousePos_relative>(env, app);
		break;
	case CCallableTypes::_CStd_fHitTestWidget_byID:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "hitTestWidget_byID");
		this->objName = "hitTestWidget_byID";
		this->obj = std::make_shared<CStd_fHitTestWidget_byID>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fSweepTestRoot_byID:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "sweepTestRoot_byID");
		this->objName = "sweepTestRoot_byID";
		this->obj = std::make_shared<CStd_fSweepTestRoot_byID>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fSetProperty:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setProperty");
		this->objName = "setProperty";
		this->obj = std::make_shared<CStd_fSetProperty>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fGetProperty:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "getProperty");
		this->objName = "getProperty";
		this->obj = std::make_shared<CStd_fGetProperty>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fSetChildProperty_byClass:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setChildProperty_byClass");
		this->objName = "setChildProperty_byClass";
		this->obj = std::make_shared<CStd_fSetChildProperty_byClass>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fSetChildProperty_byID:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setChildProperty_byID");
		this->objName = "setChildProperty_byID";
		this->obj = std::make_shared<CStd_fSetChildProperty_byID>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fSetChildProperty_byName:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setChildProperty_byName");
		this->objName = "setChildProperty_byName";
		this->obj = std::make_shared<CStd_fSetChildProperty_byName>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fSetChildProperty_byType:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setChildProperty_byType");
		this->objName = "setChildProperty_byType";
		this->obj = std::make_shared<CStd_fSetChildProperty_byType>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fSetClassProperty:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setClassProperty");
		this->objName = "setClassProperty";
		this->obj = std::make_shared<CStd_fSetClassProperty>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fSetClassProperty_excludingActive:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setClassProperty_excludingActive");
		this->objName = "setClassProperty_excludingActive";
		this->obj = std::make_shared<CStd_fSetClassProperty_excludingActive>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fSetClassProperty_excludingID:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setClassProperty_excludingID");
		this->objName = "setClassProperty_excludingID";
		this->obj = std::make_shared<CStd_fSetClassProperty_excludingID>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fResetProperty:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "resetProperty");
		this->objName = "resetProperty";
		this->obj = std::make_shared<CStd_fResetProperty>(env, app.get()->getUI());
		break;
	case CCallableTypes::_CStd_fResetClassProperty:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "resetClassProperty");
		this->objName = "resetClassProperty";
		this->obj = std::make_shared<CStd_fResetClassProperty>(env, app.get()->getUI());
		break;
	}
}
// Standard Library Trivial Class/Function Constructor
CObject::CObject(CCallableTypes libClassName, std::shared_ptr<CEnvironment> env)
{
	switch (libClassName)
	{
	case CCallableTypes::_CStd_cStyle:
		this->objType = CLiteral(CLiteralTypes::_CClass, "Style");
		this->objName = "Style";
		this->obj = std::make_shared<CStd_cStyle>(env);
		break;
	case CCallableTypes::_CStd_cWidget:
		this->objType = CLiteral(CLiteralTypes::_CClass, "Widget");
		this->objName = "Widget";
		this->obj = std::make_shared<CStd_cWidget>(env);
		break;
	case CCallableTypes::_CStd_fClock:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "clock");
		this->objName = "clock";
		this->obj = std::make_shared<CStd_fClock>(env);
		break;
	case CCallableTypes::_CStd_fToString:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "toString");
		this->objName = "toString";
		this->obj = std::make_shared<CStd_fToString>(env);
		break;
	case CCallableTypes::_CStd_fToNum:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "toNum");
		this->objName = "toNum";
		this->obj = std::make_shared<CStd_fToNum>(env);
		break;
	case CCallableTypes::_CStd_fKeyToString:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "keyToString");
		this->objName = "keyToString";
		this->obj = std::make_shared<CStd_fKeyToString>(env);
		break;
	// Math Functions
	case CCallableTypes::_CStd_fLineIntersect2D:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "lineIntersect2D");
		this->objName = "lineIntersect2D";
		this->obj = std::make_shared<CStd_fLineIntersect2D>(env);
		break;
	case CCallableTypes::_CStd_fFibonacci:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "fibonacci");
		this->objName = "fibonacci";
		this->obj = std::make_shared<CStd_fFibonacci>(env);
		break;
	case CCallableTypes::_CStd_fFloor:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "floor");
		this->objName = "floor";
		this->obj = std::make_shared<CStd_fFloor>(env);
		break;
	case CCallableTypes::_CStd_fClamp:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "clamp");
		this->objName = "clamp";
		this->obj = std::make_shared<CStd_fClamp>(env);
		break;
	case CCallableTypes::_CStd_fLerp:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "lerp");
		this->objName = "lerp";
		this->obj = std::make_shared<CStd_fLerp>(env);
		break;
	case CCallableTypes::_CStd_fRGB_toHSL:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "RGB_toHSL");
		this->objName = "RGB_toHSL";
		this->obj = std::make_shared<CStd_fRGB_toHSL>(env);
		break;
	case CCallableTypes::_CStd_fHSL_toRGB:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "HSL_toRGB");
		this->objName = "HSL_toRGB";
		this->obj = std::make_shared<CStd_fHSL_toRGB>(env);
		break;
	case CCallableTypes::_CStd_fHueByDegree_HSL:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "hueByDegree_HSL");
		this->objName = "hueByDegree_HSL";
		this->obj = std::make_shared<CStd_fHueByDegree_HSL>(env);
		break;
	case CCallableTypes::_CStd_fSaturationByPercentDegree_HSL:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "saturationByPercentDegree_HSL");
		this->objName = "saturationByPercentDegree_HSL";
		this->obj = std::make_shared<CStd_fSaturationByPercentDegree_HSL>(env);
		break;
	case CCallableTypes::_CStd_fValueByPercent_HSL:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "valueByPercent_HSL");
		this->objName = "valueByPercent_HSL";
		this->obj = std::make_shared<CStd_fValueByPercent_HSL>(env);
		break;
	// Application Functions
	case CCallableTypes::_CStd_cfExit:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "exit");
		this->objName = "exit";
		this->obj = std::make_shared<CStd_cfExit>(env);
		break;
	case CCallableTypes::_CStd_cfSaveBMP:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "saveBMP");
		this->objName = "saveBMP";
		this->obj = std::make_shared<CStd_cfSaveBMP>(env);
		break;
	case CCallableTypes::_CStd_cfGetWindowSize:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "getWindowSize");
		this->objName = "getWindowSize";
		this->obj = std::make_shared<CStd_cfGetWindowSize>(env);
		break;
	case CCallableTypes::_CStd_cfMinimize:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "minimize");
		this->objName = "minimize";
		this->obj = std::make_shared<CStd_cfMinimize>(env);
		break;
	case CCallableTypes::_CStd_cfMaximize:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "maximize");
		this->objName = "maximize";
		this->obj = std::make_shared<CStd_cfMaximize>(env);
		break;
	case CCallableTypes::_CStd_cfSetCursor:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setCursor");
		this->objName = "setCursor";
		this->obj = std::make_shared<CStd_cfSetCursor>(env);
		break;
	case CCallableTypes::_CStd_cfIsValidKeybind_Tool:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "isValidKeybind_tool");
		this->objName = "isValidKeybind_tool";
		this->obj = std::make_shared<CStd_cfIsValidKeybind_Tool>(env);
		break;
	// Ui Functions
	case CCallableTypes::_CStd_cfPreventBlurCallback:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "preventBlurCallback");
		this->objName = "preventBlurCallback";
		this->obj = std::make_shared<CStd_cfPreventBlurCallback>(env);
		break;
	case CCallableTypes::_CStd_cfPreventFocusCallback:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "preventFocusCallback");
		this->objName = "preventFocusCallback";
		this->obj = std::make_shared<CStd_cfPreventFocusCallback>(env);
		break;
	case CCallableTypes::_CStd_cfMoveRootToFront:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "moveRootToFront");
		this->objName = "moveRootToFront";
		this->obj = std::make_shared<CStd_cfMoveRootToFront>(env);
		break;
	case CCallableTypes::_CStd_cfSetFocus_byID:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setFocus_byID");
		this->objName = "setFocus_byID";
		this->obj = std::make_shared<CStd_cfSetFocus_byID>(env);
		break;
	case CCallableTypes::_CStd_cfClearFocus:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "clearFocus");
		this->objName = "clearFocus";
		this->obj = std::make_shared<CStd_cfClearFocus>(env);
		break;
	case CCallableTypes::_CStd_cfSetActivePopup:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setActivePopup");
		this->objName = "setActivePopup";
		this->obj = std::make_shared<CStd_cfSetActivePopup>(env);
		break;
	case CCallableTypes::_CStd_cfClearPopup:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "clearPopup");
		this->objName = "clearPopup";
		this->obj = std::make_shared<CStd_cfClearPopup>(env);
		break;
	case CCallableTypes::_CStd_cfGetWidget_byID:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "getWidget_byID");
		this->objName = "getWidget_byID";
		this->obj = std::make_shared<CStd_cfGetWidget_byID>(env);
		break;
	case CCallableTypes::_CStd_cfSetFGColor_HSL:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setFGColor_HSL");
		this->objName = "setFGColor_HSL";
		this->obj = std::make_shared<CStd_cfSetFGColor_HSL>(env);
		break;
	case CCallableTypes::_CStd_cfSetBGColor_HSL:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setBGColor_HSL");
		this->objName = "setBGColor_HSL";
		this->obj = std::make_shared<CStd_cfSetBGColor_HSL>(env);
		break;
	// Toolbox Functions
	case CCallableTypes::_CStd_cfCheckValidIOCombination:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "checkValidIOCombination");
		this->objName = "checkValidIOCombination";
		this->obj = std::make_shared<CStd_cfCheckValidIOCombination>(env);
		break;
	case CCallableTypes::_CStd_cfCheckValidControlScheme:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "checkValidControlScheme");
		this->objName = "checkValidControlScheme";
		this->obj = std::make_shared<CStd_cfCheckValidControlScheme>(env);
		break;
	case CCallableTypes::_CStd_cfCreateNewTool:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "createNewTool");
		this->objName = "createNewTool";
		this->obj = std::make_shared<CStd_cfCreateNewTool>(env);
		break;
	case CCallableTypes::_CStd_cfCheckActiveToolSettingsMask:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "checkActiveToolSettingsMask");
		this->objName = "checkActiveToolSettingsMask";
		this->obj = std::make_shared<CStd_cfCheckActiveToolSettingsMask>(env);
		break;
	case CCallableTypes::_CStd_cfSetActiveToolProp:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setActiveToolProp");
		this->objName = "setActiveToolProp";
		this->obj = std::make_shared<CStd_cfSetActiveToolProp>(env);
		break;
	case CCallableTypes::_CStd_cfGetActiveToolProp:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "getActiveToolProp");
		this->objName = "getActiveToolProp";
		this->obj = std::make_shared<CStd_cfGetActiveToolProp>(env);
		break;
		// Widget Functions
	case CCallableTypes::_CStd_cfSetProperty:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setProperty");
		this->objName = "setProperty";
		this->obj = std::make_shared<CStd_cfSetProperty>(env);
		break;
	case CCallableTypes::_CStd_cfGetProperty:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "getProperty");
		this->objName = "getProperty";
		this->obj = std::make_shared<CStd_cfGetProperty>(env);
		break;
	case CCallableTypes::_CStd_cfSetPosition:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setPosition");
		this->objName = "setPosition";
		this->obj = std::make_shared<CStd_cfSetPosition>(env);
		break;
		// Style Functions
	case CCallableTypes::_CStd_cfSetProperty_byName:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setProperty_byName");
		this->objName = "setProperty_byName";
		this->obj = std::make_shared<CStd_cfSetProperty_byName>(env);
		break;
	case CCallableTypes::_CStd_cfGetProperty_byName:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "getProperty_byName");
		this->objName = "getProperty_byName";
		this->obj = std::make_shared<CStd_cfGetProperty_byName>(env);
		break;
	}
}

// Standard Library Class Function Constructors
CObject::CObject(CCallableTypes libClassName, std::shared_ptr<Application> app)
{
	/*
	switch (libClassName)
	{
	case CCallableTypes::_CStd_cfExit:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "exit");
		this->objName = "exit";
		this->obj = std::make_shared<CStd_cfExit>(app);
		break;
	}
	*/
}
CObject::CObject(CCallableTypes libClassName, std::shared_ptr<WidgetStyle> style)
{
	/*
	switch (libClassName)
	{
	case CCallableTypes::_CStd_cfSetProperty_byName:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "setProperty_byName");
		this->objName = "setProperty_byName";
		this->obj = std::make_shared<CStd_cfSetProperty_byName>(style);
		break;
	case CCallableTypes::_CStd_cfGetProperty_byName:
		this->objType = CLiteral(CLiteralTypes::_CFunction, "getProperty_byName");
		this->objName = "getProperty_byName";
		this->obj = std::make_shared<CStd_cfGetProperty_byName>(style);
		break;
	}
	*/
}


