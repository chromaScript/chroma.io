#include "../include/cscript/CCallable.h"

#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CForward.h"
#include "../include/cscript/CLiteral.h"
#include "../include/cscript/CExpr.h"
#include "../include/cscript/CStmt.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CObject.h"
#include "../include/cscript/CToken.h"

#include <string>
#include <vector>
#include <iostream>
#include <variant>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Callable Object Class Definitions:
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CCallable
std::shared_ptr<CObject> CCallable::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	return nullptr;
}
std::string CCallable::toString()
{
	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class Object Definitions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CClass
// Create instance of class
std::shared_ptr<CObject> CClass::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Capture the environment
	std::shared_ptr<CEnvironment> previous = interpreter.get()->currentEnvironment;
	// Grab the pass-through arguments (ClassTemplate, asignee)
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	std::string assignee = std::get<std::string>(args.back().get()->obj);
	args.pop_back();
	std::shared_ptr<CClass> classTemplate = std::get<std::shared_ptr<CClass>>(args.back().get()->obj);
	args.pop_back();
	// Fix the namespace
	std::string _namespace;
	if (assignee == "")
	{
		_namespace = randomNamespace(16);
	}
	else { _namespace = assignee; }
	// Make the instance return object
	std::shared_ptr<CObject> returnObj =
		std::make_shared<CObject>(interpreter.get()->currentEnvironment, _namespace, classTemplate);
	// Cast to instance class for operating
	std::shared_ptr<CInstance> instance = std::get<std::shared_ptr<CInstance>>(returnObj.get()->obj);
	interpreter.get()->getEnvironment().get()->define(
		_namespace,
		returnObj);
	// Enter the object environment
	interpreter.get()->currentThisInstance = instance.get()->instanceEnv;
	interpreter.get()->currentEnvironment = instance.get()->instanceEnv;
	interpreter.get()->currentNamespaces.push_back("instance:" + _namespace);
	// Call the constructor if exists
	std::shared_ptr<CObject> constructor = instance.get()->get("constructor:" + classTemplate.get()->name);
	if (constructor != nullptr)
	{
		// The return value of the constructor is discarded because the constructor will
		// operate directly on the new instance object
		std::get<std::shared_ptr<CFunction>>(constructor.get()->obj).get()->call(interpreter, &args);
	}
	// Return type of class object
	interpreter.get()->currentEnvironment = previous;
	interpreter.get()->currentThisInstance = nullptr;
	interpreter.get()->currentNamespaces.pop_back();
	return returnObj;
}
std::string CClass::toString()
{
	return name;
}

// CInstance
CInstance::CInstance(std::string className, std::shared_ptr<CEnvironment> instanceEnv)
{
	this->type = CCallableTypes::_CClassInstance;
	this->className = className;
	this->instanceEnv = instanceEnv;
}
std::shared_ptr<CObject> CInstance::get(std::string lookup)
{
	size_t index = std::string::npos;
	if (instanceEnv.get()->values.size() != 0)
	{
		for (auto const& item : instanceEnv.get()->values)
		{
			if (item.first == lookup)
			{
				return item.second;
			}
		}
	}
	return nullptr;
}
void CInstance::set(CToken name, std::shared_ptr<CObject> value)
{
	size_t index = std::string::npos;
}
std::shared_ptr<CObject> CInstance::call(
	std::shared_ptr<CInterpreter> interpreter, 
	std::vector<std::shared_ptr<CObject>>* arguments)
{
	return nullptr;
}
std::string CInstance::toString()
{
	return className + " instance";
}

// CUser_Class
CUser_Class::CUser_Class(
	std::shared_ptr<CStmt_Class> classDeclaration, 
	std::string name, 
	CCallableTypes type,
	std::shared_ptr<CEnvironment> classEnv,
	std::shared_ptr<CClass> superClass)
{
	this->classDeclaration = classDeclaration;
	this->name = name;
	this->type = type;
	this->classEnv = classEnv;
	this->superClass = superClass;
}
std::string CUser_Class::toString()
{
	return name;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function Object Definitions:
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CFunction
std::shared_ptr<CObject> CFunction::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	return nullptr;
}
std::string CFunction::toString()
{
	return "";
}

// CUser_Function
CUser_Function::CUser_Function(
	std::shared_ptr<CStmt_Function> funcDeclaration, CCallableTypes type, std::shared_ptr<CEnvironment> funcEnv)
{
	this->type = type;
	this->funcDeclaration = funcDeclaration;
	this->funcEnv = funcEnv;
}
std::shared_ptr<CObject> CUser_Function::call(
	std::shared_ptr<CInterpreter> interpreter, std::vector<std::shared_ptr<CObject>>* arguments)
{
	// Capture the current environment
	std::shared_ptr<CEnvironment> previous = interpreter.get()->getEnvironment();
	// Set environment to new virtual function environment
	interpreter.get()->setEnvironment(funcEnv.get()->virtualize("", nullptr));
	interpreter.get()->addNamespace(
		"function:" + funcDeclaration.get()->returnType.get()->typeString() + 
		":" + funcDeclaration.get()->name.get()->lexeme);
	std::shared_ptr<CEnvironment> thisEnv = interpreter.get()->getEnvironment();
	thisEnv.get()->returnValue = nullptr;
	interpreter.get()->currentFunctions.push_back(thisEnv);
	// Copy the arguments
	std::vector<std::shared_ptr<CObject>> args = *arguments;
	for (size_t i = 0; i < args.size(); i++)
	{
		thisEnv.get()->get(funcDeclaration.get()->paramsNames[i]).get()->obj =
			interpreter.get()->console.get()->copyObj(args[i], funcDeclaration.get()->paramsNames[i]).get()->obj;
	}
	// interpret the body
	funcDeclaration.get()->body.get()->accept(*interpreter);
	// Exit and clean up
	interpreter.get()->setEnvironment(previous);
	interpreter.get()->popNamespace();
	interpreter.get()->currentFunctions.pop_back();
	return thisEnv.get()->returnValue;
}
std::string CUser_Function::toString()
{
	return "<fn " + funcDeclaration.get()->name.get()->lexeme + ">";
}