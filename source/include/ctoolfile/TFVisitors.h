#ifndef TFVISITORS_H
#define TFVISITORS_H

#include "TFForward.h"

#include <memory>

//
template <class _ReturnType>
class TFExprVisitor_Abstract
{
public:
	virtual _ReturnType visit(std::shared_ptr<TFExpr_Value> expr) = 0;
	virtual _ReturnType visit(std::shared_ptr<TFExpr_Name> stmt) = 0;
};

//
template <class _ReturnType>
class TFStmtVisitor_Abstract
{
public:
	virtual _ReturnType visit(std::shared_ptr<TFStmt_Tool> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<TFStmt_SettingsBlock> stmt) = 0;
	virtual _ReturnType visit(std::shared_ptr<TFStmt_Setting> stmt) = 0;
};

#endif