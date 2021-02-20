#include "../include/cstyle/SProc.h"
#include "../include/cstyle/SToken.h"
#include "../include/cstyle/SVisitors.h"

#include <memory>

////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

// SStmt_Style
SStmt_Style::SStmt_Style(
	int line,
	std::vector<std::shared_ptr<SToken>> selectors,
	std::vector<std::shared_ptr<SToken>> identifiers,
	std::vector<std::shared_ptr<SExpr>> properties,
	std::vector<std::vector<std::shared_ptr<SToken>>> scopeStacks)
{
	this->line = line;
	this->selectors = selectors;
	this->identifiers = identifiers;
	this->properties = properties;
	this->scopeStacks = scopeStacks;
}
void SStmt_Style::accept(SStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}


// SStmt_Script
SStmt_Script::SStmt_Script(
	int line,
	std::shared_ptr<SToken> script)
{
	this->line = line;
	this->script = script;
}
void SStmt_Script::accept(SStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// styleParser Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// SExpr_Property
SExpr_Property::SExpr_Property(
	int line,
	std::shared_ptr<SToken> propertyName,
	std::vector<std::shared_ptr<SToken>> values)
{
	this->line = line;
	this->propertyName = propertyName;
	this->values = values;
}
void SExpr_Property::accept(SExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
void SExpr_Property::acceptInterpreter(SExprVisitor_Abstract<void>& visitor)
{
	return visitor.visit(shared_from_this());
}
