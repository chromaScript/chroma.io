#include "../../include/cscript/CCallable.h"
#include "../../include/cscript/CCallable_Lib.h"
#include "../../include/cscript/lib/CCLib_STDWidget.h"

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
// Internal Built-In Widget Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CInt_TextClick
CInt_TextClick::CInt_TextClick(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CInt_TextClick;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "@textClick", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CInt_TextClick::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	if (!ui.get()->activeWidget.expired())
	{
		// Text Input widgets automatically receive focus, even in the absense of a
		// valid callback. If a focus callback exists, then it will have been triggered
		// before reaching this point.
		ui.get()->updateFocusWidget(ui.get()->activeWidget);
		// Update the active input widget, which will signal to the program to swap
		// the input stream over to text.
		ui.get()->putActiveInputWidget(ui.get()->activeWidget, false, false, UI_WEVENT_NULL);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CInt_TextClick::toString() { return "<int @textClick>"; }



////////////////////////////////////////////////////////////////////////////////////////////////
//
// Standard Library Global Widget Operator Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_fGetParent
CStd_fGetParent::CStd_fGetParent(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fGetParent;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "Widget", -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getParent", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fGetParent::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	if (!ui.get()->activeWidget.expired() && !ui.get()->activeWidget.lock().get()->parentWidget.expired())
	{
		std::vector<std::shared_ptr<CObject>> instanceArgs;
		std::shared_ptr<CObject> callee = interpreter.get()->currentEnvironment.get()->get({ "global" }, "Widget");
		instanceArgs.push_back(callee);
		std::string name = ui.get()->activeWidget.lock()->parentWidget.lock()->idAttrib;
		instanceArgs.push_back(std::make_shared<CObject>(name));
		std::shared_ptr<CObject> returnObj = std::get<std::shared_ptr<CClass>>(callee.get()->obj).get()->call(interpreter, &instanceArgs);
		std::get<std::shared_ptr<CInstance>>(returnObj.get()->obj).get()->instanceEnv.get()->get("@widgetObj").get()->obj = ui.get()->activeWidget.lock().get()->parentWidget;
		return returnObj;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fGetParent::toString() { return "<std getParent>"; }

//
//
// CStd_fGetRoot
CStd_fGetRoot::CStd_fGetRoot(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fGetRoot;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "Widget", -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getRoot", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fGetRoot::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	if (!ui.get()->activeWidget.expired())
	{
		std::weak_ptr<Widget> root = ui.get()->activeWidget.lock().get()->getRoot();
		if (root.expired()) { return std::make_shared<CObject>(nullptr); }

		std::vector<std::shared_ptr<CObject>> instanceArgs;
		std::shared_ptr<CObject> callee = interpreter.get()->currentEnvironment.get()->get({ "global" }, "Widget");
		instanceArgs.push_back(callee);
		instanceArgs.push_back(args[0]);
		std::shared_ptr<CObject> returnObj = std::get<std::shared_ptr<CClass>>(callee.get()->obj).get()->call(interpreter, &instanceArgs);
		std::get<std::shared_ptr<CInstance>>(returnObj.get()->obj).get()->instanceEnv.get()->get("@widgetObj").get()->obj = root;
		return returnObj;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fGetRoot::toString() { return "<std getRoot>"; }

//
//
// CStd_fSetActiveProperty
CStd_fSetProperty::CStd_fSetProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetProperty::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->activeWidget.lock().get()->setProperty(interpreter, name, args[1]);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetProperty::toString() { return "<std setProperty>"; }

//
//
// CStd_fGetProperty
CStd_fGetProperty::CStd_fGetProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fGetProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::ANY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fGetProperty::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Get property name
	std::string name = std::get<std::string>(args[0].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		return ui.get()->activeWidget.lock().get()->getProperty(interpreter, name);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fGetProperty::toString() { return "<std getProperty>"; }

//
//
// CStd_fSetChildProperty_byID
CStd_fSetChildProperty_byID::CStd_fSetChildProperty_byID(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetChildProperty_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("id");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildProperty_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetChildProperty_byID::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string id = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->activeWidget.lock().get()->setChildProperty(0, interpreter, id, name, args[2]);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetChildProperty_byID::toString() { return "<std setChildProperty_byID>"; }

//
//
// CStd_fSetChildProperty_byType
CStd_fSetChildProperty_byType::CStd_fSetChildProperty_byType(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetChildProperty_byType;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("type");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildProperty_byType", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetChildProperty_byType::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string typeName = std::get<std::string>(args[0].get()->obj);
	LTokenType type = LTokenType::NIL;
	if (layoutTypeMap.count(typeName) == 1) { type = layoutTypeMap.at(typeName); }
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		std::shared_ptr<std::vector<std::weak_ptr<Widget>>> bucket =
			std::make_shared<std::vector<std::weak_ptr<Widget>>>();
		ui.get()->activeWidget.lock().get()->getChild_byType(bucket, type, "");
		for (std::weak_ptr<Widget> item : *bucket)
		{
			if (!item.expired())
			{
				item.lock().get()->setProperty(interpreter, name, args[2]);
			}
		}
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetChildProperty_byType::toString() { return "<std setChildProperty_byType>"; }

//
//
// CStd_fGetChildProperty_byType
CStd_fGetChildProperty_byType::CStd_fGetChildProperty_byType(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fGetChildProperty_byType;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("type");
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::ANY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getChildProperty_byType", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fGetChildProperty_byType::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string typeName = std::get<std::string>(args[0].get()->obj);
	LTokenType type = LTokenType::NIL;
	if (layoutTypeMap.count(typeName) == 1) { type = layoutTypeMap.at(typeName); }
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		std::shared_ptr<std::vector<std::weak_ptr<Widget>>> bucket =
			std::make_shared<std::vector<std::weak_ptr<Widget>>>();
		ui.get()->activeWidget.lock().get()->getChild_byType(bucket, type, "");
		for (std::weak_ptr<Widget> item : *bucket)
		{
			if (!item.expired())
			{
				// Warning : This currently returns first of, later must expand to return multiple
				return item.lock().get()->getProperty(interpreter, name);
			}
		}
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fGetChildProperty_byType::toString() { return "<std getChildProperty_byType>"; }

//
//
// CStd_fSetChildProperty_byClass
CStd_fSetChildProperty_byClass::CStd_fSetChildProperty_byClass(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetChildProperty_byClass;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("class");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildProperty_byClass", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetChildProperty_byClass::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		std::shared_ptr<std::vector<std::weak_ptr<Widget>>> bucket =
			std::make_shared<std::vector<std::weak_ptr<Widget>>>();
		ui.get()->activeWidget.lock().get()->getChild_byClass(bucket, className, "");
		for (std::weak_ptr<Widget> item : *bucket)
		{
			if (!item.expired())
			{
				item.lock().get()->setProperty(interpreter, name, args[2]);
			}
		}
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetChildProperty_byClass::toString() { return "<std setChildProperty_byClass>"; }

//
//
// CStd_fSetChildProperty_byName
CStd_fSetChildProperty_byName::CStd_fSetChildProperty_byName(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetChildProperty_byName;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("nameID");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildProperty_byClass", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetChildProperty_byName::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string nameName = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->activeWidget.lock().get()->setChildProperty(1, interpreter, nameName, name, args[2]);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetChildProperty_byName::toString() { return "<std setChildProperty_byName>"; }

//
//
// CStd_fSetClassProperty
CStd_fSetClassProperty::CStd_fSetClassProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetClassProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("class");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setClassProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetClassProperty::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);

	// Call into the UI
	ui.get()->setPropertyByClass(interpreter, className, "", name, args[2]);

	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetClassProperty::toString() { return "<std setClassProperty>"; }

//
//
// CStd_fSetClassProperty_excludingID
CStd_fSetClassProperty_excludingID::CStd_fSetClassProperty_excludingID(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetClassProperty_excludingID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("class");
	paramsNames.push_back("exclusion");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setClassProperty_excludingID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetClassProperty_excludingID::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::string exclusion = std::get<std::string>(args[1].get()->obj);
	std::string name = std::get<std::string>(args[2].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->setPropertyByClass(interpreter, className, exclusion, name, args[3]);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetClassProperty_excludingID::toString() { return "<std setClassProperty_excludingID>"; }

//
//
// CStd_fSetClassProperty_excludingActive
CStd_fSetClassProperty_excludingActive::CStd_fSetClassProperty_excludingActive(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetClassProperty_excludingActive;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::ANY, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("class");
	paramsNames.push_back("name");
	paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setClassProperty_excludingActive", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fSetClassProperty_excludingActive::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::weak_ptr<Widget> exclusion = ui.get()->activeWidget;
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->setPropertyByClass(interpreter, className, exclusion, name, args[2]);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fSetClassProperty_excludingActive::toString() { return "<std setClassProperty_excludingActive>"; }

//
//
// CStd_fResetProperty
CStd_fResetProperty::CStd_fResetProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fResetProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "resetProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fResetProperty::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->activeWidget.lock().get()->resetProperty(interpreter, name);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fResetProperty::toString() { return "<std resetProperty>"; }

//
//
// CStd_fResetClassProperty
CStd_fResetClassProperty::CStd_fResetClassProperty(std::shared_ptr<CEnvironment> funcEnv, std::shared_ptr<UI> ui)
{
	this->ui = ui;
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fResetClassProperty;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("class");
	paramsNames.push_back("name");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "resetClassProperty", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_fResetClassProperty::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);
	if (!ui.get()->activeWidget.expired())
	{
		ui.get()->resetPropertyByClass(interpreter, className, name);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_fResetClassProperty::toString() { return "<std resetClassProperty>"; }