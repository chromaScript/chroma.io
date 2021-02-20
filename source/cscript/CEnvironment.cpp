#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/CExpr.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CToken.h"
#include "../include/cstyle/SToken.h"
#include "../include/cscript/CObject.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>


CEnvironment::CEnvironment()
{
	this->enclosing = nullptr;
}
CEnvironment::CEnvironment(std::shared_ptr<ChromaScript> console, std::string _namespace)
{
	this->_namespace = _namespace;
	this->console = console;
	this->enclosing = nullptr;
}
CEnvironment::CEnvironment(
	std::shared_ptr<CEnvironment> enclosing, 
	std::shared_ptr<ChromaScript> console, 
	std::string _namespace)
{
	this->console = console;
	this->enclosing = enclosing;
	this->_namespace = _namespace;
}

// Return a copy of this environment, such that the enclosing & scopes are real, but this environment is a copy
// along with every value in it. Computationally expensive, so could slow down a function that has
// dozens of variables or nested functions/classes/loops/logic-branches
// The scopes are kept as real to serve as templates, there is no need to virtualize them because they will
// be virtualized if they get entered
// Do not virtualize to enter anonymous blocks, and namespaces
std::shared_ptr<CEnvironment> CEnvironment::virtualize(std::string _namespaceNew, std::shared_ptr<CEnvironment> targetEnclosing)
{
	std::string newNamespace = _namespaceNew;
	if (newNamespace == "") { newNamespace = this->_namespace; }
	std::shared_ptr<CEnvironment> newEnclosing = targetEnclosing;
	if (newEnclosing == nullptr) { newEnclosing = this->enclosing; }
	std::shared_ptr<CEnvironment> virtualBox = std::make_shared<CEnvironment>(newEnclosing, console, newNamespace);
	// Soft copy the top-most scopes so they redirect to the virtualBox
	for (std::shared_ptr<CEnvironment> scope : scopes)
	{
		virtualBox.get()->scopes.push_back(scope);
		virtualBox.get()->scopes.back().get()->enclosing = virtualBox;
	}
	// Strong copy the values from this into VirtualBox
	for (auto const& item : values)
	{
		virtualBox.get()->values.insert(
			std::pair<std::string, std::shared_ptr<CObject>>(
				item.first,
				nullptr));
		virtualBox.get()->values.at(item.first) = console.get()->copyObj(item.second, item.first);
	}
	return virtualBox;
}
// Copy values from one class to another
void CEnvironment::copyData(std::shared_ptr<CEnvironment> targetEnclosing, std::shared_ptr<CEnvironment> target)
{
	for (auto& item : target.get()->values)
	{
		values.insert(
			std::pair<std::string, std::shared_ptr<CObject>>(
				item.first,
				nullptr));
		values.at(item.first) = console.get()->copyObj(item.second, item.first);
	}
	for (std::shared_ptr<CEnvironment> scope : target.get()->scopes)
	{
		scopes.push_back(scope);
		scopes.back().get()->enclosing = targetEnclosing;
	}
}

std::shared_ptr<CEnvironment> CEnvironment::lookupEnvironment(std::shared_ptr<CExpr_Scope> scopeExpr)
{
	std::vector<std::shared_ptr<CToken>> address = *scopeExpr.get()->scopeStack;
	std::string lookup = address.back()->lexeme;
	//address.pop_back();
	return lookupEnvironment(address, lookup);
}

std::shared_ptr<CEnvironment> CEnvironment::lookupEnvironment(std::vector<std::string> scopeStack, std::string lookup)
{
	if (scopeStack.size() == 0)
	{
		return lookupEnvironment(lookup, false);
	}
	if (scopeStack[0] == "global")
	{
		std::vector<std::string> trimStack = scopeStack;
		trimStack.erase(trimStack.begin());
		return console.get()->global.get()->lookupEnvironment(trimStack, lookup);
	}
	else
	{
		for (std::shared_ptr<CEnvironment> scope : scopes)
		{
			if (scope.get()->_namespace == scopeStack[0])
			{
				std::vector<std::string> trimStack = scopeStack;
				trimStack.erase(trimStack.begin());
				return scope.get()->lookupEnvironment(trimStack, lookup);
			}
		}
	}
	return nullptr;
}

std::shared_ptr<CEnvironment> CEnvironment::lookupEnvironment(std::string lookup, bool allowNew)
{
	if (lookup == _namespace)
	{
		return shared_from_this();
	}
	else
	{
		bool foundMatch = false;
		for (std::shared_ptr<CEnvironment> scope : scopes)
		{
			if (scope.get()->_namespace == lookup)
			{
				foundMatch = true;
				return scope;
			}
		}
		if (foundMatch == false)
		{
			if (allowNew == true)
			{
				scopes.emplace_back(std::make_shared<CEnvironment>(shared_from_this(), console, lookup));
				return scopes.back();
			}
			else
			{
				return nullptr;
			}
		}
	}
	// Unreachable
	return nullptr;
}
std::shared_ptr<CEnvironment> CEnvironment::lookupEnvironment(std::vector<std::shared_ptr<CToken>> scopeStack, std::string lookup)
{
	if (scopeStack.size() == 0) 
	{
		return lookupEnvironment(lookup, false); 
	}
	if (scopeStack[0].get()->lexeme == "global")
	{
		std::vector<std::shared_ptr<CToken>> trimStack = scopeStack;
		trimStack.erase(trimStack.begin());
		return console.get()->global.get()->lookupEnvironment(trimStack, lookup);
	}
	else
	{
		for (std::shared_ptr<CEnvironment> scope : scopes)
		{
			if (scope.get()->_namespace == scopeStack[0].get()->lexeme)
			{
				std::vector<std::shared_ptr<CToken>> trimStack = scopeStack;
				trimStack.erase(trimStack.begin());
				return scope.get()->lookupEnvironment(trimStack, lookup);
			}
		}
	}
	return nullptr;
}

