#include "../include/cscript/CLexer.h"
#include "../include/cscript/CParser.h"
#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CResolver.h"
#include "../include/cscript/CTypeWizard.h"
#include "../include/cscript/COptimizerRed.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CToken.h"
#include "../include/entities/WidgetStyle.h"

#include "../include/Application.h"
#include "../include/entities/UserInterface.h"

#include "../include/structs.h"

#include "glm.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <variant>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaScript Interpreter Constructor
//
////////////////////////////////////////////////////////////////////////////////////////////////

ChromaScript::ChromaScript(std::shared_ptr<Application> owner)
{
	this->owner = owner;
	lexer = std::make_shared<CLexer>();
	parser = std::make_shared<CParser>();
	resolver = std::make_shared<CResolver>();
	typeWizard = std::make_shared<CTypeWizard>();
	optimizerRed = std::make_shared<COptimizerRed>();
	interpreter = std::make_shared<CInterpreter>();

	for (size_t i = 0; i < CCallbackNames.size(); i++)
	{
		CCallbackMap.insert(std::pair<std::string, CCallbackType>(CCallbackNames[i], (CCallbackType)i));
	}
}

void ChromaScript::initializeConsole()
{
	global = std::make_shared<CEnvironment>(nullptr, shared_from_this(), "global");

	lexer.get()->initialize(shared_from_this());
	parser.get()->initialize(shared_from_this());
	resolver.get()->initialize(shared_from_this());
	resolver.get()->setEnvironment(global);
	typeWizard.get()->initialize(shared_from_this());
	typeWizard.get()->setEnvironment(global);
	optimizerRed.get()->initialize(shared_from_this());
	optimizerRed.get()->setEnvironment(global);
	interpreter.get()->initialize(shared_from_this());
	interpreter.get()->setEnvironment(global);

	loadLibraries(global);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaScript Interpreter Entry Point
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Multi/Single line script entry point - direct injection into expressionStmt with ignoreSemicolon
// Skips execute/run step and returns the statements. Used for binding scripts into widgets or other
// targets where the script should not be ran immediately.
std::vector<std::shared_ptr<CStmt>> ChromaScript::entry_compileOnly(std::string script, std::string _namespace, bool isExpr)
{
	hadError = false;
	resolver.get()->clearUnresolvedSymbols();
	std::vector<std::shared_ptr<CStmt>> statements = build(script, _namespace, isExpr);
	if (hadError == true)
	{
		error("[console:0101] errror: Encountered error during compilation, aborting script.");
		statements.clear();
		return statements;
	}
	resolver.get()->checkUnresolvedSymbols();
	resolver.get()->clearUnresolvedSymbols();
	compile(statements, _namespace);
	if (hadError == true)
	{
		error("[console:0102] errror: Encountered error during compilation, aborting script.");
		statements.clear();
		return statements;
	}
	return statements;
}

// Single line script entry point - direct injection into expressionStmt with ignoreSemicolon
std::shared_ptr<CObject> ChromaScript::entry(std::string script, std::string _namespace, bool isExpr)
{
	hadError = false;
	resolver.get()->clearUnresolvedSymbols();
	std::vector<std::shared_ptr<CStmt>> statements = build(script, _namespace, isExpr);
	if (hadError == true)
	{
		error("[console:0101] errror: Encountered error during compilation, aborting script."); 
		return std::make_shared<CObject>(nullptr);
	}
	resolver.get()->checkUnresolvedSymbols();
	resolver.get()->clearUnresolvedSymbols();
	compile(statements, _namespace);
	if (hadError == true)
	{
		error("[console:0102] errror: Encountered error during compilation, aborting script.");
		return std::make_shared<CObject>(nullptr);
	}
	// Using execute in order to get a return value
	std::shared_ptr<CObject> returnObj = execute(statements, _namespace);
	if (hadError == true)
	{
		error("[console:0103] errror: Encountered error during runtime.");
		return std::make_shared<CObject>(nullptr);
	}
	return returnObj;
}

// Pre-made multi-line script entry point
void ChromaScript::entry(std::string script, std::string _namespace, std::filesystem::path rootDir)
{
	hadError = false;
	resolver.get()->clearUnresolvedSymbols();
	std::vector<std::shared_ptr<CStmt>> statements = build(script, _namespace, false);
	if (hadError == true)
	{
		error("[console:0101] errror: Encountered error during compilation, aborting script."); return;
	}
	resolver.get()->checkUnresolvedSymbols();
	resolver.get()->clearUnresolvedSymbols();
	compile(statements, _namespace);
	if (hadError == true)
	{
		error("[console:0102] errror: Encountered error during compilation, aborting script."); return;
	}
	run(statements, _namespace);
	if (hadError == true)
	{
		error("[console:0103] errror: Encountered error during runtime."); return;
	}
}

// Multiple file entry point
void ChromaScript::entry(std::vector<std::filesystem::path> paths, std::string _namespace, std::filesystem::path rootDir)
{
	hadError = false;
	resolver.get()->clearUnresolvedSymbols();
	std::vector<std::shared_ptr<CStmt>> statements;
	for (std::filesystem::path path : paths)
	{
		std::vector<std::shared_ptr<CStmt>> output = build(path, _namespace, false);
		statements.reserve(statements.size() + output.size());
		statements.insert(statements.end(), output.begin(), output.end());
	}
	if (hadError == true)
	{
		error("[console:0101] errror: Encountered error during compilation, aborting script."); return;
	}
	resolver.get()->checkUnresolvedSymbols();
	resolver.get()->clearUnresolvedSymbols();
	compile(statements, _namespace);
	if (hadError == true) 
	{ 
		error("[console:0102] errror: Encountered error during compilation, aborting script."); return; 
	}
	run(statements, _namespace);
	if (hadError == true) 
	{
		error("[console:0103] errror: Encountered error during runtime."); return; 
	}
}

// Multi-plugin batch compmiler entry point
void ChromaScript::entry(std::map<std::filesystem::path, std::shared_ptr<WPlugin>> pathMap)
{
	hadError = false;
	resolver.get()->clearUnresolvedSymbols();
	std::map<std::string, std::vector<std::shared_ptr<CStmt>>> _namespaceMap;
	// First collect all statements associated with a top-level namespace into a map
	for (auto const& item : pathMap)
	{
		if (_namespaceMap.find(item.second.get()->script_namespace) == _namespaceMap.end())
		{
			_namespaceMap.insert(std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
				item.second.get()->script_namespace, build(item.first, item.second.get()->script_namespace, false)));
		}
		else
		{
			std::vector<std::shared_ptr<CStmt>> output = build(item.first, item.second.get()->script_namespace, false);
			_namespaceMap.at(item.second.get()->script_namespace).insert(
				_namespaceMap.at(item.second.get()->script_namespace).end(), output.begin(), output.end());
		}
	}
	if (hadError == true)
	{
		error("[console:0201] errror: Encountered error during compilation, aborting script."); return;
	}
	resolver.get()->checkUnresolvedSymbols();
	resolver.get()->clearUnresolvedSymbols();
	// Then compile and run each statement collection per namespace
	for (auto const& item : _namespaceMap)
	{
		compile(item.second, item.first);
	}
	if (hadError == true) 
	{ 
		error("[console:0202] errror: Encountered error during compilation, aborting script."); return; 
	}
	for (auto const& item : _namespaceMap)
	{
		run(item.second, item.first);
	}
	if (hadError == true) 
	{ 
		error("[console:0203] errror: Encountered error during runtime."); return; 
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaScript Compiler
//
////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::shared_ptr<CStmt>> ChromaScript::build(std::filesystem::path scriptPath, std::string _namespace, bool isExpr)
{
	std::string fileString;
	readFileToString(fileString, scriptPath);
	return build(fileString, _namespace, isExpr);
}
std::vector<std::shared_ptr<CStmt>> ChromaScript::build(std::string script, std::string _namespace, bool isExpr)
{
	// Lex the source string into a list of tokens
	std::vector<std::shared_ptr<CToken>> tokens = lexer.get()->scanTokens(script);
	// Parse the tokens into a nested heirarchy of statements and expressions
	std::vector<std::shared_ptr<CStmt>> statements;
	if (isExpr) { statements = parser.get()->parseTokensExpr(tokens); }
	else { statements = parser.get()->parseTokens(tokens); }
	// Set the entry environment for the resolver
	std::shared_ptr<CEnvironment> entryEnvironment = global.get()->lookupEnvironment(_namespace, false);
	// Generate environments and declare variables, define functions/classes
	resolver.get()->resolveStatements(statements, entryEnvironment);
	return statements;
}

void ChromaScript::compile(std::vector<std::shared_ptr<CStmt>> statements, std::string _namespace)
{
	// 1. Perform type-checking
	// Set the entry environment for the typeWizard
	std::shared_ptr<CEnvironment> entryEnvironment = global.get()->lookupEnvironment(_namespace, false);
	typeWizard.get()->checkTypes(statements, entryEnvironment);
	// 2. Perform optimizations
	optimizerRed.get()->optimize(statements, entryEnvironment);
	//
	//
}

void ChromaScript::run(std::vector<std::shared_ptr<CStmt>> statements, std::string _namespace)
{
	// 1. Run the script
	// Set the entry environment for the interpreter
	std::shared_ptr<CEnvironment> entryEnvironment = global.get()->lookupEnvironment(_namespace, false);
	interpreter.get()->interpret(statements, entryEnvironment);
}

// Same as run, but used for executing single line commands that expect return values
std::shared_ptr<CObject> ChromaScript::execute(std::vector<std::shared_ptr<CStmt>> statements, std::string _namespace)
{
	// 1. Execute the script line
	// Set the entry environment for the interpreter
	std::shared_ptr<CEnvironment> entryEnvironment = global.get()->lookupEnvironment(_namespace, false);
	return interpreter.get()->interpretLine(statements, entryEnvironment);
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaScript Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void ChromaScript::ping(double time, std::string message)
{
	std::cout << "ChromaScript::CONSOLE::PING=" << time << "::MESSAGE=" << message << std::endl;
}

void ChromaScript::print(std::string message)
{
	std::cout << message << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaScript Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void ChromaScript::loadLibraries(std::shared_ptr<CEnvironment> env)
{
	// Note: May also need to define an environment for the function as well, just to avoid
	// potential problems later (ie. user named class named "clock"?)
	env.get()->define("clock", 
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fClock, 
			global.get()->lookupEnvironment("clock", true)));
	env.get()->define("toString",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fToString,
			global.get()->lookupEnvironment("toString", true)));
	env.get()->define("fibonacci", 
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fFibonacci, 
			global.get()->lookupEnvironment("fibonacci", true)));
	env.get()->define("lineIntersect2D",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fLineIntersect2D,
			global.get()->lookupEnvironment("lineIntersect2D", true)));
	env.get()->define("clamp",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fClamp,
			global.get()->lookupEnvironment("clamp", true)));
	env.get()->define("floor",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fFloor,
			global.get()->lookupEnvironment("floor", true)));
	env.get()->define("lerp",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fLerp,
			global.get()->lookupEnvironment("lerp", true)));
	env.get()->define("RGB_toHSL",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fRGB_toHSL,
			global.get()->lookupEnvironment("RGB_toHSL", true)));
	env.get()->define("HSL_toRGB",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fHSL_toRGB,
			global.get()->lookupEnvironment("HSL_toRGB", true)));
	env.get()->define("hueByDegree_HSL",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fHueByDegree_HSL,
			global.get()->lookupEnvironment("hueByDegree_HSL", true)));
	env.get()->define("saturationByPercentDegree_HSL",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fSaturationByPercentDegree_HSL,
			global.get()->lookupEnvironment("saturationByPercentDegree_HSL", true)));
	env.get()->define("valueByPercent_HSL",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fValueByPercent_HSL,
			global.get()->lookupEnvironment("valueByPercent_HSL", true)));
	env.get()->define("bindTimerEvent",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fBindTimerEvent,
			global.get()->lookupEnvironment("bindTimerEvent", true),
			owner));
	env.get()->define("cancelTimerEvent",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fCancelTimerEvent,
			global.get()->lookupEnvironment("cancelTimerEvent", true),
			owner));
	env.get()->define("bindCallbackEvent",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fBindCallbackEvent,
			global.get()->lookupEnvironment("bindCallbackEvent", true),
			owner));
	env.get()->define("getMousePos",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fGetMousePos,
			global.get()->lookupEnvironment("getMousePos", true),
			owner));
	env.get()->define("getMousePos_relative",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fGetMousePos_relative,
			global.get()->lookupEnvironment("getMousePos_relative", true),
			owner));
	env.get()->define("hitTestWidget_byID",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fHitTestWidget_byID,
			global.get()->lookupEnvironment("hitTestWidget_byID", true),
			owner));
	env.get()->define("sweepTestRoot_byID",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fSweepTestRoot_byID,
			global.get()->lookupEnvironment("sweepTestRoot_byID", true),
			owner));
	env.get()->define("setProperty", 
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fSetProperty, 
			global.get()->lookupEnvironment("setProperty", true),
			owner));
	env.get()->define("getProperty", 
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fGetProperty, 
			global.get()->lookupEnvironment("getProperty", true),
			owner));
	env.get()->define("setChildProperty_byClass",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fSetChildProperty_byClass,
			global.get()->lookupEnvironment("setChildProperty_byClass", true),
			owner));
	env.get()->define("setChildProperty_byType",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fSetChildProperty_byType,
			global.get()->lookupEnvironment("setChildProperty_byType", true),
			owner));
	env.get()->define("setChildProperty_byID",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fSetChildProperty_byID,
			global.get()->lookupEnvironment("setChildProperty_byID", true),
			owner));
	env.get()->define("setChildProperty_byName",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fSetChildProperty_byName,
			global.get()->lookupEnvironment("setChildProperty_byName", true),
			owner));
	env.get()->define("setClassProperty",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fSetClassProperty,
			global.get()->lookupEnvironment("setClassProperty", true),
			owner));
	env.get()->define("setClassProperty_excludingActive",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fSetClassProperty_excludingActive,
			global.get()->lookupEnvironment("setClassProperty_excludingActive", true),
			owner));
	env.get()->define("setClassProperty_excludingID",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fSetClassProperty_excludingID,
			global.get()->lookupEnvironment("setClassProperty_excludingID", true),
			owner));
	env.get()->define("resetProperty",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fResetProperty,
			global.get()->lookupEnvironment("resetProperty", true),
			owner));
	env.get()->define("resetClassProperty",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_fResetClassProperty,
			global.get()->lookupEnvironment("resetClassProperty", true),
			owner));

	// Define the native classes from smallest component to largest. If one contains another, the small
	// object must appear before the container.

	// Style Class
	env.get()->define("Style",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_cStyle,
			env.get()->lookupEnvironment("Style", true)));
	// Widget Class
	env.get()->define("Widget",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_cWidget,
			env.get()->lookupEnvironment("Widget", true)));
	// Toolbox Class
	env.get()->define("Toolbox",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_cToolbox,
			env.get()->lookupEnvironment("Toolbox", true),
			owner));
	// Ui Class
	env.get()->define("Ui",
		std::make_shared<CObject>(
			CCallableTypes::_CStd_cUi,
			env.get()->lookupEnvironment("Ui", true),
			owner));
	// App Class
	env.get()->define("App", 
		std::make_shared<CObject>(
			CCallableTypes::_CStd_cApp, 
			env.get()->lookupEnvironment("App", true), 
			owner));
	// Default App Instance
	env.get()->define("app", 
		std::make_shared<CObject>(
			env, 
			"app",
			std::get<std::shared_ptr<CClass>>(env.get()->get("App").get()->obj)));
	
	return;
}



