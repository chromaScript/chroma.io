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
#include "../include/cscript/CStmt.h"

#include "../include/Application.h"
#include "../include/entities/UserInterface.h"

#include "../include/structs.h"

#include <string>
#include <vector>
#include <exception>
#include <map>
#include <stack>
#include <filesystem>
#include <iostream>

LInterpreter::LInterpreter()
{

}

void LInterpreter::initialize(std::shared_ptr<ChromaLayout> console)
{
	this->console = console;
	this->ui = console.get()->owner.get()->getUI();
}

//
void LInterpreter::constructWidgets(
	std::vector<std::shared_ptr<LStmt>> statements,
	std::shared_ptr<CEnvironment> rootEnvironment,
	std::filesystem::path rootDir)
{
	currentEnvironment = rootEnvironment;
	currentRootDir = rootDir;
	for (std::shared_ptr<LStmt> statement : statements)
	{
		build(statement);
	}
	currentRootDir.clear();
	currentEnvironment = console.get()->scriptConsole.get()->global;
}

//
void LInterpreter::clearUnresolvedSymbols()
{
	unresolvedSymbols.clear();
}

//
void LInterpreter::checkUnresolvedSymbols()
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
void LInterpreter::build(std::shared_ptr<LStmt> stmt)
{
	stmt->accept(*this);
}
void LInterpreter::build(std::shared_ptr<LExpr> expr)
{
	expr->acceptBuilder(*this);
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// layoutResolver Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

// LStmt_Root
void LInterpreter::visit(std::shared_ptr<LStmt_Root> stmt)
{
	// Initialize constructor call parameters
	basicAttribs.clear();
	std::shared_ptr<WidgetStyle> style = std::make_shared<WidgetStyle>(); // Empty style, won't be used
	std::shared_ptr<Shader> shader = console.get()->owner.get()->getWidgetShader();
	
	// Set Widget constructor call parameters
	unboundAttribs.clear();
	basicOnly = true;
	for (std::shared_ptr<LExpr> attrib : stmt.get()->attributes)
	{
		attrib.get()->acceptBuilder(*this);
	}
	basicOnly = false;

	// Make the widget (may only have one child)
	ui.get()->widgets.push_back(stmt.get()->childElements[0].get()->acceptBuilder(*this));
	ui.get()->widgets.back().get()->isRoot = true;
	ui.get()->widgets.back().get()->rootId = stmt.get()->id;
	ui.get()->widgets.back().get()->setLocation((int)stmt.get()->defaultX, (int)stmt.get()->defaultY);
	ui.get()->widgets.back().get()->_namespace = currentEnvironment.get()->_namespace;

	// Clean up and exit
	currentElement.reset();
	//currentRoot.reset();
	unboundAttribs.clear();
	basicAttribs.clear();
	return;
}
// LStmt_Proto
void LInterpreter::visit(std::shared_ptr<LStmt_Proto> stmt)
{
	// Initialize constructor call parameters
	basicAttribs.clear();
	std::shared_ptr<WidgetStyle> style = std::make_shared<WidgetStyle>(); // Empty style, won't be used
	std::shared_ptr<Shader> shader = console.get()->owner.get()->getWidgetShader();

	// Set Widget constructor call parameters
	unboundAttribs.clear();
	basicOnly = true;
	for (std::shared_ptr<LExpr> attrib : stmt.get()->attributes)
	{
		attrib.get()->acceptBuilder(*this);
	}
	basicOnly = false;

	// Make the widget (Can only be one child)
	ui.get()->prototypeFactoryTable.insert(
		std::pair<std::string, std::shared_ptr<Widget>>(
			stmt.get()->id, stmt.get()->childElements[0].get()->acceptBuilder(*this)));
	ui.get()->prototypeFactoryTable.at(stmt.get()->id).get()->isRoot = true;
	ui.get()->prototypeFactoryTable.at(stmt.get()->id).get()->rootId = stmt.get()->id;
	ui.get()->prototypeFactoryTable.at(stmt.get()->id).get()->setLocation((int)stmt.get()->defaultX, (int)stmt.get()->defaultY);
	ui.get()->prototypeFactoryTable.at(stmt.get()->id).get()->_namespace = currentEnvironment.get()->_namespace;

	// Clean up and exit
	currentElement.reset();
	currentRoot.reset();
	unboundAttribs.clear();
	basicAttribs.clear();
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// layoutResolver Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// LExpr_Element
std::shared_ptr<Widget> LInterpreter::visit(std::shared_ptr<LExpr_Element> expr)
{
	// Initialize constructor call parameters
	basicAttribs.clear();
	std::shared_ptr<WidgetStyle> style = std::make_shared<WidgetStyle>();
	std::shared_ptr<Shader> shader = nullptr;
	switch (expr.get()->widgetType.get()->type)
	{
	case LTokenType::H_BOX:
		shader = console.get()->owner.get()->getWidgetShader(); break;
	case LTokenType::V_BOX:
		shader = console.get()->owner.get()->getWidgetShader(); break;
	case LTokenType::IMAGE:
		shader = console.get()->owner.get()->getWidgetShader(); break;
	case LTokenType::TEXT:
		shader = console.get()->owner.get()->getTextShader(); break;
	case LTokenType::GRADIENT_BOX:
		shader = console.get()->owner.get()->getGradientBoxShader(); break;
	default:
		shader = console.get()->owner.get()->getWidgetShader(); break;
	}
	if (expr.get()->id != "")
	{
		std::shared_ptr<CObject> idStyle = currentEnvironment.get()->get("#" + expr.get()->id);
		if (idStyle != nullptr)
		{
			style.get()->mergeStyle(std::get<std::shared_ptr<WidgetStyle>>(idStyle.get()->obj));
		}
		
	}
	for (std::string className : expr.get()->classes)
	{
		std::shared_ptr<CObject> classStyle = currentEnvironment.get()->get("." + className);
		if (classStyle != nullptr)
		{
			style.get()->mergeStyle(std::get<std::shared_ptr<WidgetStyle>>(classStyle.get()->obj));
		}
		
	}
	
	// Set constructor call parameters
	unboundAttribs.clear();
	basicOnly = true;
	for (std::shared_ptr<LExpr> attrib : expr.get()->attributes)
	{
		attrib.get()->acceptBuilder(*this);
	}
	basicOnly = false;

	std::shared_ptr<Widget> element = ui.get()->createWidget(expr.get()->widgetType.get()->type, basicAttribs, currentElement, style, shader);
	// Set properties
	element.get()->_namespace = currentEnvironment.get()->_namespace;
	element.get()->classAttribs = expr.get()->classes;
	element.get()->groupsAttribs = expr.get()->groups;
	
	// Resolve unbound attributes
	currentElement = element;
	for (std::shared_ptr<LExpr> attrib : unboundAttribs)
	{
		attrib.get()->acceptBuilder(*this);
	}

	// Create child widgets
	for (std::shared_ptr<LExpr> child : expr.get()->childElements)
	{
		currentElement = element;
		std::shared_ptr<Widget> childWidget = child.get()->acceptBuilder(*this);
		if (childWidget != nullptr)
		{
			element.get()->childWidgets.push_back(childWidget);
		}
	}
	// Clean up and exit
	unboundAttribs.clear();
	basicAttribs.clear();
	return element;
}
// LExpr_RootAttrib
std::shared_ptr<Widget> LInterpreter::visit(std::shared_ptr<LExpr_RootAttrib> expr)
{
	std::string attribName = expr.get()->attribName.get()->typeString();
	std::string attribValue = expr.get()->attribValue.get()->lexeme;
	switch (expr.get()->attribName.get()->type)
	{
	case LTokenType::ID:
		basicAttribs.insert(std::pair<std::string, std::string>(attribName, attribValue));
		break;
	}
	return nullptr;
}
// LExpr_ElementAttrib
std::shared_ptr<Widget> LInterpreter::visit(std::shared_ptr<LExpr_ElementAttrib> expr)
{
	std::string attribName = expr.get()->attribName.get()->typeString();
	std::string attribValue = expr.get()->attribValue.get()->lexeme;
	switch (expr.get()->attribName.get()->type)
	{
	case LTokenType::ID:
		basicAttribs.insert(std::pair<std::string, std::string>(attribName, attribValue));
		break;
	case LTokenType::NAME:
		basicAttribs.insert(std::pair<std::string, std::string>(attribName, attribValue));
		break;
	case LTokenType::VALUE:
		basicAttribs.insert(std::pair<std::string, std::string>(attribName, attribValue));
		break;
	case LTokenType::IMG:
		basicAttribs.insert(std::pair<std::string, std::string>(attribName, attribValue));
		break;
	case LTokenType::INNER_CONTENT:
		basicAttribs.insert(std::pair<std::string, std::string>(attribName, attribValue));
		break;
	case LTokenType::STYLE:
		if (basicOnly) { unboundAttribs.push_back(expr); break; }
		else
		{
			std::shared_ptr<WidgetStyle> overWrite = console.get()->owner.get()->styleConsole.get()->create(
				attribValue, currentEnvironment.get()->_namespace, currentRootDir);
			if (overWrite != nullptr && !currentElement.expired())
			{
				currentElement.lock().get()->style.mergeStyle(overWrite);
			}
		} break;
	case LTokenType::ON_BLUR:
	case LTokenType::ON_FOCUS:
	case LTokenType::ON_CLICK:
	case LTokenType::ON_RIGGHTCLICK:
	case LTokenType::ON_DBL_CLICK:
	case LTokenType::ON_DBL_RIGHTCLICK:
	case LTokenType::ON_MIDDLECLICK:
	case LTokenType::ON_DBL_MIDDLECLICK:
	case LTokenType::ON_DRAG:
	case LTokenType::ON_DRAG_START:
	case LTokenType::ON_DRAG_END:
	case LTokenType::ON_DRAG_ENTER:
	case LTokenType::ON_DRAG_OVER:
	case LTokenType::ON_DRAG_LEAVE:
	case LTokenType::ON_DROP:
	case LTokenType::ON_KEY_DOWN:
	case LTokenType::ON_KEY_PRESS:
	case LTokenType::ON_KEY_UP:
	case LTokenType::ON_LOAD:
	case LTokenType::ON_COPY:
	case LTokenType::ON_PASTE:
	case LTokenType::ON_CUT:
	case LTokenType::ON_MOUSE_OVER:
	case LTokenType::ON_MOUSE_ENTER:
	case LTokenType::ON_MOUSE_LEAVE:
	case LTokenType::ON_MOUSE_WHEELUP:
	case LTokenType::ON_MOUSE_WHEELDOWN:
	case LTokenType::ON_RESIZE:
	case LTokenType::ON_RELEASE:
		if (basicOnly) { unboundAttribs.push_back(expr); break; }
		else
		{
			bool isExpr = false;
			if (attribValue.find(";") == std::string::npos) { isExpr = true; }
			if (!currentElement.expired())
			{
				currentElement.lock().get()->callbackMap.insert(
					std::pair<std::string, std::vector<std::shared_ptr<CStmt>>>(
						attribName,
						console.get()->scriptConsole.get()->entry_compileOnly(attribValue, currentEnvironment.get()->_namespace, isExpr)));
			}
		} break;
	case LTokenType::DRAGGABLE:
		if (basicOnly) { unboundAttribs.push_back(expr); break; }
		else
		{
			bool value = false;
			if (attribValue == "true") { value = true; }
			if (!currentElement.expired())
			{
				currentElement.lock().get()->isDraggable = value;
			}
		} break;
	case LTokenType::DROPPABLE:
		if (basicOnly) { unboundAttribs.push_back(expr); break; }
		else
		{
			bool value = false;
			if (attribValue == "true") { value = true; }
			if (!currentElement.expired())
			{
				currentElement.lock().get()->isDroppable = value;
			}
		} break;
	case LTokenType::DRAG_TYPE:
		if (basicOnly) { unboundAttribs.push_back(expr); break; }
		else
		{
			if (!currentElement.expired())
			{
				currentElement.lock().get()->dragType = attribValue;
			}
		} break;
	case LTokenType::DROP_TYPE:
		if (basicOnly) { unboundAttribs.push_back(expr); break; }
		else
		{
			if (!currentElement.expired())
			{
				currentElement.lock().get()->dragType = attribValue;
			}
		} break;
	}
	return nullptr;
}
