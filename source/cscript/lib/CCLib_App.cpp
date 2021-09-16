#include "../../include/cscript/CCallable.h"
#include "../../include/cscript/CCallable_Lib.h"
#include "../../include/cscript/lib/CCLib_App.h"

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

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_App
//
////////////////////////////////////////////////////////////////////////////////////////////////

CStd_cApp::CStd_cApp(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app)
{
	this->name = "App";
	this->type = CCallableTypes::_CStd_cApp;
	this->classEnv = classEnv;
	this->superClass = nullptr;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CStmt>> methods;
	this->classEnv.get()->define(
		"exit",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfExit, classEnv.get()->lookupEnvironment("exit", true)));
	this->classEnv.get()->define(
		"saveBMP",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSaveBMP, classEnv.get()->lookupEnvironment("saveBMP", true)));
	this->classEnv.get()->define(
		"getWindowSize",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetWindowSize, classEnv.get()->lookupEnvironment("getWindowSize", true)));
	this->classEnv.get()->define(
		"getWindowPosition",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetWindowPosition, classEnv.get()->lookupEnvironment("getWindowPosition", true)));
	this->classEnv.get()->define(
		"minimize",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfMinimize, classEnv.get()->lookupEnvironment("minimize", true)));
	this->classEnv.get()->define(
		"maximize",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfMaximize, classEnv.get()->lookupEnvironment("maximize", true)));
	this->classEnv.get()->define(
		"setCursor",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetCursor, classEnv.get()->lookupEnvironment("setCursor", true)));
	this->classEnv.get()->define(
		"isValidKeybind_tool",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfIsValidKeybind_Tool, classEnv.get()->lookupEnvironment("isValidKeybind_tool", true)));
	this->classEnv.get()->define(
		"isValidKeybind_alphaOnly",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfIsValidKeybind_AlphaOnly, classEnv.get()->lookupEnvironment("isValidKeybind_alphaOnly", true)));
	this->classEnv.get()->define(
		"isValidKeybind_symbolOrChar",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfIsValidKeybind_SymbolOrChar, classEnv.get()->lookupEnvironment("isValidKeybind_symbolOrChar", true)));
	this->classEnv.get()->define(
		"isValidKeybind_modOnly",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfIsValidKeybind_ModOnly, classEnv.get()->lookupEnvironment("isValidKeybind_modOnly", true)));
	this->classEnv.get()->define(
		"moveAppWindow",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfMoveAppWindow, classEnv.get()->lookupEnvironment("moveAppWindow", true)));
	this->classEnv.get()->define(
		"resizeAppWindow",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfResizeAppWindow, classEnv.get()->lookupEnvironment("resizeAppWindow", true)));
	for (auto const& item : this->classEnv.get()->values)
	{
		std::shared_ptr<CStmt> func = std::get<std::shared_ptr<CFunction>>(item.second.get()->obj).get()->funcDeclaration;
		methods.push_back(func);
	}

	// Build the properties
	std::vector<std::shared_ptr<CStmt>> properties;
	// Raw App Literal Object (Hidden using @)
	this->classEnv.get()->define("@appObj", std::make_shared<CObject>(app));

	// Native 'Ui' Wrapper Class Object - Use secure "global" get to ensure good get
	std::shared_ptr<CClass> uiClassTemplate = std::get<std::shared_ptr<CClass>>(classEnv.get()->get({ "global" }, "Ui").get()->obj);
	this->classEnv.get()->define("ui", std::make_shared<CObject>(classEnv, "ui", uiClassTemplate));
	// Native 'Toolbox' Wrapper Class Object - Use secure "global" get to ensure good get
	std::shared_ptr<CClass> toolboxClassTemplate = std::get<std::shared_ptr<CClass>>(classEnv.get()->get({ "global" }, "Toolbox").get()->obj);
	this->classEnv.get()->define("toolbox", std::make_shared<CObject>(classEnv, "toolbox", toolboxClassTemplate));

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
std::string CStd_cApp::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// App Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_cfExit
CStd_cfExit::CStd_cfExit(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfExit;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "exit", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfExit::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->closeApplication();
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfExit::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSaveBMP
CStd_cfSaveBMP::CStd_cfSaveBMP(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSaveBMP;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "saveBMP", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSaveBMP::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	std::shared_ptr<Application> app = std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj);
	app.get()->renderCanvas_toFile(app.get()->ui.get()->activeCanvas);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSaveBMP::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetWindowSize
CStd_cfGetWindowSize::CStd_cfGetWindowSize(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfGetWindowSize;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getWindowSize", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetWindowSize::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	Application* app = std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get();
	// Create the return object
	std::vector<std::shared_ptr<CObject>> returnList;
	returnList.emplace_back(std::make_shared<CObject>((double)app->getWindowWidth()));
	returnList.emplace_back(std::make_shared<CObject>((double)app->getWindowHeight()));
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, std::make_shared<std::vector<std::shared_ptr<CObject>>>(returnList));
}
std::string CStd_cfGetWindowSize::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetWindowPosition
CStd_cfGetWindowPosition::CStd_cfGetWindowPosition(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfGetWindowPosition;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::NUM_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getWindowPosition", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetWindowPosition::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	Application* app = std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get();
	// Create the return object
	std::vector<std::shared_ptr<CObject>> returnList;
	returnList.emplace_back(std::make_shared<CObject>((double)app->getWindowPosition().x));
	returnList.emplace_back(std::make_shared<CObject>((double)app->getWindowPosition().y));
	return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, std::make_shared<std::vector<std::shared_ptr<CObject>>>(returnList));
}
std::string CStd_cfGetWindowPosition::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfMaximize
CStd_cfMaximize::CStd_cfMaximize(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfExit;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "exit", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfMaximize::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->maximizeApplication();
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfMaximize::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfMinimize
CStd_cfMinimize::CStd_cfMinimize(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfExit;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "exit", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfMinimize::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->minimizeApplication();
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfMinimize::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetCursor
CStd_cfSetCursor::CStd_cfSetCursor(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfExit;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setCursor", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetCursor::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->setCursor(convertCursorName(name));
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetCursor::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfIsValidKeybind_Tool
CStd_cfIsValidKeybind_Tool::CStd_cfIsValidKeybind_Tool(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfIsValidKeybind_Tool;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("modKeyBit");
	paramsNames.push_back("glfwKey");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "isValidKeybind_tool", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfIsValidKeybind_Tool::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	int modKey = (int)std::get<double>(args[0].get()->obj);
	int glfwKey = (int)std::get<double>(args[1].get()->obj);
	// Fetch the appObj, always has the same value name
	bool result = std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->isValidKeybind_tool(modKey, glfwKey);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfIsValidKeybind_Tool::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfIsValidKeybind_AlphaOnly
CStd_cfIsValidKeybind_AlphaOnly::CStd_cfIsValidKeybind_AlphaOnly(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfIsValidKeybind_AlphaOnly;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("modKeyBit");
	paramsNames.push_back("glfwKey");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "isValidKeybind_tool", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfIsValidKeybind_AlphaOnly::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	int modKey = (int)std::get<double>(args[0].get()->obj);
	int glfwKey = (int)std::get<double>(args[1].get()->obj);
	// Fetch the appObj, always has the same value name
	bool result = std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->isValidKeybind_alphaOnly(modKey, glfwKey);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfIsValidKeybind_AlphaOnly::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfIsValidKeybind_SymbolOrChar
CStd_cfIsValidKeybind_SymbolOrChar::CStd_cfIsValidKeybind_SymbolOrChar(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfIsValidKeybind_SymbolOrChar;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("modKeyBit");
	paramsNames.push_back("glfwKey");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "isValidKeybind_symbolOrChar", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfIsValidKeybind_SymbolOrChar::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	int modKey = (int)std::get<double>(args[0].get()->obj);
	int glfwKey = (int)std::get<double>(args[1].get()->obj);
	// Fetch the appObj, always has the same value name
	bool result = std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->isValidKeybind_symbolOrChar(modKey, glfwKey);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfIsValidKeybind_SymbolOrChar::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfIsValidKeybind_ModOnly
CStd_cfIsValidKeybind_ModOnly::CStd_cfIsValidKeybind_ModOnly(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfIsValidKeybind_ModOnly;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("modKeyBit");
	paramsNames.push_back("glfwKey");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "isValidKeybind_modOnly", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfIsValidKeybind_ModOnly::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	int modKey = (int)std::get<double>(args[0].get()->obj);
	int glfwKey = (int)std::get<double>(args[1].get()->obj);
	// Fetch the appObj, always has the same value name
	bool result = std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->isValidKeybind_modOnly(modKey, glfwKey);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfIsValidKeybind_ModOnly::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfMoveAppWindow
CStd_cfMoveAppWindow::CStd_cfMoveAppWindow(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfMoveAppWindow;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("xpos");
	paramsNames.push_back("ypos");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "moveAppWindow", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfMoveAppWindow::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	int xpos = (int)std::get<double>(args[0].get()->obj);
	int ypos = (int)std::get<double>(args[1].get()->obj);
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->moveApplication(xpos, ypos);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfMoveAppWindow::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfResizeAppWindow
CStd_cfResizeAppWindow::CStd_cfResizeAppWindow(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfResizeAppWindow;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("width");
	paramsNames.push_back("height");
	paramsNames.push_back("moveWindow");
	paramsNames.push_back("xpos");
	paramsNames.push_back("ypos");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "resizeAppWindow", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfResizeAppWindow::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	int width = (int)std::get<double>(args[0].get()->obj);
	int height = (int)std::get<double>(args[1].get()->obj);
	int moveWindow = (int)std::get<double>(args[2].get()->obj);
	int xpos = (int)std::get<double>(args[3].get()->obj);
	int ypos = (int)std::get<double>(args[4].get()->obj);
	// Fetch the appObj, always has the same value name
	std::get<std::shared_ptr<Application>>(
		interpreter.get()->currentEnvironment.get()->values.at("@appObj").get()->obj).get()->resizeApplication(width, height);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfResizeAppWindow::toString() { return funcDeclaration.get()->name.get()->lexeme; }