////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaScript Error Handling
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Error builder
void ChromaScript::error(std::string message)
{
	report("cscript= " + message);
}
void ChromaScript::error(size_t line, std::string message)
{
	std::string out =
		"cscript= [line:" + std::to_string(line) + "] error: " + message;
	report(out);
}
void ChromaScript::error(std::shared_ptr<CToken> line, std::string message)
{
	if (line != nullptr)
	{
		std::string out =
			"cscript= [line:" + std::to_string(line.get()->line) + "] error: " + line.get()->typeString() + " - " + message;
		report(out);
	}
	else
	{
		std::string out =
			"cscript= [line: nil] error: nil - [console] Nullptr token passed to error handler.";
		report(out);
	}
}

// Error reporter
void ChromaScript::report(std::string message)
{
	std::cout << message << std::endl;
	hadError = true;
}

// Warning builder
void ChromaScript::warning(std::string message)
{
	alert("cscript= " + message);
}
void ChromaScript::warning(size_t line, std::string message)
{
	std::string out =
		"cscript= [line:" + std::to_string(line) + "] warning: " + message;
	alert(out);
}
void ChromaScript::warning(std::shared_ptr<CToken> line, std::string message)
{
	std::string out =
		"cscript= [line:" + std::to_string(line.get()->line) + "] warning: " + line.get()->typeString() + " - " + message;
	alert(out);
}

