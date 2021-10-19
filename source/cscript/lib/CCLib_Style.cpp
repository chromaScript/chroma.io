#include "../../include/cscript/CCallable.h"
#include "../../include/cscript/CCallable_Lib.h"
#include "../../include/cscript/lib/CCLib_Style.h"

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
// CStd_Style
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CStd_Style
CStd_cStyle::CStd_cStyle(std::shared_ptr<CEnvironment> classEnv)
{
	this->name = "Style";
	this->type = CCallableTypes::_CStd_cStyle;
	this->classEnv = classEnv;
	this->superClass = nullptr;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CStmt>> methods;
	this->classEnv.get()->define("setProperty_byName", std::make_shared<CObject>(CCallableTypes::_CStd_cfSetProperty_byName, this->classEnv));
	this->classEnv.get()->define("getProperty_byName", std::make_shared<CObject>(CCallableTypes::_CStd_cfGetProperty_byName, this->classEnv));
	for (auto const& item : this->classEnv.get()->values)
	{
		std::shared_ptr<CStmt> func = std::get<std::shared_ptr<CFunction>>(item.second.get()->obj).get()->funcDeclaration;
		methods.push_back(func);
	}

	// Build the properties
	std::vector<std::shared_ptr<CStmt>> properties;
	// Raw Style Literal Object (Hidden using @)
	this->classEnv.get()->define("@styleObj", std::make_shared<CObject>(std::make_shared<WidgetStyle>()));
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
std::string CStd_cStyle::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Style Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CStd_cfSetProperty_byName
CStd_cfSetProperty_byName::CStd_cfSetProperty_byName(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetProperty_byName;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setProperty_byName", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetProperty_byName::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	// Fetch the styleObj, always has the same value name
	std::shared_ptr<WidgetStyle> thisStyle =
		std::get<std::shared_ptr<WidgetStyle>>(
			interpreter.get()->currentEnvironment.get()->values.at("@styleObj").get()->obj);
	thisStyle.get()->getSetProperty(interpreter, name, args[1], false);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetProperty_byName::toString() { return funcDeclaration.get()->name.get()->lexeme; }

// CStd_cfGetProperty_byName
CStd_cfGetProperty_byName::CStd_cfGetProperty_byName(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfGetProperty_byName;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::ANY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getProperty_byName", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetProperty_byName::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Get property name
	std::string name = std::get<std::string>(args[0].get()->obj);
	// Fetch the styleObj, always has the same value name
	std::shared_ptr<WidgetStyle> thisStyle =
		std::get<std::shared_ptr<WidgetStyle>>(
			interpreter.get()->currentEnvironment.get()->values.at("@styleObj").get()->obj);
	return thisStyle.get()->getSetProperty(interpreter, name, nullptr, true);
}
std::string CStd_cfGetProperty_byName::toString() { return funcDeclaration.get()->name.get()->lexeme; }