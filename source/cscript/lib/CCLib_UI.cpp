#include "../../include/cscript/CCallable.h"
#include "../../include/cscript/CCallable_Lib.h"
#include "../../include/cscript/lib/CCLib_UI.h"

#include "../../include/cscript/ChromaScript.h"
#include "../../include/cscript/CForward.h"
#include "../../include/cscript/CLiteral.h"
#include "../../include/cscript/CExpr.h"
#include "../../include/cscript/CStmt.h"
#include "../../include/cscript/CEnvironment.h"
#include "../../include/cscript/CInterpreter.h"
#include "../../include/cscript/CEnums.h"
#include "../../include/clayout/LEnums.h"
#include "../../include/cscript/CObject.h"
#include "../../include/cscript/CToken.h"
#include "../../include/entities/WidgetStyle.h"

#include "../../include/Application.h"
#include "../../include/entities/UserInterface.h"
#include "../../include/Toolbox.h"
#include "../../include/entities/Widget.h"
#include "../../include/structs.h"
#include "../../include/keys.h"
#include <glm.hpp>

#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include <map>

extern std::shared_ptr<Application> chromaIO;

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_Ui
//
////////////////////////////////////////////////////////////////////////////////////////////////

CStd_cUi::CStd_cUi(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app)
{
	this->name = "Ui";
	this->type = CCallableTypes::_CStd_cUi;
	this->classEnv = classEnv;
	this->superClass = nullptr;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CStmt>> methods;
	this->classEnv.get()->define(
		"checkWidgetIDTable",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCheckWidgetIDTable, classEnv.get()->lookupEnvironment("checkWidgetIDTable", true)));
	this->classEnv.get()->define(
		"moveRootToFront",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfMoveRootToFront, classEnv.get()->lookupEnvironment("moveRootToFront", true)));
	this->classEnv.get()->define(
		"preventBlurCallback",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfPreventBlurCallback, classEnv.get()->lookupEnvironment("preventBlurCallback", true)));
	this->classEnv.get()->define(
		"preventFocusCallback",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfPreventFocusCallback, classEnv.get()->lookupEnvironment("preventFocusCallback", true)));
	this->classEnv.get()->define(
		"setActivePopup",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetActivePopup, classEnv.get()->lookupEnvironment("setActivePopup", true)));
	this->classEnv.get()->define(
		"clearPopup",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfClearPopup, classEnv.get()->lookupEnvironment("clearPopup", true)));
	this->classEnv.get()->define(
		"setFocus_byID",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetFocus_byID, classEnv.get()->lookupEnvironment("setFocus_byID", true)));
	this->classEnv.get()->define(
		"clearFocus",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfClearFocus, classEnv.get()->lookupEnvironment("clearFocus", true)));
	this->classEnv.get()->define(
		"getWidget_byID",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetWidget_byID, classEnv.get()->lookupEnvironment("getWidget_byID", true)));
	this->classEnv.get()->define(
		"deleteWidget_byID",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfDeleteWidget_byID, classEnv.get()->lookupEnvironment("deleteWidget_byID", true)));
	this->classEnv.get()->define(
		"sortTargetWidgetChildren_byMacro",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSortTargetWidgetChildren_byMacro, classEnv.get()->lookupEnvironment("sortTargetWidgetChildren_byMacro", true)));
	this->classEnv.get()->define(
		"setFGColor_HSL",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetFGColor_HSL, classEnv.get()->lookupEnvironment("setFGColor_HSL", true)));
	this->classEnv.get()->define(
		"setBGColor_HSL",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetBGColor_HSL, classEnv.get()->lookupEnvironment("setBGColor_HSL", true)));
	this->classEnv.get()->define(
		"resetFGBGColor",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfResetFGBGColor, classEnv.get()->lookupEnvironment("resetFGBGColor", true)));
	this->classEnv.get()->define(
		"swapFGBGColor",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSwapFGBGColor, classEnv.get()->lookupEnvironment("swapFGBGColor", true)));
	this->classEnv.get()->define(
		"createNewDocument",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCreateNewDocument, classEnv.get()->lookupEnvironment("createNewDocument", true)));
	this->classEnv.get()->define(
		"closeDocument",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCloseDocument, classEnv.get()->lookupEnvironment("closeDocument", true)));
	for (auto const& item : this->classEnv.get()->values)
	{
		std::shared_ptr<CStmt> func = std::get<std::shared_ptr<CFunction>>(item.second.get()->obj).get()->funcDeclaration;
		methods.push_back(func);
	}

	// Build the properties
	std::vector<std::shared_ptr<CStmt>> properties;
	// Raw UI Literal Object (Hidden using @)
	this->classEnv.get()->define("@uiObj", std::make_shared<CObject>(app.get()->getUI()));

	// Construct the pseudo-CStmt_Class object
	this->classDeclaration = std::make_shared<CStmt_Class>(
		scopeStack,	// scopeStack (empty)
		std::make_shared<CToken>(CTokenType::IDENTIFIER, this->name, -1),	// name
		nullptr,	// superClass
		false,		// isDeclarationOnly
		nullptr,	// constructor
		methods,	// methods
		properties	// properties
		);
}
std::string CStd_cUi::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Ui Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_cfCheckWidgetIDTable
CStd_cfCheckWidgetIDTable::CStd_cfCheckWidgetIDTable(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCheckWidgetIDTable;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "checkWidgetIDTable", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCheckWidgetIDTable::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Call getWidget_byID on the UI
	std::weak_ptr<UI> ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj);
	if (!ui.expired())
	{
		if (ui.lock()->widgetIDTable.count(id) == 0) { return std::make_shared<CObject>(true); }
	}
	return std::make_shared<CObject>(false);
}
std::string CStd_cfCheckWidgetIDTable::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfMoveRootToFront
CStd_cfMoveRootToFront::CStd_cfMoveRootToFront(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfMoveRootToFront;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "moveRootToFront", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfMoveRootToFront::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string & Arg 1 as Function
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Check that the widget id is valid
	UI* ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get();
	std::weak_ptr<Widget> widget = ui->getWidgetByID(id);
	if (!widget.expired())
	{
		ui->moveRootToFront(widget.lock().get()->getRoot());
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfMoveRootToFront::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetActivePopup
CStd_cfSetActivePopup::CStd_cfSetActivePopup(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetActivePopup;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::FUNCTION, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	paramsNames.push_back("isBlocking");
	paramsNames.push_back("escapeCallback");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setActivePopup", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetActivePopup::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string & Arg 1 as Function
	std::string id = std::get<std::string>(args[0].get()->obj);
	bool isBlocking = std::get<bool>(args[1].get()->obj);
	std::shared_ptr<CFunction> callback = std::get<std::shared_ptr<CFunction>>(args[2].get()->obj);
	// Check that the widget id is valid
	UI* ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get();
	std::weak_ptr<Widget> widget = ui->getWidgetByID(id);
	if (!widget.expired())
	{
		ui->putActivePopupWidget(widget, isBlocking, callback);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetActivePopup::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfClearPopup
CStd_cfClearPopup::CStd_cfClearPopup(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfClearPopup;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "clearPopup", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfClearPopup::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Get the UI
	UI* ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get();
	std::weak_ptr<Widget> widget = ui->getWidgetByID(id);
	if (!widget.expired())
	{
		ui->clearPopupWidget(widget);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfClearPopup::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfPreventBlurCallback
CStd_cfPreventBlurCallback::CStd_cfPreventBlurCallback(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetFocus_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "preventBlurCallback", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfPreventBlurCallback::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the UI
	std::weak_ptr<UI> ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj);
	if (!ui.expired())
	{
		ui.lock().get()->interruptBlur = true;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfPreventBlurCallback::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfPreventFocusCallback
CStd_cfPreventFocusCallback::CStd_cfPreventFocusCallback(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfPreventFocusCallback;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "preventFocusCallback", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfPreventFocusCallback::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the UI
	std::weak_ptr<UI> ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj);
	if (!ui.expired())
	{
		ui.lock().get()->interruptFocus = true;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfPreventFocusCallback::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetFocus_byID
CStd_cfSetFocus_byID::CStd_cfSetFocus_byID(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetFocus_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setFocus_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetFocus_byID::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Call getWidget_byID on the UI
	std::weak_ptr<UI> ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj);
	std::weak_ptr<Widget> widget = ui.lock().get()->getWidgetByID(id);
	if (!widget.expired())
	{
		ui.lock().get()->updateFocusWidget(widget);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetFocus_byID::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfClearFocus
CStd_cfClearFocus::CStd_cfClearFocus(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfClearFocus;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "clearFocus", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfClearFocus::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Call getWidget_byID on the UI
	std::weak_ptr<UI> ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj);
	ui.lock().get()->clearFocusWidget();
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfClearFocus::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetWidget_byID
CStd_cfGetWidget_byID::CStd_cfGetWidget_byID(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetChildProperty_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "Widget", -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildProperty_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetWidget_byID::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Call getWidget_byID on the UI
	std::weak_ptr<Widget> widget = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get()->getWidgetByID(id);
	if (!widget.expired())
	{
		std::vector<std::shared_ptr<CObject>> instanceArgs;
		std::shared_ptr<CObject> callee = interpreter.get()->currentEnvironment.get()->get({ "global" }, "Widget");
		instanceArgs.push_back(callee);
		instanceArgs.push_back(args[0]);
		std::shared_ptr<CObject> returnObj = std::get<std::shared_ptr<CClass>>(callee.get()->obj).get()->call(interpreter, &instanceArgs);
		std::get<std::shared_ptr<CInstance>>(returnObj.get()->obj).get()->instanceEnv.get()->get("@widgetObj").get()->obj = widget;
		return returnObj;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfGetWidget_byID::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfDeleteWidget_byID
CStd_cfDeleteWidget_byID::CStd_cfDeleteWidget_byID(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfDeleteWidget_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "deleteWidget_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfDeleteWidget_byID::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string id = std::get<std::string>(args[0].get()->obj);
	// Call getWidget_byID on the UI
	bool result = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get()->deleteWidget_byID(interpreter, id);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfDeleteWidget_byID::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSortTargetWidgetChildren_byMacro
CStd_cfSortTargetWidgetChildren_byMacro::CStd_cfSortTargetWidgetChildren_byMacro(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSortTargetWidgetChildren_byMacro;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	paramsNames.push_back("sortProperty");
	paramsNames.push_back("macroIDName");
	paramsNames.push_back("setVisible");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "sortTargetWidgetChildren_byMacro", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSortTargetWidgetChildren_byMacro::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Unpack arguments
	std::string id = std::get<std::string>(args[0].get()->obj);
	std::string sortProperty = std::get<std::string>(args[1].get()->obj);
	std::string macroIDName = std::get<std::string>(args[2].get()->obj);
	bool setVisible = std::get<bool>(args[3].get()->obj);
	// Call the function
	bool result = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get()->sortTargetWidgetChildren(
			interpreter, id, sortProperty, macroIDName, setVisible);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfSortTargetWidgetChildren_byMacro::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetFGColor_HSL
CStd_cfSetFGColor_HSL::CStd_cfSetFGColor_HSL(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetFGColor_HSL;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("hueDegree");
	paramsNames.push_back("satPercent");
	paramsNames.push_back("valPercent");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setFGColor_HSL", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetFGColor_HSL::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the values
	double degree = abs(std::get<double>(args[0].get()->obj));
	float satP = clampf(abs((float)std::get<double>(args[1].get()->obj)), 0.0f, 1.0f);
	float valP = clampf(abs((float)std::get<double>(args[2].get()->obj)), 0.0f, 1.0f);
	CColor hsl = HSL_toRGB(glm::dvec3(degree / 360.0, satP, valP));
	std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get()->updateFGColor(hsl, 0, 0);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetFGColor_HSL::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetBGColor_HSL
CStd_cfSetBGColor_HSL::CStd_cfSetBGColor_HSL(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetBGColor_HSL;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("hueDegree");
	paramsNames.push_back("satPercent");
	paramsNames.push_back("valPercent");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "Widget", -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setBGColor_HSL", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetBGColor_HSL::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the values
	double degree = abs(std::get<double>(args[0].get()->obj));
	float satP = clampf(abs((float)std::get<double>(args[1].get()->obj)), 0.0f, 1.0f);
	float valP = clampf(abs((float)std::get<double>(args[2].get()->obj)), 0.0f, 1.0f);
	CColor hsl = HSL_toRGB(glm::dvec3(degree / 360.0, satP, valP));
	std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get()->updateBGColor(hsl, 0, 0);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetBGColor_HSL::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfResetFGBGColor
CStd_cfResetFGBGColor::CStd_cfResetFGBGColor(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfResetFGBGColor;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "resetFGBGColor", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfResetFGBGColor::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the values
	std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get()->resetFGBGColor(black, white);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfResetFGBGColor::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSwapFGBGColor
CStd_cfSwapFGBGColor::CStd_cfSwapFGBGColor(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSwapFGBGColor;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "swapFGBGColor", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSwapFGBGColor::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Set the values
	std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get()->swapFGBGColor();
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSwapFGBGColor::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfCreateNewDocument
CStd_cfCreateNewDocument::CStd_cfCreateNewDocument(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCreateNewDocument;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	paramsNames.push_back("width");
	paramsNames.push_back("height");
	paramsNames.push_back("setAsActive");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "createNewDocument", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCreateNewDocument::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	int width = (int)std::round(std::get<double>(args[1].get()->obj));
	int height = (int)std::round(std::get<double>(args[2].get()->obj));
	bool setActive = std::get<bool>(args[3].get()->obj);
	// Get the UI
	UI* ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get();
	if (ui != nullptr)
	{
		ui->newDocument(name, width, height, setActive);
		return std::make_shared<CObject>(true);
	}
	return std::make_shared<CObject>(false);
}
std::string CStd_cfCreateNewDocument::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfCreateNewDocument
CStd_cfCloseDocument::CStd_cfCloseDocument(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCloseDocument;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	paramsNames.push_back("ueid");
	paramsNames.push_back("closeActive");
	paramsNames.push_back("saveBeforeExit");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "closeDocument", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCloseDocument::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	int UEID = (int)std::round(std::get<double>(args[1].get()->obj));
	bool closeActive = std::get<bool>(args[2].get()->obj);
	bool saveBeforeExit = std::get<bool>(args[3].get()->obj);
	// Get the UI
	UI* ui = std::get<std::shared_ptr<UI>>(
		interpreter.get()->currentEnvironment.get()->values.at("@uiObj").get()->obj).get();
	if (ui != nullptr)
	{
		return std::make_shared<CObject>(ui->closeDocument(name, UEID, closeActive, saveBeforeExit));
	}
	return std::make_shared<CObject>(false);
}
std::string CStd_cfCloseDocument::toString() { return funcDeclaration.get()->name.get()->lexeme; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_Document
//
////////////////////////////////////////////////////////////////////////////////////////////////

CStd_cDocument::CStd_cDocument(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app)
{
	//this->classDeclaration = classDeclaration;
	//this->name = name;
	//this->type = type;
	//this->classEnv = classEnv;
	//this->superClass = superClass;
}
std::string CStd_cDocument::toString() { return name; }