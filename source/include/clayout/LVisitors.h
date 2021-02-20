#ifndef LVISITORS_H
#define LVISITORS_H

#include "LForward.h"

#include <memory>

//
template <class _ReturnType>
class LExprVisitor_Abstract
{
public:
	virtual _ReturnType visit(std::shared_ptr<LExpr_Element> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<LExpr_RootAttrib> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<LExpr_ElementAttrib> expr) = 0;
};

//
template <class _ReturnType>
class LStmtVisitor_Abstract
{
public:
	virtual _ReturnType visit(std::shared_ptr<LStmt_Root> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<LStmt_Proto> stmt) = 0;

};

#endif