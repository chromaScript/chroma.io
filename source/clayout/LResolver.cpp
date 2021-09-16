#include "../include/clayout/LResolver.h"
#include "../include/clayout/LInterpreter.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CObject.h"
#include "../include/clayout/ChromaLayout.h"
#include "../include/clayout/LToken.h"
#include "../include/clayout/LProc.h"
#include "../include/entities/WidgetStyle.h"
#include "../include/cscript/CEnums.h"
#include "../include/entities/Widget.h"

#include "../include/structs.h"

#include <string>
#include <vector>
#include <exception>
#include <map>
#include <stack>

LResolver::LResolver()
{

}

void LResolver::initialize(std::shared_ptr<ChromaLayout> console)
{
	this->console = console;
}

//
void LResolver::resolveStatements(std::vector<std::shared_ptr<LStmt>> statements,
	std::shared_ptr<CEnvironment> rootEnvironment)
{
	currentEnvironment = rootEnvironment;
	for (std::shared_ptr<LStmt> statement : statements)
	{
		resolve(statement);
	}
	currentEnvironment = console.get()->scriptConsole.get()->global;
}

//
void LResolver::clearUnresolvedSymbols()
{
	unresolvedSymbols.clear();
}

//
void LResolver::checkUnresolvedSymbols()
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
void LResolver::resolve(std::shared_ptr<LStmt> stmt)
{
	stmt->accept(*this);
}
void LResolver::resolve(std::shared_ptr<LExpr> expr)
{
	expr->acceptResolver(*this);
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// layoutResolver Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

// LStmt_Root
void LResolver::visit(std::shared_ptr<LStmt_Root> stmt)
{
	bool hasId = false;
	std::string idName = "";
	std::vector<std::shared_ptr<LToken>> attribs;
	for (std::shared_ptr<LExpr> attribExpr : stmt.get()->attributes)
	{
		attribs.push_back(attribExpr.get()->acceptResolver(*this));
	}
	for (std::shared_ptr<LToken> attribToken : attribs)
	{
		if (attribToken.get()->type == LTokenType::ID)
		{
			hasId = true;
			idName = attribToken.get()->lexeme;
			if (idName.find(",") != std::string::npos)
			{
				idName = splitRegularString(idName, ",");
			}
			stmt.get()->id = idName;
		}
		if (attribToken.get()->type == LTokenType::DEFAULT_POSITION)
		{
			std::string defaultPosString = attribToken.get()->lexeme;
			defaultPosString.erase(std::remove(defaultPosString.begin(), defaultPosString.end(), '\n'), defaultPosString.end());
			defaultPosString.erase(remove_if(defaultPosString.begin(), defaultPosString.end(), isspace), defaultPosString.end());
			defaultPosString.erase(std::remove(defaultPosString.begin(), defaultPosString.end(), '\t'), defaultPosString.end());
			if (defaultPosString.find(",") == std::string::npos) 
			{ 
				console.get()->error(attribToken, "[resolver:0301] 'defaultPosition' attribute must have two values."); 
			}
			else
			{
				int count = 0;
				while (defaultPosString.find(",") != std::string::npos)
				{
					std::string eval = splitRegularString(defaultPosString, ",");
					if (eval != "" && eval != ",")
					{
						if (count == 0) { stmt.get()->defaultX = std::stof(eval); count++; }
						else if (count == 1) { stmt.get()->defaultY = std::stof(eval); count++; }
						else
						{
							console.get()->warning(attribToken,
								"[resolver:0302] 'defaultPosition' attribute contained more than two values, "
								"trimmed all values after the first.");
							break;
						}
					}
				}
				if (defaultPosString != "")
				{
					stmt.get()->defaultY = std::stof(defaultPosString); count++;
				}
			}
		}
	}
	// Check for Id
	if (!hasId)
	{
		console.get()->error(stmt.get()->widgetType,
			"[resolver:0302] Widgets of 'ROOT' type must have an 'id' attribute with "
			"valid and unique id value.");
	}
	else
	{
		// Check for valid Id, add to WidgetTable
		// Use idName for both fields, because this is a root-level widget
		if (!console.get()->scriptConsole.get()->insertWidgetTableID(
			idName, idName) || stmt.get()->childElements.size() == 0)
		{
			console.get()->error(stmt.get()->widgetType,
				"[resolver:0303] Widgets of 'ROOT' type must have an 'id' attribute with "
				"valid and unique id value, and contain one child widget.");
		}
		// Check child elements
		else
		{
			if (stmt.get()->childElements.size() > 1)
			{
				console.get()->warning(stmt.get()->widgetType,
					"[resolver:0304] Widgets of 'ROOT' type may only have one child. Removing additional children.");
				stmt.get()->childElements.erase(stmt.get()->childElements.begin() + 1, stmt.get()->childElements.end());
			}
			currentRootId = idName;
			stmt.get()->childElements[0].get()->acceptResolver(*this);
			currentRootId = "";
		}
	}
	return;
}
// LStmt_Proto
void LResolver::visit(std::shared_ptr<LStmt_Proto> stmt)
{
	bool hasId = false;
	std::string idName = "";
	std::vector<std::shared_ptr<LToken>> attribs;
	for (std::shared_ptr<LExpr> attribExpr : stmt.get()->attributes)
	{
		if (attribExpr == nullptr) { continue; }
		attribs.push_back(attribExpr.get()->acceptResolver(*this));
	}
	for (std::shared_ptr<LToken> attribToken : attribs)
	{
		if (attribToken.get()->type == LTokenType::ID)
		{
			hasId = true;
			idName = attribToken.get()->lexeme;
			if (idName.find(",") != std::string::npos)
			{
				idName = splitRegularString(idName, ",");
			}
			stmt.get()->id = idName;
		}
	}
	// Check for Id
	if (!hasId)
	{
		console.get()->error(stmt.get()->widgetType,
			"[resolver:0301] Widgets of 'ROOT' type must have an 'id' attribute with "
			"valid and unique id value.");
	}
	else
	{
		// Check for valid Id, declare an empty entry in prototypeFactoryTable
		// The interpreter will construct the value later
		if (!console.get()->scriptConsole.get()->insertPrototypeTableID(
			idName))
		{
			console.get()->error(stmt.get()->widgetType,
				"[resolver:0301] Widgets of 'ROOT' type must have an 'id' attribute with "
				"valid and unique id value.");
		}
		// Check child elements
		else
		{
			currentRootId = idName;
			prototypeIdList.push_back(idName);
			for (std::shared_ptr<LExpr> child : stmt.get()->childElements)
			{
				child.get()->acceptResolver(*this);
			}
			prototypeIdList.clear();
			currentRootId = "";
		}
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// layoutResolver Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// LExpr_Element
std::shared_ptr<LToken> LResolver::visit(std::shared_ptr<LExpr_Element> expr)
{
	bool hasId = false;
	bool hasClasses = false;
	bool hasGroups = false;
	std::string idName = "";
	std::string classString = "";
	std::string groupsString = "";
	std::vector<std::shared_ptr<LToken>> attribs;
	for (std::shared_ptr<LExpr> attribExpr : expr.get()->attributes)
	{
		attribs.push_back(attribExpr.get()->acceptResolver(*this));
	}
	for (std::shared_ptr<LToken> attribToken : attribs)
	{
		if (attribToken.get()->type == LTokenType::ID)
		{
			hasId = true;
			idName = attribToken.get()->lexeme;
			if (idName.find(",") != std::string::npos)
			{
				idName = splitRegularString(idName, ",");
			}
			expr.get()->id = idName;
			if (idName == "")
			{
				console.get()->error("");
			}
		}
		if (attribToken.get()->type == LTokenType::CLASS)
		{
			hasClasses = true;
			classString = attribToken.get()->lexeme;
			classString.erase(std::remove(classString.begin(), classString.end(), '\n'), classString.end());
			classString.erase(remove_if(classString.begin(), classString.end(), isspace), classString.end());
			classString.erase(std::remove(classString.begin(), classString.end(), '\t'), classString.end());
		}
		if (attribToken.get()->type == LTokenType::GROUP)
		{
			hasGroups = true;
			groupsString = attribToken.get()->lexeme;
			groupsString.erase(std::remove(groupsString.begin(), groupsString.end(), '\n'), groupsString.end());
			groupsString.erase(remove_if(groupsString.begin(), groupsString.end(), isspace), groupsString.end());
			groupsString.erase(std::remove(groupsString.begin(), groupsString.end(), '\t'), groupsString.end());
		}
	}
	// Check for Id
	if (hasId)
	{
		// Check for valid Id, add to WidgetTable
		if (!console.get()->scriptConsole.get()->insertWidgetTableID(
			idName, currentRootId))
		{
			console.get()->error(expr.get()->widgetType,
				"[resolver:0301] Cannot use id '" + idName + 
				"' because a widget with that id already exists.");
		}
	}
	// Check for Classes
	if (hasClasses)
	{
		// If ',' exists, separate the classes
		std::vector<std::string> classNames;
		if (classString.find(",") == std::string::npos) { classNames.push_back(classString); }
		else
		{
			while (classString.find(",") != std::string::npos)
			{
				std::string eval = splitRegularString(classString, ",");
				if (eval != "" && eval != ",")
				{
					classNames.push_back(eval);
				}
			}
			if (classString != "") { classNames.push_back(classString); }
		}
		for (std::string className : classNames)
		{
			expr.get()->classes.push_back(className);
			console.get()->scriptConsole.get()->insertWidgetTableClass(className, currentRootId);
		}
	}
	// Check for Classes
	if (hasGroups)
	{
		// If ',' exists, separate the classes
		std::vector<std::string> groupNames;
		if (groupsString.find(",") == std::string::npos) { groupNames.push_back(groupsString); }
		else
		{
			while (groupsString.find(",") != std::string::npos)
			{
				std::string eval = splitRegularString(groupsString, ",");
				if (eval != "" && eval != ",")
				{
					groupNames.push_back(eval);
				}
			}
			if (groupsString != "") { groupNames.push_back(groupsString); }
		}
		for (std::string groupName : groupNames)
		{
			expr.get()->groups.push_back(groupName);
		}
	}
	// Check child elements
	for (std::shared_ptr<LExpr> child : expr.get()->childElements)
	{
		child.get()->acceptResolver(*this);
	}
	return nullptr;
}
// LExpr_RootAttrib
std::shared_ptr<LToken> LResolver::visit(std::shared_ptr<LExpr_RootAttrib> expr)
{
	return std::make_shared<LToken>(expr.get()->attribName.get()->type, expr.get()->attribValue.get()->lexeme, expr.get()->line);
}
// LExpr_ElementAttrib
std::shared_ptr<LToken> LResolver::visit(std::shared_ptr<LExpr_ElementAttrib> expr)
{
	return std::make_shared<LToken>(expr.get()->attribName.get()->type, expr.get()->attribValue.get()->lexeme, expr.get()->line);
}
