#ifndef SPROC_H
#define SPROC_H

#include "SForward.h"
#include "SInterpreter.h"
#include "SToken.h"
#include "SVisitors.h"

#include <memory>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

class SStmt
{
public:
	int line;
	virtual void accept(SStmtVisitor_Abstract<void>& visitor) = 0;
};

// Child Classes
class SStmt_Style : public SStmt, public std::enable_shared_from_this<SStmt_Style>
{
public:
	std::vector<std::shared_ptr<SToken>> selectors;
	std::vector<std::shared_ptr<SToken>> identifiers;
	std::vector<std::shared_ptr<SExpr>> properties;
	std::vector<std::vector<std::shared_ptr<SToken>>> scopeStacks;
	SStmt_Style(
		int line, 
		std::vector<std::shared_ptr<SToken>> selectors,
		std::vector<std::shared_ptr<SToken>> identifiers,
		std::vector<std::shared_ptr<SExpr>> properties,
		std::vector<std::vector<std::shared_ptr<SToken>>> scopeStacks);
	virtual void accept(SStmtVisitor_Abstract<void>& visitor);
};
class SStmt_Script : public SStmt, public std::enable_shared_from_this<SStmt_Script>
{
public:
	std::shared_ptr<SToken> script;
	SStmt_Script(
		int line,
		std::shared_ptr<SToken> script);
	virtual void accept(SStmtVisitor_Abstract<void>& visitor);
};

////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////

class SExpr
{
public:
	int line;
	virtual void accept(SExprVisitor_Abstract<void>& visitor) = 0;
	virtual void acceptInterpreter(SExprVisitor_Abstract<void>& visitor) = 0;
};

class SExpr_Property : public SExpr, public std::enable_shared_from_this<SExpr_Property>
{
public:
	std::shared_ptr<SToken> propertyName;
	std::vector<std::shared_ptr<SToken>> values;
	SExpr_Property(
		int line,
		std::shared_ptr<SToken> propertyName,
		std::vector<std::shared_ptr<SToken>> values);
	virtual void accept(SExprVisitor_Abstract<void>& visitor);
	virtual void acceptInterpreter(SExprVisitor_Abstract<void>& visitor);
};


#endif