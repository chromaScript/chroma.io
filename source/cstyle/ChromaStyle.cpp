#include "../include/cstyle/SLexer.h"
#include "../include/cstyle/SParser.h"
#include "../include/cstyle/SInterpreter.h"
#include "../include/cstyle/SResolver.h"
#include "../include/cstyle/ChromaStyle.h"
#include "../include/cstyle/SToken.h"
#include "../include/cstyle/SEnums.h"
#include "../include/entities/WidgetStyle.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/ChromaScript.h"

#include "../include/Application.h"

#include "../include/structs.h"

#include "glm.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <variant>
#include <filesystem>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaStyle Interpreter Constructor
//
////////////////////////////////////////////////////////////////////////////////////////////////

ChromaStyle::ChromaStyle(std::shared_ptr<Application> owner, std::shared_ptr<ChromaScript> scriptConsole)
{
	this->owner = owner;
	this->scriptConsole = scriptConsole;
	for (size_t i = 0; i < STokenTypeNames.size(); i++)
	{
		styleTypeMap.insert(std::pair<std::string, STokenType>(STokenTypeNames[i], (STokenType)i));
	}
	lexer = std::make_shared<SLexer>();
	parser = std::make_shared<SParser>();
	resolver = std::make_shared<SResolver>();
	interpreter = std::make_shared<SInterpreter>();
}

