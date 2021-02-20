#include "../include/cscript/CExpr.h"
#include "../include/cscript/CToken.h"
#include "../include/cscript/CObject.h"

#include <memory>

// CExpr_Approximate
CExpr_Approximate::CExpr_Approximate(
	std::shared_ptr<CExpr> left, 
	std::shared_ptr<CExpr> value,
	std::shared_ptr<CExpr> right, 
	std::shared_ptr<CToken> line)
{
	this->left = left;
	this->value = value;
	this->right = right;
	this->line = line;
}
void CExpr_Approximate::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Approximate::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Approximate::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}

// CExpr_Array
CExpr_Array::CExpr_Array(std::vector<std::shared_ptr<CExpr>> values, std::shared_ptr<CToken> line)
{
	this->values = values;
	this->line = line;
}
void CExpr_Array::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Array::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Array::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Assign
CExpr_Assign::CExpr_Assign(std::shared_ptr<CExpr> identifier, std::shared_ptr<CToken> _operator, std::shared_ptr<CExpr> value)
{
	this->identifier = identifier;
	this->_operator = _operator;
	this->value = value;
}
void CExpr_Assign::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Assign::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Assign::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Binary
CExpr_Binary::CExpr_Binary(std::shared_ptr<CExpr> left, std::shared_ptr<CToken> _operator, std::shared_ptr<CExpr> right)
{
	this->left = left;
	this->_operator = _operator;
	this->right = right;
}
void CExpr_Binary::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Binary::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Binary::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Call
CExpr_Call::CExpr_Call(
	std::shared_ptr<CExpr> callee,
	std::vector<std::shared_ptr<CExpr>> arguments,
	std::shared_ptr<CExpr> index,
	std::shared_ptr<CToken> line)
{
	this->callee = callee;
	this->arguments = arguments;
	this->index = index;
	this->line = line;
}
void CExpr_Call::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Call::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Call::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Get
CExpr_Get::CExpr_Get(
	std::shared_ptr<CExpr> object,
	std::shared_ptr<CExpr> field,
	std::shared_ptr<CToken> line,
	std::shared_ptr<CToken> fieldName)
{
	this->object = object;
	this->field = field;
	this->line = line;
	this->fieldName = fieldName;
}
void CExpr_Get::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Get::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Get::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_InstanceOf
CExpr_InstanceOf::CExpr_InstanceOf(std::shared_ptr<CToken> type, std::shared_ptr<CExpr> right, std::shared_ptr<CToken> line)
{
	this->type = type;
	this->right = right;
	this->line = line;
}
void CExpr_InstanceOf::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_InstanceOf::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_InstanceOf::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Grouping
CExpr_Grouping::CExpr_Grouping(std::shared_ptr<CExpr> expression, std::shared_ptr<CToken> line)
{
	this->expression = expression;
	this->line = line;
}
void CExpr_Grouping::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Grouping::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Grouping::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Literal
CExpr_Literal::CExpr_Literal(std::shared_ptr<CToken> value)
{
	this->value = value;
}
void CExpr_Literal::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Literal::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Literal::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Logical
CExpr_Logical::CExpr_Logical(std::shared_ptr<CExpr> left, std::shared_ptr<CToken> _operator, std::shared_ptr<CExpr> right)
{
	this->left = left;
	this->_operator = _operator;
	this->right = right;
}
void CExpr_Logical::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Logical::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Logical::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Scope
CExpr_Scope::CExpr_Scope(
	std::shared_ptr<std::vector<std::shared_ptr<CToken>>> scopeStack,
	std::shared_ptr<CExpr> right,
	std::shared_ptr<CToken> line)
{
	this->scopeStack = scopeStack;
	this->right = right;
	this->line = line;
}
void CExpr_Scope::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Scope::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Scope::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}

// CExpr_TypeCast
CExpr_TypeCast::CExpr_TypeCast(std::shared_ptr<CToken> type, std::shared_ptr<CExpr> right)
{
	this->type = type;
	this->right = right;
}
void CExpr_TypeCast::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_TypeCast::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_TypeCast::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Unary
CExpr_Unary::CExpr_Unary(std::shared_ptr<CToken> _operator, std::shared_ptr<CExpr> right)
{
	this->_operator = _operator;
	this->right = right;
}
void CExpr_Unary::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Unary::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Unary::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Variable
CExpr_Variable::CExpr_Variable(std::shared_ptr<CToken> name)
{
	this->name = name;
}
CExpr_Variable::CExpr_Variable(std::shared_ptr<CToken> name, std::shared_ptr<CExpr> index)
{
	this->name = name;
	this->index = index;
}
void CExpr_Variable::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Variable::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Variable::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}


// CExpr_Vector
CExpr_Vector::CExpr_Vector(std::shared_ptr<CToken> kind, std::vector<std::shared_ptr<CExpr>> values)
{
	this->kind = kind;
	if (values.size() != 0)
	{
		if (values.size() >= 1) { this->x = values[0]; }
		if (values.size() >= 2) { this->y = values[1]; }
		if (values.size() >= 3) { this->z = values[2]; }
		if (values.size() >= 4) { this->w = values[3]; }
	}
	
}
void CExpr_Vector::accept(CExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> CExpr_Vector::acceptInterpreter(CExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}
std::shared_ptr<CToken> CExpr_Vector::acceptTypeWizard(CExprVisitor_Abstract<std::shared_ptr<CToken>>& visitor)
{
	return visitor.visit(shared_from_this());
}
