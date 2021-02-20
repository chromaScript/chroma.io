#include "../include/clayout/LProc.h"
#include "../include/clayout/LToken.h"
#include "../include/clayout/LVisitors.h"
#include "../include/entities/Widget.h"

#include <memory>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

// LStmt_Root
LStmt_Root::LStmt_Root(int line,
	std::shared_ptr<LToken> widgetType,
	std::vector<std::shared_ptr<LExpr>> childElements,
	std::vector<std::shared_ptr<LExpr>> attributes)
{
	this->line = line;
	this->widgetType = widgetType;
	this->childElements = childElements;
	this->attributes = attributes;
}
void LStmt_Root::accept(LStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// LStmt_Proto
LStmt_Proto::LStmt_Proto(int line,
	std::shared_ptr<LToken> widgetType,
	std::vector<std::shared_ptr<LExpr>> childElements,
	std::vector<std::shared_ptr<LExpr>> attributes)
{
	this->line = line;
	this->widgetType = widgetType;
	this->childElements = childElements;
	this->attributes = attributes;
}
void LStmt_Proto::accept(LStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// LExpr_Element
LExpr_Element::LExpr_Element(int line,
	std::shared_ptr<LToken> widgetType,
	std::vector<std::shared_ptr<LExpr>> childElements,
	std::vector<std::shared_ptr<LExpr>> attributes)
{
	this->line = line;
	this->widgetType = widgetType;
	this->childElements = childElements;
	this->attributes = attributes;
}
void LExpr_Element::accept(LExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<LToken> LExpr_Element::acceptResolver(LExprVisitor_Abstract<std::shared_ptr<LToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<Widget> LExpr_Element::acceptBuilder(LExprVisitor_Abstract<std::shared_ptr<Widget>>& visitor)
{
	return visitor.visit(shared_from_this());
}

// LExpr_RootAttrib
LExpr_RootAttrib::LExpr_RootAttrib(int line,
	std::shared_ptr<LToken> attribName,
	std::shared_ptr<LToken> attribValue)
{
	this->line = line;
	this->attribName = attribName;
	this->attribValue = attribValue;
}
void LExpr_RootAttrib::accept(LExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<LToken> LExpr_RootAttrib::acceptResolver(LExprVisitor_Abstract<std::shared_ptr<LToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<Widget> LExpr_RootAttrib::acceptBuilder(LExprVisitor_Abstract<std::shared_ptr<Widget>>& visitor)
{
	return visitor.visit(shared_from_this());
}

// LExpr_ElementAttrib
LExpr_ElementAttrib::LExpr_ElementAttrib(int line,
	std::shared_ptr<LToken> attribName,
	std::shared_ptr<LToken> attribValue)
{
	this->line = line;
	this->attribName = attribName;
	this->attribValue = attribValue;
}
void LExpr_ElementAttrib::accept(LExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<LToken> LExpr_ElementAttrib::acceptResolver(LExprVisitor_Abstract<std::shared_ptr<LToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<Widget> LExpr_ElementAttrib::acceptBuilder(LExprVisitor_Abstract<std::shared_ptr<Widget>>& visitor)
{
	return visitor.visit(shared_from_this());
}