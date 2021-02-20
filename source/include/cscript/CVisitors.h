#ifndef CVISITORS_H
#define CVISITORS_H

#include "CForward.h"

#include <memory>

//
template <class _ReturnType>
class CExprVisitor_Abstract
{
public:
	virtual _ReturnType visit(std::shared_ptr<CExpr_Approximate> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Array> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Assign> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Binary> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Call> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Get> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Grouping> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_InstanceOf> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Literal> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Logical> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Scope> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_TypeCast> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Unary> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Variable> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<CExpr_Vector> expr) = 0;
};

//
template <class _ReturnType>
class CStmtVisitor_Abstract
{
public:
	virtual _ReturnType visit(std::shared_ptr<CStmt_Block> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_Break> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_Class> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_Continue> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_Expression> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_For> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_Foreach> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_Function> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_Namespace> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_If> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_Print> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_Return> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_TryCatch> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_Variable> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<CStmt_While> stmt) = 0;
};

#endif