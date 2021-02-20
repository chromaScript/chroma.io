#ifndef LPROC_H
#define LPROC_H

#include "LForward.h"
#include "LToken.h"
#include "LVisitors.h"
class Widget;

#include <memory>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// layoutParser Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

class LStmt
{
public:
	int line = 0;
	std::string id = "";
	float defaultX = 0;
	float defaultY = 0;
	virtual void accept(LStmtVisitor_Abstract<void>& visitor) = 0;
};

// Child Classes
class LStmt_Root : public LStmt, public std::enable_shared_from_this<LStmt_Root>
{
public:
	std::shared_ptr<LToken> widgetType;
	std::vector<std::shared_ptr<LExpr>> childElements;
	std::vector<std::shared_ptr<LExpr>> attributes;
	LStmt_Root(int line,
		std::shared_ptr<LToken> widgetType,
		std::vector<std::shared_ptr<LExpr>> childElements,
		std::vector<std::shared_ptr<LExpr>> attributes);
	virtual void accept(LStmtVisitor_Abstract<void>& visitor);
};
class LStmt_Proto : public LStmt, public std::enable_shared_from_this<LStmt_Proto>
{
public:
	std::shared_ptr<LToken> widgetType;
	std::vector<std::shared_ptr<LExpr>> childElements;
	std::vector<std::shared_ptr<LExpr>> attributes;
	LStmt_Proto(int line,
		std::shared_ptr<LToken> widgetType,
		std::vector<std::shared_ptr<LExpr>> childElements,
		std::vector<std::shared_ptr<LExpr>> attributes);
	virtual void accept(LStmtVisitor_Abstract<void>& visitor);
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// layoutParser Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class LExpr
{
public:
	int line = 0;
	virtual void accept(LExprVisitor_Abstract<void>& visitor) = 0;
	virtual std::shared_ptr<LToken> acceptResolver(LExprVisitor_Abstract<std::shared_ptr<LToken>>& visitor) = 0;
	virtual std::shared_ptr<Widget> acceptBuilder(LExprVisitor_Abstract<std::shared_ptr<Widget>>& visitor) = 0;
};

// Child Classes
class LExpr_Element : public LExpr, public std::enable_shared_from_this<LExpr_Element>
{
public:
	std::shared_ptr<LToken> widgetType;
	std::vector<std::shared_ptr<LExpr>> childElements;
	std::vector<std::shared_ptr<LExpr>> attributes;
	std::string id;
	std::vector<std::string> classes;
	std::vector<std::string> groups;
	LExpr_Element(int line,
		std::shared_ptr<LToken> widgetType,
		std::vector<std::shared_ptr<LExpr>> childElements,
		std::vector<std::shared_ptr<LExpr>> attributes);
	virtual void accept(LExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<LToken> acceptResolver(LExprVisitor_Abstract<std::shared_ptr<LToken>>& visitor);
	virtual std::shared_ptr<Widget> acceptBuilder(LExprVisitor_Abstract<std::shared_ptr<Widget>>& visitor);
};
class LExpr_RootAttrib : public LExpr, public std::enable_shared_from_this<LExpr_RootAttrib>
{
public:
	std::shared_ptr<LToken> attribName;
	std::shared_ptr<LToken> attribValue;
	LExpr_RootAttrib(int line,
		std::shared_ptr<LToken> attribName,
		std::shared_ptr<LToken> attribValue);
	virtual void accept(LExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<LToken> acceptResolver(LExprVisitor_Abstract<std::shared_ptr<LToken>>& visitor);
	virtual std::shared_ptr<Widget> acceptBuilder(LExprVisitor_Abstract<std::shared_ptr<Widget>>& visitor);
};
class LExpr_ElementAttrib : public LExpr, public std::enable_shared_from_this<LExpr_ElementAttrib>
{
public:
	std::shared_ptr<LToken> attribName;
	std::shared_ptr<LToken> attribValue;
	LExpr_ElementAttrib(int line,
		std::shared_ptr<LToken> attribName,
		std::shared_ptr<LToken> attribValue);
	virtual void accept(LExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<LToken> acceptResolver(LExprVisitor_Abstract<std::shared_ptr<LToken>>& visitor);
	virtual std::shared_ptr<Widget> acceptBuilder(LExprVisitor_Abstract<std::shared_ptr<Widget>>& visitor);
};



#endif