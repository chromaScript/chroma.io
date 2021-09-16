#include "../../include/cscript/CCallable.h"
#include "../../include/cscript/CCallable_Lib.h"
#include "../../include/cscript/lib/CCLib_Widget.h"

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
// CStd_Widget 
//
////////////////////////////////////////////////////////////////////////////////////////////////

CStd_cWidget::CStd_cWidget(std::shared_ptr<CEnvironment> classEnv)
{
	this->name = "Widget";
	this->type = CCallableTypes::_CStd_cWidget;
	this->classEnv = classEnv;
	this->superClass = nullptr;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	// Build the methods
	std::vector<std::shared_ptr<CStmt>> methods;
	this->classEnv.get()->define(
		"rebuildOutofBoundsWidgets",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfRebuildOutofBoundsWidgets, classEnv.get()->lookupEnvironment("rebuildOutofBoundsWidgets", true)));
	this->classEnv.get()->define(
		"setProperty",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetProperty, classEnv.get()->lookupEnvironment("setProperty", true)));
	this->classEnv.get()->define(
		"setChildProperty_byClass",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetChildProperty_byClass, classEnv.get()->lookupEnvironment("setChildProperty_byClass", true)));
	this->classEnv.get()->define(
		"getChildrenProperties",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetChildrenProperties, classEnv.get()->lookupEnvironment("getChildrenProperties", true)));
	this->classEnv.get()->define(
		"getProperty",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfGetProperty, classEnv.get()->lookupEnvironment("getProperty", true)));
	this->classEnv.get()->define(
		"setPosition",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetPosition, classEnv.get()->lookupEnvironment("setPosition", true)));
	this->classEnv.get()->define(
		"setChildOrder_byID",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfSetChildOrder_byID, classEnv.get()->lookupEnvironment("setChildOrder_byID", true)));
	this->classEnv.get()->define(
		"addChildWidget_prototype",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfAddChildWidget_Prototype, classEnv.get()->lookupEnvironment("addChildWidget_prototype", true)));
	this->classEnv.get()->define(
		"removeChildWidget_byID",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfRemoveChildWidget_byID, classEnv.get()->lookupEnvironment("removeChildWidget_byID", true)));
	this->classEnv.get()->define(
		"removeChildWidgets_byClass",
		std::make_shared<CObject>(CCallableTypes::_CStd_cfRemoveChildWidgets_byClass, classEnv.get()->lookupEnvironment("removeChildWidgets_byClass", true)));
	for (auto const& item : this->classEnv.get()->values)
	{
		std::shared_ptr<CStmt> func = std::get<std::shared_ptr<CFunction>>(item.second.get()->obj).get()->funcDeclaration;
		methods.push_back(func);
	}

	// Create properties
	std::vector<std::shared_ptr<CStmt>> properties;
	// Raw Empty Widget Literal Object (Hidden using @) - Set this when returning a widget
	this->classEnv.get()->define("@widgetObj", std::make_shared<CObject>(CLiteral(CLiteralTypes::_CWidget, this->name), "@widgetObj"));

	// Create the pseudo-CStmt_Class
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
std::string CStd_cWidget::toString() { return name; }

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Widget Class Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
//
// CStd_cfRebuildOutofBoundsWidgets
CStd_cfRebuildOutofBoundsWidgets::CStd_cfRebuildOutofBoundsWidgets(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfRebuildOutofBoundsWidgets;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "rebuildOutofBoundsWidgets", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfRebuildOutofBoundsWidgets::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	// Call setProperty on the widgetObj
	std::weak_ptr<Widget> widget = std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj);
	if (!widget.expired())
	{
		std::weak_ptr<Widget> root = widget.lock().get()->getRoot();
		if (!root.expired())
		{
			root.lock()->checkOutofBoundsWidgets(root, root.lock()->getColliderBox());
		}
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfRebuildOutofBoundsWidgets::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetProperty
CStd_cfSetProperty::CStd_cfSetProperty(std::shared_ptr<CEnvironment> funcEnv)
{
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
std::shared_ptr<CObject> CStd_cfSetProperty::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	// Call setProperty on the widgetObj
	std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj).lock().get()->setProperty(interpreter, name, args[1]);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetProperty::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetChildOrder
CStd_cfSetChildOrder_byID::CStd_cfSetChildOrder_byID(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetChildOrder_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("childID");
	paramsNames.push_back("order");
	paramsNames.push_back("ignoreHidden");
	paramsNames.push_back("asLiteral"); // Determines whether to "shift" by num or to as "literal" swap
	//paramsNames.push_back("value");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setChildOrder_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetChildOrder_byID::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string childID = std::get<std::string>(args[0].get()->obj);
	int moveNumber = (int)std::get<double>(args[1].get()->obj);
	bool ignoreHidden = std::get<bool>(args[2].get()->obj);
	bool asLiteral = std::get<bool>(args[3].get()->obj);
	// Call getChild_byID on the widgetObj
	std::weak_ptr<Widget> parent = std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj);
	if (!parent.expired())
	{
		parent.lock()->updateChildOrder(childID, moveNumber, ignoreHidden, asLiteral, false);
		return std::make_shared<CObject>(nullptr);
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetChildOrder_byID::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfAddChildWidget_Prototype
CStd_cfAddChildWidget_Prototype::CStd_cfAddChildWidget_Prototype(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfAddChildWidget_Prototype;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING_ARRAY, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("protoID");
	paramsNames.push_back("newChildID");
	paramsNames.push_back("newChildClasses");
	paramsNames.push_back("newChildOrdering");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "Widget", -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "addChildWidget_prototype", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfAddChildWidget_Prototype::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string protoID = std::get<std::string>(args[0].get()->obj);
	std::string childID = std::get<std::string>(args[1].get()->obj);
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> extraClassObj =
		std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(args[2].get()->obj);
	std::vector<std::string> extraClasses;
	for (std::shared_ptr<CObject> extra : *extraClassObj)
	{
		if (extra.get()->objType.type == CLiteralTypes::_CString) { extraClasses.push_back(std::get<std::string>(extra.get()->obj)); }
	}
	int moveNumber = (int)std::get<double>(args[3].get()->obj);
	// Call setProperty on the widgetObj
	std::weak_ptr<Widget> targetWidget = std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj);
	std::weak_ptr<Widget> newWidget = targetWidget.lock()->addChildWidget(
		interpreter, protoID, childID, extraClasses, moveNumber);
	if (!newWidget.expired())
	{
		std::vector<std::shared_ptr<CObject>> instanceArgs;
		std::shared_ptr<CObject> callee = interpreter.get()->currentEnvironment.get()->get({ "global" }, "Widget");
		instanceArgs.push_back(callee);
		instanceArgs.push_back(args[1]);
		std::shared_ptr<CObject> returnObj = std::get<std::shared_ptr<CClass>>(callee.get()->obj).get()->call(interpreter, &instanceArgs);
		std::get<std::shared_ptr<CInstance>>(returnObj.get()->obj).get()->instanceEnv.get()->get("@widgetObj").get()->obj = newWidget;
		return returnObj;
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfAddChildWidget_Prototype::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfRemoveChildWidget_byID
CStd_cfRemoveChildWidget_byID::CStd_cfRemoveChildWidget_byID(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfRemoveChildWidget_byID;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("childID");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "removeChildWidget_byID", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfRemoveChildWidget_byID::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string deleteID = std::get<std::string>(args[0].get()->obj);
	// Call deleteChild on the widgetObj
	bool result = std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj).lock().get()->removeChild_byID(interpreter, deleteID);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfRemoveChildWidget_byID::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfRemoveChildWidgets_byClass
CStd_cfRemoveChildWidgets_byClass::CStd_cfRemoveChildWidgets_byClass(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfRemoveChildWidgets_byClass;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("className");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::BOOL, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "removeChildWidgets_byClass", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfRemoveChildWidgets_byClass::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	// Call setProperty on the widgetObj
	bool result = std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj).lock().get()->removeChildren_byClass(interpreter, className);
	return std::make_shared<CObject>(result);
}
std::string CStd_cfRemoveChildWidgets_byClass::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetChildProperty_byClass
CStd_cfSetChildProperty_byClass::CStd_cfSetChildProperty_byClass(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetChildProperty_byClass;
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
std::shared_ptr<CObject> CStd_cfSetChildProperty_byClass::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string className = std::get<std::string>(args[0].get()->obj);
	std::string name = std::get<std::string>(args[1].get()->obj);
	// Get the widgets and set the properties
	std::shared_ptr<std::vector<std::weak_ptr<Widget>>> bucket =
		std::make_shared<std::vector<std::weak_ptr<Widget>>>();
	std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj).lock().get()->getChild_byClass(bucket, className, "");
	for (std::weak_ptr<Widget> item : *bucket)
	{
		if (!item.expired())
		{
			item.lock().get()->setProperty(interpreter, name, args[2]);
		}
	}
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetChildProperty_byClass::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetChildrenProperties
CStd_cfGetChildrenProperties::CStd_cfGetChildrenProperties(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfGetChildrenProperties;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::STRING, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::BOOL, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("value");
	paramsNames.push_back("ignoreHidden");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::ANY_ARRAY, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "getChildrenProperties", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfGetChildrenProperties::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string valueName = std::get<std::string>(args[0].get()->obj);
	bool ignoreHidden = std::get<bool>(args[1].get()->obj);
	// Get the widget and then loop through its children
	std::weak_ptr<Widget> targetParent = std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj);

	std::vector<std::shared_ptr<CObject>> valuesBucket;
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> valuesBucketObj = std::make_shared<std::vector<std::shared_ptr<CObject>>>(valuesBucket);
	bool foundType = false;
	CLiteralTypes firstType = CLiteralTypes::_CNil;
	if (!targetParent.expired())
	{
		for (std::weak_ptr<Widget> child : targetParent.lock()->childWidgets)
		{
			if (!child.expired())
			{
				if (ignoreHidden && child.lock()->style.visibility <= 0) { continue; }
				valuesBucketObj.get()->push_back(child.lock().get()->getProperty(interpreter, valueName));
				if (valuesBucketObj.get()->back() == nullptr) { valuesBucketObj.get()->pop_back(); }
				else
				{
					if (!foundType) { firstType = valuesBucketObj.get()->back().get()->objType.type; }
				}
			}
		}
	}
	return std::make_shared<CObject>(firstType, valuesBucketObj);
}
std::string CStd_cfGetChildrenProperties::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfGetProperty
CStd_cfGetProperty::CStd_cfGetProperty(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_fSetProperty;
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
std::shared_ptr<CObject> CStd_cfGetProperty::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	std::string name = std::get<std::string>(args[0].get()->obj);
	// Call getProperty on the widgetObj
	return std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj).lock().get()->getProperty(interpreter, name);
}
std::string CStd_cfGetProperty::toString() { return funcDeclaration.get()->name.get()->lexeme; }

