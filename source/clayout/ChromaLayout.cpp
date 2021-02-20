#include "../include/clayout/LLexer.h"
#include "../include/clayout/LParser.h"
#include "../include/clayout/LInterpreter.h"
#include "../include/clayout/LResolver.h"
#include "../include/clayout/ChromaLayout.h"
#include "../include/clayout/LToken.h"
#include "../include/clayout/LEnums.h"

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

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaLayout Interpreter Constructor
//
////////////////////////////////////////////////////////////////////////////////////////////////

ChromaLayout::ChromaLayout(std::shared_ptr<Application> owner, std::shared_ptr<ChromaScript> scriptConsole)
{
	this->owner = owner;
	this->scriptConsole = scriptConsole;
	for (size_t i = 0; i < LTokenTypeNames.size(); i++)
	{
		layoutTypeMap.insert(std::pair<std::string, LTokenType>(LTokenTypeNames[i], (LTokenType)i));
	}
	lexer = std::make_shared<LLexer>();
	parser = std::make_shared<LParser>();
	resolver = std::make_shared<LResolver>();
	interpreter = std::make_shared<LInterpreter>();
}

void ChromaLayout::initializeConsole()
{
	lexer.get()->initialize(shared_from_this());
	parser.get()->initialize(shared_from_this());
	resolver.get()->initialize(shared_from_this());
	resolver.get()->setEnvironment(scriptConsole.get()->global);
	interpreter.get()->initialize(shared_from_this());
	interpreter.get()->setEnvironment(scriptConsole.get()->global);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// chromaScript Interpreter Entry Point
//
////////////////////////////////////////////////////////////////////////////////////////////////

void ChromaLayout::entry(std::vector<std::filesystem::path> paths, std::string _namespace, std::filesystem::path rootDir)
{
	hadError = false;
	resolver.get()->clearUnresolvedSymbols();
	std::vector<std::shared_ptr<LStmt>> statements;
	for (std::filesystem::path path : paths)
	{
		std::vector<std::shared_ptr<LStmt>> output = build(path, _namespace);
		statements.reserve(statements.size() + output.size());
		statements.insert(statements.end(), output.begin(), output.end());
	}
	// Currently unused
	resolver.get()->checkUnresolvedSymbols();
	resolver.get()->clearUnresolvedSymbols();
	if (hadError == true)
	{
		error("[console:0101] errror: Encountered error during compilation, aborting style-sheet."); return;
	}
	construct(statements, _namespace, rootDir);
	if (hadError == true)
	{
		error("[console:0103] errror: Encountered error during runtime."); return;
	}
}

void ChromaLayout::entry(std::map<std::filesystem::path, std::shared_ptr<WPlugin>> pathMap)
{
	hadError = false;
	resolver.get()->clearUnresolvedSymbols();
	std::map<std::shared_ptr<WPlugin>, std::vector<std::shared_ptr<LStmt>>> _namespaceMap;
	// First collect all statements associated with a top-level namespace into a map
	for (auto const& item : pathMap)
	{
		if (_namespaceMap.find(item.second) == _namespaceMap.end())
		{
			_namespaceMap.insert(
				std::pair<std::shared_ptr<WPlugin>, std::vector<std::shared_ptr<LStmt>>>(
					item.second, build(item.first, item.second.get()->script_namespace)));
		}
		else
		{
			std::vector<std::shared_ptr<LStmt>> output = build(item.first, item.second.get()->script_namespace);
			_namespaceMap.at(item.second).insert(
				_namespaceMap.at(item.second).end(), output.begin(), output.end());
		}
	}
	// Currently unused
	resolver.get()->checkUnresolvedSymbols();
	resolver.get()->clearUnresolvedSymbols();
	if (hadError == true)
	{
		error("[console:0201] errror: Encountered error during compilation, aborting script."); return;
	}
	for (auto const& item : _namespaceMap)
	{
		construct(item.second, item.first.get()->script_namespace, item.first.get()->plugin_path);
	}
	if (hadError == true)
	{
		error("[console:0203] errror: Encountered error during runtime."); return;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaLayout Compiler
//
////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::shared_ptr<LStmt>> ChromaLayout::build(std::filesystem::path scriptPath, std::string _namespace)
{
	std::string fileString;
	readFileToString(fileString, scriptPath);
	// Lex the source string into a list of tokens
	std::vector<std::shared_ptr<LToken>> tokens = lexer.get()->scanTokens(fileString);
	// Parse the tokens into a nested heirarchy of statements and expressions
	std::vector<std::shared_ptr<LStmt>> statements = parser.get()->parseTokens(tokens);
	// Set the entry environment for the resolver
	std::shared_ptr<CEnvironment> entryEnvironment = scriptConsole.get()->global.get()->lookupEnvironment(_namespace, false);
	// Use script environment and declare style variables, which are handled as instances of
	// the native 'Style' class in chromaScript and are wrappers over the CStlye class
	resolver.get()->resolveStatements(statements, entryEnvironment);
	return statements;
}

void ChromaLayout::construct(std::vector<std::shared_ptr<LStmt>> statements, std::string _namespace, std::filesystem::path rootDir)
{
	// 1. Run the script
	// Set the entry environment for the interpreter
	std::shared_ptr<CEnvironment> entryEnvironment = scriptConsole.get()->global.get()->lookupEnvironment(_namespace, false);
	interpreter.get()->constructWidgets(statements, entryEnvironment, rootDir);
}

void ChromaLayout::create()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaLayout Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

void ChromaLayout::ping(double time, std::string message)
{
	std::cout << "ChromaLayout::CONSOLE::PING=" << time << "::MESSAGE=" << message << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// ChromaLayout Error Handling
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Error builder
void ChromaLayout::error(std::string message)
{
	report("layout= " + message);
}
void ChromaLayout::error(size_t line, std::string message)
{
	std::string out =
		"layout= [line:" + std::to_string(line) + "] error: " + message;
	report(out);
}
void ChromaLayout::error(std::shared_ptr<LToken> line, std::string message)
{
	if (line != nullptr)
	{
		std::string out =
			"layout= [line:" + std::to_string(line.get()->line) + "] error: " + line.get()->typeString() + " - " + message;
		report(out);
	}
	else
	{
		std::string out =
			"layout= [line: nil] error: nil - [console] Nullptr token passed to error handler.";
		report(out);
	}
}

// Error reporter
void ChromaLayout::report(std::string message)
{
	std::cout << message << std::endl;
	hadError = true;
}

// Warning builder
void ChromaLayout::warning(std::string message)
{
	alert("layout= " + message);
}
void ChromaLayout::warning(size_t line, std::string message)
{
	std::string out =
		"layout= [line:" + std::to_string(line) + "] warning: " + message;
	alert(out);
}
void ChromaLayout::warning(std::shared_ptr<LToken> line, std::string message)
{
	std::string out =
		"layout= [line:" + std::to_string(line.get()->line) + "] warning: " + line.get()->typeString() + " - " + message;
	alert(out);
}

// Warning reporter
void ChromaLayout::alert(std::string message)
{
	std::cout << message << std::endl;
}