#include "../include/cstyle/SResolver.h"
#include "../include/cstyle/SInterpreter.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CObject.h"
#include "../include/cstyle/ChromaStyle.h"
#include "../include/cstyle/SToken.h"
#include "../include/cstyle/SProc.h"
#include "../include/entities/widgets/WidgetStyle.h"
#include "../include/cscript/CEnums.h"

#include <string>
#include <vector>
#include <exception>
#include <map>
#include <stack>
#include <filesystem>

SResolver::SResolver()
{

}

void SResolver::initialize(std::shared_ptr<ChromaStyle> console)
{
	this->console = console;
}

//
void SResolver::resolveStatements(std::vector<std::shared_ptr<SStmt>> statements,
	std::shared_ptr<CEnvironment> rootEnvironment, std::filesystem::path rootDir)
{
	currentEnvironment = rootEnvironment;
	currentRootDir = rootDir;
	for (std::shared_ptr<SStmt> statement : statements)
	{
		resolve(statement);
	}
	currentRootDir.clear();
	currentEnvironment = console.get()->scriptConsole.get()->global;
}
//
void SResolver::clearUnresolvedSymbols()
{
	unresolvedSymbols.clear();
}
//
void SResolver::checkUnresolvedSymbols()
{
	for (auto const& item : unresolvedSymbols)
	{
		if (item.second != nullptr)
		{
			console.get()->error("[resolver:0101] Unresolved style declaration at '"
				+ item.first + "'. Check spelling errors and make sure style is defined.");
		}
	}
}
//
std::string SResolver::scopeToString(std::vector<std::shared_ptr<SToken>> scopeStack)
{
	std::string out;
	for (std::shared_ptr<SToken> token : scopeStack)
	{
		out += token.get()->lexeme;
		out += "::";
	}
	return out;
}
//
std::string SResolver::getCurrentAddress(std::shared_ptr<CEnvironment> environment)
{
	std::string out;
	std::shared_ptr<CEnvironment> address = environment;
	while (address.get()->enclosing != nullptr)
	{
		out.insert(0, address.get()->_namespace + "::");
		address = address.get()->enclosing;

	}
	out.insert(0, address.get()->_namespace + "::");
	return out;
}
//
void SResolver::resolve(std::shared_ptr<SStmt> stmt)
{
	stmt->accept(*this);
}
void SResolver::resolve(std::shared_ptr<SExpr> expr)
{
	expr->accept(*this);
}

// Statements
void SResolver::visit(std::shared_ptr<SStmt_Script> stmt)
{
	// Run the script in full here.
	console.get()->scriptConsole.get()->entry(stmt.get()->script.get()->lexeme, currentEnvironment.get()->_namespace, currentRootDir);
}
void SResolver::visit(std::shared_ptr<SStmt_Style> stmt)
{
	size_t idLen = stmt.get()->identifiers.size();
	for (size_t i = 0; i < idLen; i++)
	{
		std::string styleName = "";
		// If scopeStack exists, must successfully get the style variable, or error
		if (stmt.get()->scopeStacks[i].size() != 0)
		{
			if (stmt.get()->selectors[i] != nullptr)
			{
				styleName += stmt.get()->selectors[i].get()->lexeme;
			}
			if (stmt.get()->identifiers[i] != nullptr)
			{
				styleName += stmt.get()->identifiers[i].get()->lexeme;
			}
			std::shared_ptr<CObject> fetch = currentEnvironment.get()->get(stmt.get()->scopeStacks[i], styleName);
			if (fetch == nullptr)
			{
				console.get()->error("[resolver:0101] Failed to located existing style variable '" + styleName +
					"' at the supplied scope address.");
			}
			else if (fetch.get()->objType.type != CLiteralTypes::_CStyle)
			{
				console.get()->error("[resolver:0101] Variable '" + styleName +
					"' at the supplied scope address is not a 'style' object.");
			}
		}
		// Else, create the style name, and then declare it as a new empty style
		// in the script environment
		else
		{
			
			if (stmt.get()->selectors[i] != nullptr)
			{
				styleName += stmt.get()->selectors[i].get()->lexeme;
			}
			if (stmt.get()->identifiers[i] != nullptr)
			{
				styleName += stmt.get()->identifiers[i].get()->lexeme;
			}
			currentEnvironment.get()->define(styleName, std::make_shared<CObject>(std::make_shared<WidgetStyle>()));
			unresolvedSymbols.insert(std::pair<std::string, std::shared_ptr<SStmt>>(styleName, stmt));
		}
	}
	if (idLen != 0 && stmt.get()->properties.size() != 0)
	{
		std::string styleName = "";
		if (stmt.get()->selectors[0] != nullptr)
		{
			styleName += stmt.get()->selectors[0].get()->lexeme;
		}
		if (stmt.get()->identifiers[0] != nullptr)
		{
			styleName += stmt.get()->identifiers[0].get()->lexeme;
		}
		if (unresolvedSymbols.count(styleName) == 1)
		{
			unresolvedSymbols.at(styleName) = nullptr;
		}
	}
	return;
}


// Expressions
void SResolver::visit(std::shared_ptr<SExpr_Property> expr)
{
	// Do Nothing
	return;
}
