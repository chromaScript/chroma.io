#include "../include/cstyle/SInterpreter.h"
#include "../include/cstyle/ChromaStyle.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cstyle/SForward.h"
#include "../include/cstyle/SProc.h"
#include "../include/cstyle/SToken.h"
#include "../include/cscript/CToken.h"
#include "../include/cscript/CObject.h"
#include "../include/entities/WidgetStyle.h"
#include "../include/Application.h"
#include "../include/entities/UserInterface.h"

#include <string>
#include <vector>
#include <exception>
#include <filesystem>

SInterpreter::SInterpreter()
{

}

void SInterpreter::initialize(std::shared_ptr<ChromaStyle> console)
{
	this->console = console;
}

//
void SInterpreter::interpret(std::vector<std::shared_ptr<SStmt>> statements, std::shared_ptr<CEnvironment> entryEnv, std::filesystem::path rootDir)
{
	currentEnvironment = entryEnv;
	currentRootDir = rootDir;
	for (std::shared_ptr<SStmt> stmt : statements)
	{
		stmt.get()->accept(*this);
	}
	currentRootDir.clear();
	currentEnvironment = console.get()->scriptConsole.get()->global;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Visit Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

// Statements
void SInterpreter::visit(std::shared_ptr<SStmt_Script> stmt)
{
	// Do nothing, script was run in the resolver
}
void SInterpreter::visit(std::shared_ptr<SStmt_Style> stmt)
{
	int idLen = (int)stmt.get()->identifiers.size();
	if (idLen == 0) { return; }
	std::string styleName = "";
	if (stmt.get()->selectors[0] != nullptr)
	{
		styleName += stmt.get()->selectors[0].get()->lexeme;
	}
	if (stmt.get()->identifiers[0] != nullptr)
	{
		styleName += stmt.get()->identifiers[0].get()->lexeme;
	}
	currentStyle = std::get<std::shared_ptr<WidgetStyle>>(currentEnvironment.get()->get(stmt.get()->scopeStacks[0], styleName).get()->obj);
	currentStyle.get()->name = styleName;

	if (idLen == 1)
	{
		// Apply properties
		for (std::shared_ptr<SExpr> expr : stmt.get()->properties)
		{
			expr.get()->acceptInterpreter(*this);
		}
	}
	else
	{
		// Combine styles in reverse order
		for (int i = idLen - 1; i >= 0; i--)
		{
			styleName = "";
			// If scopeStack exists, must successfully get the style variable, or error
			if (stmt.get()->selectors[(size_t)i] != nullptr)
			{
				styleName += stmt.get()->selectors[(size_t)i].get()->lexeme;
			}
			if (stmt.get()->identifiers[(size_t)i] != nullptr)
			{
				styleName += stmt.get()->identifiers[(size_t)i].get()->lexeme;
			}
			std::shared_ptr<CObject> fetch = currentEnvironment.get()->get(stmt.get()->scopeStacks[(size_t)i], styleName);
			if (fetch != nullptr)
			{
				currentStyle.get()->mergeStyle(std::get<std::shared_ptr<WidgetStyle>>(fetch.get()->obj));
			}
		}
	}
	currentStyle = nullptr;
	return;
}


// Expressions
void SInterpreter::visit(std::shared_ptr<SExpr_Property> expr)
{
	STokenType propType = expr.get()->propertyName.get()->type;
	// Make property values into a string
	// Send to getSetPropertyByName()
	std::string fieldValue = "";
	for (std::shared_ptr<SToken> token : expr.get()->values)
	{
		if (token.get()->type == STokenType::SCRIPT)
		{
			std::string scriptLine = token.get()->lexeme;
			if (scriptLine.find(';') != std::string::npos)
			{
				size_t split = scriptLine.find(';');
				scriptLine = scriptLine.substr(0, split);
				console.get()->warning(expr.get()->propertyName,
					"[interpreter:0401] Encountered unnecessary script statement terminator ';'. "
					"Trimming ';' character and any statements beyond the first. "
					"Place complex script commands inside of a 'script' statement.");
			}
			std::shared_ptr<CObject> obj = console.get()->scriptConsole.get()->entry(scriptLine, currentEnvironment.get()->_namespace, true);
			std::string objString = console.get()->scriptConsole.get()->toString(obj);
			fieldValue += objString + ",";
		}
		else if (token.get()->type == STokenType::BOOL ||
			token.get()->type == STokenType::NUM ||
			token.get()->type == STokenType::STRING)
		{
			fieldValue += token.get()->lexeme + ",";
		}
		else
		{
			fieldValue += token.get()->typeString() + ",";
		}
	}
	
	if (fieldValue.size() != 0)
	{
		// Pop-back the trailing ',' character
		fieldValue.pop_back();
		// Fix file paths
		if (propType == STokenType::BACKGROUND_TEXTURE ||
			propType == STokenType::FONT_PATH)
		{
			std::filesystem::path fieldPath = currentRootDir / fieldValue;
			fieldValue = fieldPath.string();
		}
		if (propType == STokenType::FONT_PATH)
		{
			int fontErr = console.get()->owner.get()->getUI().get()->loadFont(fieldValue);
			if (fontErr != 0)
			{
				console.get()->error(expr.get()->propertyName, 
					"[interpreter:0402] Failed to load font file at path '" + fieldValue + 
					"'. Check for spelling errors and that the font is a valid '.ttf' file.");
			}
		}
		// Set the property on the currentStyle
		currentStyle.get()->getSetProperty(
			console.get()->scriptConsole.get()->getInterpreter(),
			*expr.get()->propertyName.get(), fieldValue, false);
	}
	return;
}