//
std::shared_ptr<CEnvironment> CEnvironment::fetchEnvironment(std::string lookup)
{
	// Only use this function for superclass lookup. Only scenario where automatic
	// scope resolution is used.
	// Begin bottom-up lookup
	if (lookup == _namespace)
	{
		return shared_from_this();
	}
	bool foundMatch = false;
	for (std::shared_ptr<CEnvironment> scope : scopes)
	{
		if (scope.get()->_namespace == lookup)
		{
			foundMatch = true;
			return scope;
		}
	}
	// Reached global environment
	if (enclosing != nullptr) { return enclosing.get()->fetchEnvironment(lookup); }
	else
	{
		// Use top-down lookup
		return findEnvironment(lookup);
	}
	return nullptr;
}

//
std::shared_ptr<CEnvironment> CEnvironment::findEnvironment(std::string lookup)
{
	bool foundMatch = false;
	for (std::shared_ptr<CEnvironment> scope : scopes)
	{
		if (scope.get()->_namespace == lookup)
		{
			foundMatch = true;
			return scope;
		}
	}
	// Untested!
	std::shared_ptr<CEnvironment> out = nullptr;
	if (scopes.size() != 0) 
	{ 
		for (std::shared_ptr<CEnvironment> scope : scopes)
		{
			out = findEnvironment(lookup);
			if (out != nullptr)
			{
				return out;
			}
		}
	}
	return nullptr;
}

//
std::vector<std::string> CEnvironment::fetchValueNames()
{
	std::vector<std::string> names;
	for (auto const& item : values)
	{
		names.push_back(item.first);
	}
	return names;
}

//
int CEnvironment::declare(std::string name)
{
	// Object name not existing in current environment
	if (values.find(name) == values.end())
	{
		values.insert(std::pair<std::string, std::shared_ptr<CObject>>(name, nullptr));
		return 0;
	}
	// Object already exists in current environment
	else
	{
		return 1;
	}
}

// 
int CEnvironment::define(std::string name, std::shared_ptr<CObject> value)
{
	if (values.count(name) == 0)
	{
		values.insert(std::pair<std::string, std::shared_ptr<CObject>>(name, value));
	}
	else
	{
		values.at(name) = value;
	}
	return 0;
}

// Primary get from local scope
std::shared_ptr<CObject> CEnvironment::get(std::string name)
{
	if (values.count(name))
	{
		return values.at(name);
	}
	else
	{
		if (enclosing != nullptr) { return enclosing.get()->get(name); }
		return nullptr;
	}
}
// Secondary get using raw string scopeStack vector
std::shared_ptr<CObject> CEnvironment::get(std::vector<std::string> scopeStack, std::string name)
{
	if (scopeStack.size() == 0)
	{
		return get(name);
	}
	if (scopeStack[0] == "global")
	{
		std::vector<std::string> trimStack = scopeStack;
		trimStack.erase(trimStack.begin());
		return console.get()->global.get()->get(trimStack, name);
	}
	else
	{
		for (std::shared_ptr<CEnvironment> scope : scopes)
		{
			if (scope.get()->_namespace == scopeStack[0])
			{
				std::vector<std::string> trimStack = scopeStack;
				trimStack.erase(trimStack.begin());
				return scope.get()->get(trimStack, name);
			}
		}
	}
	return nullptr;
}
// Conversion get, CToken to string vector
std::shared_ptr<CObject> CEnvironment::get(std::vector<std::shared_ptr<CToken>> scopeStack, std::string name)
{
	if (scopeStack.size() == 0)
	{
		return get(name);
	}
	else
	{
		std::vector<std::string> stringStack;
		for (std::shared_ptr<CToken> token : scopeStack)
		{
			stringStack.push_back(token.get()->lexeme);
		}
		return get(stringStack, name);
	}
}
// Conversion get, SToken to string vector
std::shared_ptr<CObject> CEnvironment::get(std::vector<std::shared_ptr<SToken>> scopeStack, std::string name)
{
	if (scopeStack.size() == 0)
	{
		return get(name);
	}
	else
	{
		std::vector<std::string> stringStack;
		for (std::shared_ptr<SToken> token : scopeStack)
		{
			stringStack.push_back(token.get()->lexeme);
		}
		return get(stringStack, name);
	}
}

std::shared_ptr<CEnvironment> CEnvironment::getEnvironment(std::string name)
{
	for (std::shared_ptr<CEnvironment> scope : scopes)
	{
		if (scope.get()->_namespace == name)
		{
			return scope;
		}
	}
	if (enclosing != nullptr) { return enclosing.get()->getEnvironment(name); }
	console.get()->error("[environment:0401] Failed to locate environment of name '" + name + "' during runtime.");
	return nullptr;
}