// Warning reporter
void ChromaScript::alert(std::string message)
{
	std::cout << message << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaScript Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Update WidgetIDTable
bool ChromaScript::insertWidgetTableID(std::string id, std::string rootName)
{
	if (owner.get()->getUI().get()->widgetIDTable.count(id) == 0)
	{
		owner.get()->getUI().get()->widgetIDTable.insert(std::pair<std::string, std::string>(id, rootName));
		return true;
	}
	else
	{
		error("[console:1201] Widget with unique id of '" + 
			id + "' already exists, cannot add new widget with this id.");
		return false;
	}
}
void ChromaScript::insertWidgetTableClass(std::string className, std::string rootName)
{
	// Check if new entry
	if (owner.get()->getUI().get()->widgetClassTable.count(className) == 0)
	{
		owner.get()->getUI().get()->widgetClassTable.insert(std::pair<std::string, std::vector<std::string>>(className, { rootName }));
	}
	else
	{
		// Class is already in the table, check if root is already in the list
		bool rootExists = false;
		for (std::string root : owner.get()->getUI().get()->widgetClassTable.at(className))
		{
			if (rootName == root) { rootExists = true; break; }
		}
		if (!rootExists)
		{
			// Add new rootName to value
			owner.get()->getUI().get()->widgetClassTable.at(className).push_back(rootName);
		}
	}
}
bool ChromaScript::insertPrototypeTableID(std::string id)
{
	if (owner.get()->getUI().get()->prototypeFactoryTable.count(id) == 0)
	{
		owner.get()->getUI().get()->prototypeFactoryTable.insert(std::pair<std::string, std::shared_ptr<Widget>>(id, nullptr));
		return true;
	}
	else
	{
		error("[console:1201] Widget with unique id of '" +
			id + "' already exists, cannot add new widget with this id.");
		return false;
	}
}

// Strong Copy Object
std::shared_ptr<CObject> ChromaScript::copyObj(std::shared_ptr<CObject> right, std::string name)
{
	if (right.get()->obj.index() == 0) { return right; }
	std::shared_ptr<std::vector<std::shared_ptr<CObject>>> leftArr = std::make_shared<std::vector<std::shared_ptr<CObject>>>();
	std::vector<std::shared_ptr<CObject>> rightArr;
	std::shared_ptr<CInstance> rightInstance;
	std::shared_ptr<CInstance> leftInstance;
	std::shared_ptr<CObject> leftObj;
	std::string _namespace = name == "" ? randomNamespace(anonLength) : name;
	switch (right.get()->objType.type)
	{
	case CLiteralTypes::_CString:
		return std::make_shared<CObject>(std::get<std::string>(right.get()->obj)); break;
	case CLiteralTypes::_CBool:
		return std::make_shared<CObject>(std::get<bool>(right.get()->obj)); break;
	case CLiteralTypes::_CNumber:
		return std::make_shared<CObject>(std::get<double>(right.get()->obj)); break;
	case CLiteralTypes::_CString_Array:
		rightArr = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(right.get()->obj);
		for (size_t i = 0; i < rightArr.size(); i++)
		{
			leftArr.get()->push_back(std::make_shared<CObject>(std::get<std::string>(rightArr[i].get()->obj)));
		}
		return std::make_shared<CObject>(CLiteralTypes::_CString_Array, leftArr); break;
	case CLiteralTypes::_CBool_Array:
		rightArr = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(right.get()->obj);
		for (size_t i = 0; i < rightArr.size(); i++)
		{
			leftArr.get()->push_back(std::make_shared<CObject>(std::get<bool>(rightArr[i].get()->obj)));
		}
		return std::make_shared<CObject>(CLiteralTypes::_CBool_Array, leftArr); break;
	case CLiteralTypes::_CNumber_Array:
		rightArr = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(right.get()->obj);
		for (size_t i = 0; i < rightArr.size(); i++)
		{
			leftArr.get()->push_back(std::make_shared<CObject>(std::get<double>(rightArr[i].get()->obj)));
		}
		return std::make_shared<CObject>(CLiteralTypes::_CNumber_Array, leftArr); break;
	case CLiteralTypes::_CInstance:
		rightInstance = std::get<std::shared_ptr<CInstance>>(right.get()->obj);
		leftObj = std::make_shared<CObject>(interpreter.get()->getEnvironment(), _namespace, rightInstance);
		return leftObj; break;
	case CLiteralTypes::_CInstance_Array:
		rightArr = *std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(right.get()->obj);
		for (size_t i = 0; i < rightArr.size(); i++)
		{
			leftArr.get()->push_back(copyObj(rightArr[i], rightArr[i].get()->objName));
		}
		return std::make_shared<CObject>(CLiteralTypes::_CInstance_Array, leftArr);; break;
	case CLiteralTypes::_CFunction:
		return right;
	case CLiteralTypes::_CFunction_Array:
		error("[console:1902] Encountered unfinished area of the program.");
		return right;
	case CLiteralTypes::_CVec2:
	case CLiteralTypes::_CVec3:
	case CLiteralTypes::_CVec4:
	case CLiteralTypes::_CVec2_Array:
	case CLiteralTypes::_CVec3_Array:
	case CLiteralTypes::_CVec4_Array:
		error("[console:1903] Encountered unfinished area of the program.");
		return right;
	case CLiteralTypes::_CStyle:
		return std::make_shared<CObject>(std::get<std::shared_ptr<WidgetStyle>>(right.get()->obj).get()->makeCopy());
	case CLiteralTypes::_CWidget:
		// Must leave as reflexive return.
		return right;
	case CLiteralTypes::_CToolbox:
		// Must leave as reflexive return.
		return right;
	case CLiteralTypes::_CUi:
		// Do not copy the ui!
		//warning("[console:1905] Should not instantiate new 'Ui' object. "
		//	"All 'Ui' class objects point to the same UI instance. "
		//	"Use existing 'Ui' instance from 'app.ui' to perform operations with the 'Ui' class.");
		return right;
	case CLiteralTypes::_CApp:
		// Do not copy the application!
		//warning("[console:1905] Should not instantiate new 'App' object. "
		//	"All 'App' class objects point to the same Application instance. "
		//	"Use existing 'app' instance to perform operations with the 'App' class.");
		return right;
	case CLiteralTypes::_CNil:
	default:
		return right;
	}
}



// Print Statement
void ChromaScript::printObj(std::shared_ptr<CObject> obj)
{
	print(toString(obj));
}

// Retrieve CAny value as string
std::string ChromaScript::toString(std::shared_ptr<CObject> obj)
{
	if (obj == nullptr || obj.get()->obj.index() == 0) 
	{ 
		return "nil"; 
	}
	switch (obj.get()->objType.type)
	{
	case CLiteralTypes::_CBool:
		return boolToString(std::get<bool>(obj.get()->obj));
		break;
	case CLiteralTypes::_CNumber:
		return numToString(std::get<double>(obj.get()->obj));
		break;
	case CLiteralTypes::_CString:
		return std::get<std::string>(obj.get()->obj);
		break;
	case CLiteralTypes::_CBool_Array:
	case CLiteralTypes::_CNumber_Array:
	case CLiteralTypes::_CString_Array:
		return arrayToString(obj);
		break;
	case CLiteralTypes::_CVec2:
	case CLiteralTypes::_CVec3:
	case CLiteralTypes::_CVec4:
		return vecToString(obj);
		break;
	case CLiteralTypes::_CVec2_Array:
	case CLiteralTypes::_CVec3_Array:
	case CLiteralTypes::_CVec4_Array:
		return arrayToString(obj);
		break;
	case CLiteralTypes::_CFunction:
		return std::get<std::shared_ptr<CFunction>>(obj.get()->obj).get()->toString();
		break;
	case CLiteralTypes::_CClass:
		return std::get<std::shared_ptr<CClass>>(obj.get()->obj).get()->toString();
		break;
	case CLiteralTypes::_CInstance:
		return std::get<std::shared_ptr<CInstance>>(obj.get()->obj).get()->toString();
		break;
	default:
		return "nil";
		break;
	}
}
std::string ChromaScript::numToString(double d)
{
	if (d == 0) { return "0"; }
	else if (floor(d) / d == 1) { return std::to_string(int(d)); }
	else 
	{ 
		std::string out = std::to_string(d);
		while (out.back() == '0' && out.back() != '.') { out.erase(out.size() - 1, std::string::npos); }
		return out; 
	}
}
std::string ChromaScript::numToString(float f)
{
	return numToString((double)f);
}
std::string ChromaScript::boolToString(bool b)
{
	if (b == true) { return "true"; }
	else { return "false"; }
}
std::string ChromaScript::vecToString(std::shared_ptr<CObject> obj)
{
	std::string out;
	CLiteralTypes type = obj.get()->objType.type;
	if (type == CLiteralTypes::_CVec2)
	{
		glm::dvec2 objVec2 = std::get<glm::dvec2>(obj.get()->obj);
		out = numToString(objVec2.x) + "," + numToString(objVec2.y);
		return out;
	}
	else if (type == CLiteralTypes::_CVec3)
	{
		glm::dvec3 objVec3 = std::get<glm::dvec3>(obj.get()->obj);
		out = numToString(objVec3.x) + "," + numToString(objVec3.y) + "," + numToString(objVec3.z);
		return out;
	}
	else if (type == CLiteralTypes::_CVec4)
	{
		glm::dvec4 objVec4 = std::get<glm::dvec4>(obj.get()->obj);
		out = numToString(objVec4.x) + "," + numToString(objVec4.y) + "," + numToString(objVec4.z) + "," + numToString(objVec4.w);
		return out;
	}
	else { return "nil"; }
}

// Array to String
std::string ChromaScript::arrayToString(std::shared_ptr<CObject> obj)
{
	std::string out = "";
	if (obj.get()->objType.type == CLiteralTypes::_CBool_Array)
	{
		std::shared_ptr<std::vector<std::shared_ptr<CObject>>> arr = std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(obj.get()->obj);
		size_t arrSize = arr.get()->size();
		for (size_t i = 0; i < arrSize; i++)
		{
			if (i == arrSize - 1)
			{
				out += boolToString(std::get<bool>(arr.get()->at(i).get()->obj));
				continue;
			}
			out += boolToString(std::get<bool>(arr.get()->at(i).get()->obj)) + ",";
		}
	}
	else if (obj.get()->objType.type == CLiteralTypes::_CNumber_Array)
	{
		std::shared_ptr<std::vector<std::shared_ptr<CObject>>> arr = std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(obj.get()->obj);
		size_t arrSize = arr.get()->size();
		for (size_t i = 0; i < arrSize; i++)
		{
			if (i == arrSize - 1)
			{
				out += numToString(std::get<double>(arr.get()->at(i).get()->obj));
				continue;
			}
			out += numToString(std::get<double>(arr.get()->at(i).get()->obj)) + ",";
		}
	}
	else if (obj.get()->objType.type == CLiteralTypes::_CString_Array)
	{
		std::shared_ptr<std::vector<std::shared_ptr<CObject>>> arr = std::get<std::shared_ptr<std::vector<std::shared_ptr<CObject>>>>(obj.get()->obj);
		size_t arrSize = arr.get()->size();
		for (size_t i = 0; i < arrSize; i++)
		{
			if (i == arrSize - 1)
			{
				out += std::get<std::string>(arr.get()->at(i).get()->obj);
				continue;
			}
			out += std::get<std::string>(arr.get()->at(i).get()->obj) + ",";
		}
	}
	else if (obj.get()->objType.type == CLiteralTypes::_CVec2_Array)
	{
		out = "unfinished";
	}
	else if (obj.get()->objType.type == CLiteralTypes::_CVec3_Array)
	{
		out = "unfinished";
	}
	else if (obj.get()->objType.type == CLiteralTypes::_CFunction_Array)
	{
		out = "unfinished";
	}
	else if (obj.get()->objType.type == CLiteralTypes::_CInstance_Array)
	{
		out = "unfinished";
	}
	else
	{
		out = "nil";
	}
	return out;
}