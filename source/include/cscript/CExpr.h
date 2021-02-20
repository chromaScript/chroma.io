#ifndef CEXPR_H
#define CEXPR_H

#include "CForward.h"
#include "CInterpreter.h"
#include "CToken.h"
#include "CVisitors.h"
#include "CObject.h"

#include <memory>
#include <string>

class CExpr
{
public:
	virtual void accept(CExprVisitor_Abstract<void>& visitor) = 0;
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor) = 0;
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor) = 0;
};

// Child Classes
class CExpr_Approximate : public CExpr, public std::enable_shared_from_this<CExpr_Approximate>
{
public:
	std::shared_ptr<CToken> line;
	std::shared_ptr<CExpr> left;
	std::shared_ptr<CExpr> value;
	std::shared_ptr<CExpr> right;
	CExpr_Approximate(std::shared_ptr<CExpr> left, std::shared_ptr<CExpr> value, std::shared_ptr<CExpr> right, std::shared_ptr<CToken> line);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Array : public CExpr, public std::enable_shared_from_this<CExpr_Array>
{
public:
	std::shared_ptr<CToken> line;
	std::vector<std::shared_ptr<CExpr>> values;
	CExpr_Array(std::vector<std::shared_ptr<CExpr>> values, std::shared_ptr<CToken> line);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Assign : public CExpr, public std::enable_shared_from_this<CExpr_Assign>
{
public:
	std::shared_ptr<CExpr> identifier;
	std::shared_ptr<CToken> _operator;
	std::shared_ptr<CExpr> value;
	CExpr_Assign(std::shared_ptr<CExpr> identifier, std::shared_ptr<CToken> _operator, std::shared_ptr<CExpr> value);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Binary : public CExpr, public std::enable_shared_from_this<CExpr_Binary>
{
public:
	std::shared_ptr<CExpr> left;
	std::shared_ptr<CToken> _operator;
	std::shared_ptr<CExpr> right;
	CExpr_Binary(std::shared_ptr<CExpr> left, std::shared_ptr<CToken> _operator, std::shared_ptr<CExpr> right);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Call : public CExpr, public std::enable_shared_from_this<CExpr_Call>
{
public:
	std::shared_ptr<CToken> line;
	std::shared_ptr<CExpr> callee;
	std::vector<std::shared_ptr<CExpr>> arguments;
	std::shared_ptr<CExpr> index;
	CExpr_Call(
		std::shared_ptr<CExpr> callee, 
		std::vector<std::shared_ptr<CExpr>> arguments,
		std::shared_ptr<CExpr> index,
		std::shared_ptr<CToken> line);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Get : public CExpr, public std::enable_shared_from_this<CExpr_Get>
{
public:
	// Object is the expression before '.' access, and field is after. CExpr is used to allow array access specifier '[i]'
	std::shared_ptr<CToken> line;
	std::shared_ptr<CExpr> object;
	std::shared_ptr<CExpr> field;
	std::shared_ptr<CToken> fieldName;
	CExpr_Get(
		std::shared_ptr<CExpr> object,
		std::shared_ptr<CExpr> field,
		std::shared_ptr<CToken> line,
		std::shared_ptr<CToken> fieldName);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Grouping : public CExpr, public std::enable_shared_from_this<CExpr_Grouping>
{
public:
	std::shared_ptr<CToken> line;
	std::shared_ptr<CExpr> expression;
	CExpr_Grouping(std::shared_ptr<CExpr> expression, std::shared_ptr<CToken> line);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_InstanceOf : public CExpr, public std::enable_shared_from_this<CExpr_InstanceOf>
{
public:
	std::shared_ptr<CToken> type;
	std::shared_ptr<CExpr> right;
	std::shared_ptr<CToken> line;
	CExpr_InstanceOf(std::shared_ptr<CToken> type, std::shared_ptr<CExpr> right, std::shared_ptr<CToken> line);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Literal : public CExpr, public std::enable_shared_from_this<CExpr_Literal>
{
public:
	std::shared_ptr<CToken> value = nullptr;
	CExpr_Literal(std::shared_ptr<CToken> value);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Logical : public CExpr, public std::enable_shared_from_this<CExpr_Logical>
{
public:
	std::shared_ptr<CExpr> left;
	std::shared_ptr<CToken> _operator;
	std::shared_ptr<CExpr> right;
	CExpr_Logical(std::shared_ptr<CExpr> left, std::shared_ptr<CToken> _operator, std::shared_ptr<CExpr> right);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Scope : public CExpr, public std::enable_shared_from_this<CExpr_Scope>
{
public:
	std::shared_ptr<CToken> line;
	std::shared_ptr<std::vector<std::shared_ptr<CToken>>> scopeStack;
	std::shared_ptr<CExpr> right;
	CExpr_Scope(
		std::shared_ptr<std::vector<std::shared_ptr<CToken>>> scopeStack,
		std::shared_ptr<CExpr> right,
		std::shared_ptr<CToken> line);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_TypeCast : public CExpr, public std::enable_shared_from_this<CExpr_TypeCast>
{
public:
	std::shared_ptr<CToken> type;
	std::shared_ptr<CExpr> right;
	CExpr_TypeCast(std::shared_ptr<CToken> type, std::shared_ptr<CExpr> right);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Unary : public CExpr, public std::enable_shared_from_this<CExpr_Unary>
{
public:
	std::shared_ptr<CToken> _operator;
	std::shared_ptr<CExpr> right;
	CExpr_Unary(std::shared_ptr<CToken> _operator, std::shared_ptr<CExpr> right);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Variable : public CExpr, public std::enable_shared_from_this<CExpr_Variable>
{
public:
	std::shared_ptr<CToken> name;
	std::shared_ptr<CExpr> index = nullptr;
	CExpr_Variable(std::shared_ptr<CToken> name);
	CExpr_Variable(std::shared_ptr<CToken> name, std::shared_ptr<CExpr> index);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

class CExpr_Vector : public CExpr, public std::enable_shared_from_this<CExpr_Vector>
{
public:
	std::shared_ptr<CToken> kind;
	std::shared_ptr<CExpr> x;
	std::shared_ptr<CExpr> y;
	std::shared_ptr<CExpr> z;
	std::shared_ptr<CExpr> w;
	CExpr_Vector(std::shared_ptr<CToken> kind, std::vector<std::shared_ptr<CExpr>> values);
	virtual void accept(CExprVisitor_Abstract<void>& visitor);
	virtual std::shared_ptr<CObject> acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor);
	virtual std::shared_ptr<CToken> acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor);
};

#endif