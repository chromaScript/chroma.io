#ifndef SVISITORS_H
#define SVISITORS_H

#include "SForward.h"

#include <memory>

//
template <class _ReturnType>
class SExprVisitor_Abstract
{
public:
	virtual _ReturnType visit(std::shared_ptr<SExpr_Property> expr) = 0;

};

//
template <class _ReturnType>
class SStmtVisitor_Abstract
{
public:
	virtual _ReturnType visit(std::shared_ptr<SStmt_Script> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<SStmt_Style> stmt) = 0;

};

#endif