void ChromaStyle::initializeConsole()
{
	lexer.get()->initialize(shared_from_this());
	parser.get()->initialize(shared_from_this());
	resolver.get()->initialize(shared_from_this());
	resolver.get()->setEnvironment(scriptConsole.get()->global);
	interpreter.get()->initialize(shared_from_this());
	interpreter.get()->currentEnvironment = scriptConsole.get()->global;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaStyle Interpreter Entry Point
//
////////////////////////////////////////////////////////////////////////////////////////////////

void ChromaStyle::entry(std::vector<std::filesystem::path> paths, std::string _namespace, std::filesystem::path rootDir)
{
	hadError = false;
	resolver.get()->clearUnresolvedSymbols();
	std::vector<std::shared_ptr<SStmt>> statements;
	for (std::filesystem::path path : paths)
	{
		std::vector<std::shared_ptr<SStmt>> output = make(path, _namespace, rootDir);
		statements.reserve(statements.size() + output.size());
		statements.insert(statements.end(), output.begin(), output.end());
	}
	resolver.get()->checkUnresolvedSymbols();
	resolver.get()->clearUnresolvedSymbols();
	if (hadError == true)
	{
		error("[console:0101] errror: Encountered error during compilation, aborting style-sheet."); return;
	}
	build(statements, _namespace, rootDir);
	if (hadError == true)
	{
		error("[console:0103] errror: Encountered error during runtime."); return;
	}
}

void ChromaStyle::entry(std::map<std::filesystem::path, std::shared_ptr<WPlugin>> pathMap)
{
	hadError = false;
	resolver.get()->clearUnresolvedSymbols();
	std::map<std::shared_ptr<WPlugin>, std::vector<std::shared_ptr<SStmt>>> _namespaceMap;
	// First collect all statements associated with a top-level namespace into a map
	for (auto const& item : pathMap)
	{
		if (_namespaceMap.find(item.second) == _namespaceMap.end())
		{
			_namespaceMap.insert(
				std::pair<std::shared_ptr<WPlugin>, std::vector<std::shared_ptr<SStmt>>>(
					item.second, make(item.first, item.second.get()->script_namespace, item.second.get()->plugin_path)));
		}
		else
		{
			std::vector<std::shared_ptr<SStmt>> output = make(item.first, item.second.get()->script_namespace, item.second.get()->plugin_path);
			_namespaceMap.at(item.second).insert(
				_namespaceMap.at(item.second).end(), output.begin(), output.end());
		}
	}
	resolver.get()->checkUnresolvedSymbols();
	resolver.get()->clearUnresolvedSymbols();
	if (hadError == true)
	{
		error("[console:0201] errror: Encountered error during compilation, aborting script."); return;
	}
	for (auto const& item : _namespaceMap)
	{
		build(item.second, item.first.get()->script_namespace, item.first.get()->plugin_path);
	}
	if (hadError == true)
	{
		error("[console:0203] errror: Encountered error during runtime."); return;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaStyle Compiler
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::shared_ptr<SStmt>> ChromaStyle::make(std::filesystem::path scriptPath, std::string _namespace, std::filesystem::path rootDir)
{
	std::string fileString;
	readFileToString(fileString, scriptPath);
	// Lex the source string into a list of tokens
	std::vector<std::shared_ptr<SToken>> tokens = lexer.get()->scanTokens(fileString);
	// Parse the tokens into a nested heirarchy of statements and expressions
	std::vector<std::shared_ptr<SStmt>> statements = parser.get()->parseTokens(tokens);
	// Set the entry environment for the resolver
	std::shared_ptr<CEnvironment> entryEnvironment = scriptConsole.get()->global.get()->lookupEnvironment(_namespace, false);
	// Use script environment and declare style variables, which are handled as instances of
	// the native 'Style' class in chromaScript and are wrappers over the CStlye class
	resolver.get()->resolveStatements(statements, entryEnvironment, rootDir);
	return statements;
}

void ChromaStyle::build(std::vector<std::shared_ptr<SStmt>> statements, std::string _namespace, std::filesystem::path rootDir)
{
	// 1. Run the script
	// Set the entry environment for the interpreter
	std::shared_ptr<CEnvironment> entryEnvironment = scriptConsole.get()->global.get()->lookupEnvironment(_namespace, false);
	interpreter.get()->interpret(statements, entryEnvironment, rootDir);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaStyle Constructor
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::shared_ptr<WidgetStyle> ChromaStyle::create(std::string styleString, std::string _namespace, std::filesystem::path rootDir)
{
	std::shared_ptr<WidgetStyle> style = std::make_shared<WidgetStyle>();
	// Lex the source string into a list of tokens
	std::vector<std::shared_ptr<SToken>> tokens = lexer.get()->scanTokens("voidStyle {" + styleString + "}");
	tokens.front().get()->lexeme = "@voidStyle";
	// Parse the tokens into a nested heirarchy of statements and expressions
	std::vector<std::shared_ptr<SStmt>> statements = parser.get()->parseTokens(tokens);
	// Resolve the style snippet
	resolver.get()->resolveStatements(statements, scriptConsole.get()->global, rootDir);
	// Interpret the style snippet
	interpreter.get()->interpret(statements, scriptConsole.get()->global, rootDir);
	*style = *std::get<std::shared_ptr<WidgetStyle>>(scriptConsole.get()->global.get()->get("@voidStyle").get()->obj);
	style.get()->name = "unassociated_empty";
	std::get<std::shared_ptr<WidgetStyle>>(scriptConsole.get()->global.get()->get("@voidStyle").get()->obj).reset();
	scriptConsole.get()->global.get()->values.erase("@voidStyle");
	return style;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaStyle Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void ChromaStyle::ping(double time, std::string message)
{
	std::cout << "ChromaStyle::CONSOLE::PING=" << time << "::MESSAGE=" << message << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaStyle Error Handling
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Error builder
void ChromaStyle::error(std::string message)
{
	report("style= " + message);
}
void ChromaStyle::error(size_t line, std::string message)
{
	std::string out =
		"style= [line:" + std::to_string(line) + "] error: " + message;
	report(out);
}
void ChromaStyle::error(std::shared_ptr<SToken> line, std::string message)
{
	if (line != nullptr)
	{
		std::string out =
			"style= [line:" + std::to_string(line.get()->line) + "] error: " + line.get()->typeString() + " - " + message;
		report(out);
	}
	else
	{
		std::string out =
			"style= [line: nil] error: nil - [console] Nullptr token passed to error handler.";
		report(out);
	}
}

// Error reporter
void ChromaStyle::report(std::string message)
{
	std::cout << message << std::endl;
	hadError = true;
}

// Warning builder
void ChromaStyle::warning(std::string message)
{
	alert("style= " + message);
}
void ChromaStyle::warning(size_t line, std::string message)
{
	std::string out =
		"style= [line:" + std::to_string(line) + "] warning: " + message;
	alert(out);
}
void ChromaStyle::warning(std::shared_ptr<SToken> line, std::string message)
{
	std::string out =
		"style= [line:" + std::to_string(line.get()->line) + "] warning: " + line.get()->typeString() + " - " + message;
	alert(out);
}

// Warning reporter
void ChromaStyle::alert(std::string message)
{
	std::cout << message << std::endl;
}