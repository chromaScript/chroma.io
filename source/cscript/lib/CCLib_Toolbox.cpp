#include "../../include/cscript/CCallable.h"
#include "../../include/cscript/CCallable_Lib.h"
#include "../../include/cscript/lib/CCLib_Toolbox.h"

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
#include "../../include/entities/widgets/WidgetStyle.h"

#include "../../include/Application.h"
#include "../../include/entities/UserInterface.h"
#include "../../include/tool/Toolbox.h"
#include "../../include/entities/widgets/Widget.h"
#include "../../include/input/keys.h"
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
// CStd_cToolbox
//
////////////////////////////////////////////////////////////////////////////////////////////////

CStd_cToolbox::CStd_cToolbox(std::shared_ptr<CEnvironment> classEnv, std::shared_ptr<Application> app)
{
	this->name = "Toolbox";
	this->type = CCallableTypes::_CStd_cToolbox;
	this->classEnv = classEnv;
	this->superClass = nullptr;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CStmt>> methods;
	this->classEnv.get()->define(
		"checkValidIOCombination",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCheckValidIOCombination, classEnv.get()->lookupEnvironment("checkValidIOCombination", true)));
	this->classEnv.get()->define(
		"checkValidControlScheme",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCheckValidControlScheme, classEnv.get()->lookupEnvironment("checkValidControlScheme", true)));
	this->classEnv.get()->define(
		"createNewTool",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCreateNewTool, classEnv.get()->lookupEnvironment("createNewTool", true)));
	this->classEnv.get()->define(
		"checkActiveToolSettingsMask",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfCheckActiveToolSettingsMask, classEnv.get()->lookupEnvironment("checkActiveToolSettingsMask", true)));
	this->classEnv.get()->define(
		"setActiveToolProp",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetActiveToolProp, classEnv.get()->lookupEnvironment("setActiveToolProp", true)));
	this->classEnv.get()->define(
		"getActiveToolProp",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetActiveToolProp, classEnv.get()->lookupEnvironment("getActiveToolProp", true)));
	this->classEnv.get()->define(
		"setActiveToolInterest",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetActiveToolInterest, classEnv.get()->lookupEnvironment("setActiveToolInterest", true)));
	this->classEnv.get()->define(
		"getActiveToolInterest",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetActiveToolInterest, classEnv.get()->lookupEnvironment("getActiveToolInterest", true)));
	this->classEnv.get()->define(
		"exportAllTools",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfExportAllTools, classEnv.get()->lookupEnvironment("exportAllTools", true)));
	for (auto const& item : this->classEnv.get()->values)
	{
		std::shared_ptr<CStmt> func = std::get<std::shared_ptr<CFunction>>(item.second.get()->obj).get()->funcDeclaration;
		methods.push_back(func);
	}

	// Build the properties
	std::vector<std::shared_ptr<CStmt>> properties;
	// Raw UI Literal Object (Hidden using @)
	this->classEnv.get()->define("@toolboxObj", std::make_shared<CObject>(app.get()->getToolbox()));

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
std::string CStd_cToolbox::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// CStd_cToolbox Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_cfCheckValidIOCombination
CStd_cfCheckValidIOCombination::CStd_cfCheckValidIOCombination(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCheckValidIOCombination;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("inputMethod");
	paramsNames.push_back("outputMethod");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "checkValidIOCombination", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCheckValidIOCombination::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string inputName = std::get<std::string>(args[0].get()->obj);
	std::string outputName = std::get<std::string>(args[1].get()->obj);
	// Set the values
	bool result = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->checkValidIOCombination(
			interpreter, inputName, outputName);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfCheckValidIOCombination::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfCheckValidControlScheme
CStd_cfCheckValidControlScheme::CStd_cfCheckValidControlScheme(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCheckValidControlScheme;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("inputMethod");
	paramsNames.push_back("controlScheme");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "checkValidControlScheme", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCheckValidControlScheme::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string inputName = std::get<std::string>(args[0].get()->obj);
	std::string controlScheme = std::get<std::string>(args[1].get()->obj);
	// Set the values
	bool result = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->checkValidControlScheme(
			interpreter, inputName, controlScheme);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfCheckValidControlScheme::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfCreateNewTool
CStd_cfCreateNewTool::CStd_cfCreateNewTool(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCreateNewTool;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("cursorHover");
	paramsNames.push_back("cursorPress");
	paramsNames.push_back("toolID");
	paramsNames.push_back("toolName");
	paramsNames.push_back("inputMethod");
	paramsNames.push_back("controlScheme");
	paramsNames.push_back("outputMethod");
	paramsNames.push_back("keybind_modBit");
	paramsNames.push_back("keybind_glfwKey");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "createNewTool", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCreateNewTool::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string cursorHover = std::get<std::string>(args[0].get()->obj);
	std::string cursorPress = std::get<std::string>(args[1].get()->obj);
	int toolID = (int)std::get<double>(args[2].get()->obj);
	std::string toolName = std::get<std::string>(args[3].get()->obj);
	std::string inputMethod = std::get<std::string>(args[4].get()->obj);
	std::string controlScheme = std::get<std::string>(args[5].get()->obj);
	std::string outputMethod = std::get<std::string>(args[6].get()->obj);
	int keybind_modBit = (int)std::get<double>(args[7].get()->obj);
	int keybind_glfwKey = (int)std::get<double>(args[8].get()->obj);
	Keybind keybind = Keybind(keybind_glfwKey, keybind_modBit);
	// Set the values
	bool result = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->createCustomTool(
			cursorHover, cursorPress, toolID, toolName, inputMethod,
			controlScheme, outputMethod, keybind);
	if (!result)
	{
		interpreter.get()->console.get()->error("");
	}
	return std::make_shared<CObject>(result);
}
std::string CStd_cfCreateNewTool::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfCheckActiveToolSettingsMask
CStd_cfCheckActiveToolSettingsMask::CStd_cfCheckActiveToolSettingsMask(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfCheckActiveToolSettingsMask;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("maskName");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "checkActiveToolSettingsMask", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfCheckActiveToolSettingsMask::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string settingsMaskName = std::get<std::string>(args[0].get()->obj);
	// Set the values
	std::shared_ptr<Toolbox> toolbox = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj);
	bool result = false;
	if (toolbox.get()->checkActiveTool()) {
		result = toolbox.get()->getActiveTool().get()->checkInterestMask(
			interpreter, settingsMaskName);
	}
	return std::make_shared<CObject>(result);
}
std::string CStd_cfCheckActiveToolSettingsMask::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetActiveToolProp
CStd_cfSetActiveToolProp::CStd_cfSetActiveToolProp(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetActiveToolProp;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("settingSig");
	paramsNames.push_back("subOptionSig");
	paramsNames.push_back("value");
	paramsNames.push_back("asPercentage");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setActiveToolProp", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetActiveToolProp::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	int sig = (int)std::get<double>(args[0].get()->obj);
	int subSig = (int)std::get<double>(args[1].get()->obj);
	bool asPercentage = std::get<bool>(args[3].get()->obj);
	// Set the values
	std::shared_ptr<Tool> activeTool = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->getActiveTool();
	if (activeTool != nullptr)
	{
		activeTool.get()->putProperty(
				interpreter, sig, subSig, args[2], false, asPercentage, false);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetActiveToolProp::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetActiveToolProp
CStd_cfGetActiveToolProp::CStd_cfGetActiveToolProp(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfGetActiveToolProp;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("settingSig");
	paramsNames.push_back("subOptionSig");
	paramsNames.push_back("asPercentage");
	paramsNames.push_back("asString");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::ANY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getActiveToolProp", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetActiveToolProp::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	int sig = (int)std::get<double>(args[0].get()->obj);
	int subSig = (int)std::get<double>(args[1].get()->obj);
	bool getAsPercentage = std::get<bool>(args[2].get()->obj);
	bool asString = std::get<bool>(args[3].get()->obj);
	// Set the values
	std::shared_ptr<Tool> activeTool = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->getActiveTool();
	if (activeTool != nullptr)
	{
		std::shared_ptr<CObject> returnObj = activeTool.get()->putProperty(
				interpreter, sig, subSig, nullptr, true, getAsPercentage, asString);
		return returnObj;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfGetActiveToolProp::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetActiveToolInterest
CStd_cfSetActiveToolInterest::CStd_cfSetActiveToolInterest(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetActiveToolInterest;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("interestName");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setActiveToolInterest", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetActiveToolInterest::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string interestName = std::get<std::string>(args[0].get()->obj);
	TSetType interest = stringToTSetType(interestName);
	// Set the values
	std::shared_ptr<Tool> activeTool = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->getActiveTool();
	if (activeTool != nullptr)
	{
		activeTool.get()->putInterest(
				interpreter, interest, args[1], false);
	}
	
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetActiveToolInterest::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetActiveToolInterest
CStd_cfGetActiveToolInterest::CStd_cfGetActiveToolInterest(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfGetActiveToolInterest;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("interestName");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::ANY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getActiveToolInterest", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetActiveToolInterest::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string interestName = std::get<std::string>(args[0].get()->obj);
	TSetType interest = stringToTSetType(interestName);
	// Set the values
	std::shared_ptr<Tool> activeTool = std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->getActiveTool();
	if (activeTool != nullptr)
	{
		std::shared_ptr<CObject> returnObj = activeTool.get()->putInterest(
				interpreter, interest, nullptr, true);
		return returnObj;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfGetActiveToolInterest::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfExportAllTools
CStd_cfExportAllTools::CStd_cfExportAllTools(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfExportAllTools;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("overwrite");
	paramsNames.push_back("useDefaultRoot");
	paramsNames.push_back("rootDir");

	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "exportAllTools", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfExportAllTools::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	bool overwrite = std::get<bool>(args[0].get()->obj);
	bool useDefault = std::get<bool>(args[1].get()->obj);
	std::filesystem::path rootDir = (std::filesystem::path)std::get<std::string>(args[2].get()->obj);
	// Set the values
	std::get<std::shared_ptr<Toolbox>>(
		interpreter.get()->currentEnvironment.get()->values.at("@toolboxObj").get()->obj).get()->exportTools(true, { 0 }, overwrite, useDefault, rootDir);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfExportAllTools::toString() { return funcDeclaration.get()->name.get()->lexeme; }