//
//
// CStd_cfSetPosition
CStd_cfSetPosition::CStd_cfSetPosition(std::shared_ptr<CEnvironment> funcEnv)
{
	this->funcEnv = funcEnv;
	this->type = CCallableTypes::_CStd_cfSetPosition;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	paramsTypes.push_back(std::make_shared<CToken>(CTokenType::NUM, -1));
	std::vector<std::string> paramsNames;
	paramsNames.push_back("x");
	paramsNames.push_back("y");
	this->funcDeclaration = std::make_shared<CStmt_Function>(
		std::make_shared<CToken>(CTokenType::_VOID, -1), // returnType
		std::make_shared<CToken>(CTokenType::IDENTIFIER, "setPosition", -1), // name
		false, // isDeclarationOnly
		scopeStack, // scope operator (empty)
		paramsTypes, // paramTypes (empty)
		paramsNames, // paramNames (empty)
		nullptr // function body (native, see 'call()')
		);
}
std::shared_ptr<CObject> CStd_cfSetPosition::call(std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Get Args
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	// Cast Arg 0 as string
	int x = (int)std::get<double>(args[0].get()->obj);
	int y = (int)std::get<double>(args[1].get()->obj);
	// Call setProperty on the widgetObj
	std::get<std::weak_ptr<Widget>>(
		interpreter.get()->currentEnvironment.get()->values.at("@widgetObj").get()->obj).lock().get()->setLocation(x, y);
	return std::make_shared<CObject>(nullptr);
}
std::string CStd_cfSetPosition::toString() { return funcDeclaration.get()->name.get()->lexeme; }