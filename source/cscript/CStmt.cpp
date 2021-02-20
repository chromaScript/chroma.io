#include "../include/cscript/CStmt.h"
#include "../include/cscript/CExpr.h"
#include "../include/cscript/CToken.h"
#include "../include/cscript/CVisitors.h"

#include <memory>

// CStmt_Block
CStmt_Block::CStmt_Block(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CToken> line)
{
	this->statements = statements;
	this->line = line;
}
void CStmt_Block::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_Break
CStmt_Break::CStmt_Break(std::shared_ptr<CToken> keyword)
{
	this->keyword = keyword;
}
void CStmt_Break::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_Class
CStmt_Class::CStmt_Class(
	std::vector<std::shared_ptr<CToken>> scopeStack,
	std::shared_ptr<CToken> name,
	std::shared_ptr<CToken> superclass,
	bool isNameDeclarationOnly,
	std::shared_ptr<CStmt> constructor,
	std::vector<std::shared_ptr<CStmt>> methods,
	std::vector<std::shared_ptr<CStmt>> properties)
{
	this->scopeStack = scopeStack;
	this->name = name;
	this->superclass = superclass;
	this->isNameDeclarationOnly = isNameDeclarationOnly;
	this->constructor = constructor;
	this->methods = methods;
	this->properties = properties;
}
void CStmt_Class::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_Continue
CStmt_Continue::CStmt_Continue(std::shared_ptr<CToken> keyword)
{
	this->keyword = keyword;
}
void CStmt_Continue::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_Expression
CStmt_Expression::CStmt_Expression(std::shared_ptr<CExpr> expression, std::shared_ptr<CToken> line)
{
	this->expression = expression;
	this->line = line;
}
void CStmt_Expression::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_For
CStmt_For::CStmt_For(
	std::shared_ptr<CStmt> initializer,
	std::shared_ptr<CExpr> condition,
	std::shared_ptr<CExpr> increment,
	std::shared_ptr<CStmt> body,
	std::shared_ptr<CToken> line)
{
	this->initializer = initializer;
	this->condition = condition;
	this->increment = increment;
	this->body = body;
	this->line = line;
}
void CStmt_For::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_Foreach
CStmt_Foreach::CStmt_Foreach(
	std::shared_ptr<CToken> typeName,
	std::shared_ptr<CToken> iteratorName,
	std::shared_ptr<CExpr> listExpression,
	std::shared_ptr<CStmt> body,
	std::shared_ptr<CToken> line)
{
	this->typeName = typeName;
	this->iteratorName = iteratorName;
	this->listExpression = listExpression;
	this->body = body;
	this->line = line;
}
void CStmt_Foreach::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_Function
CStmt_Function::CStmt_Function(
	std::shared_ptr<CToken> returnType,
	std::shared_ptr<CToken> name,
	bool isNameDeclarationOnly,
	std::vector<std::shared_ptr<CToken>> scopeStack,
	std::shared_ptr<CStmt> body)
{
	this->returnType = returnType;
	this->name = name;
	this->isNameDeclarationOnly = isNameDeclarationOnly;
	this->scopeStack = scopeStack;
	this->body = body;
}
CStmt_Function::CStmt_Function(
	std::shared_ptr<CToken> returnType,
	std::shared_ptr<CToken> name,
	bool isNameDeclarationOnly,
	std::vector<std::shared_ptr<CToken>> scopeStack,
	std::vector<std::shared_ptr<CToken>> paramsTypes,
	std::vector<std::string> paramsNames,
	std::shared_ptr<CStmt> body)
{
	this->returnType = returnType;
	this->name = name;
	this->isNameDeclarationOnly = isNameDeclarationOnly;
	this->scopeStack = scopeStack;
	this->paramsTypes = paramsTypes;
	this->paramsNames = paramsNames;
	this->body = body;
	this->arity = (int)paramsNames.size();
}
void CStmt_Function::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_Namespace
CStmt_Namespace::CStmt_Namespace(
	std::shared_ptr<CToken> name,
	std::shared_ptr<CStmt> body)
{
	this->name = name;
	this->body = body;
}
void CStmt_Namespace::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_If
CStmt_If::CStmt_If(
	std::vector<std::shared_ptr<CExpr>> conditionsList,
	std::vector<std::shared_ptr<CStmt>> executionList,
	std::shared_ptr<CStmt> elseBranch,
	std::shared_ptr<CToken> line)
{
	this->conditionsList = conditionsList;
	this->executionList = executionList;
	this->elseBranch = elseBranch;
	this->line = line;
}
void CStmt_If::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_Print
CStmt_Print::CStmt_Print(std::shared_ptr<CExpr> expression, std::shared_ptr<CToken> line)
{
	this->expression = expression;
	this->line = line;
}
void CStmt_Print::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_Return
CStmt_Return::CStmt_Return(std::shared_ptr<CToken> keyword, std::shared_ptr<CExpr> value)
{
	this->keyword = keyword;
	this->value = value;
}
void CStmt_Return::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_TryCatch
CStmt_TryCatch::CStmt_TryCatch(std::shared_ptr<CStmt> tryBlock, std::shared_ptr<CStmt> catchBlock, std::shared_ptr<CToken> line)
{
	this->tryBlock = tryBlock;
	this->catchBlock = catchBlock;
	this->line = line;
}
void CStmt_TryCatch::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_Variable
CStmt_Variable::CStmt_Variable(
	std::shared_ptr<CToken> typeName, 
	std::shared_ptr<CToken> name, 
	std::shared_ptr<CExpr> initializer)
{
	this->typeName = typeName;
	this->name = name;
	this->initializer = initializer;
}
void CStmt_Variable::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// CStmt_While
CStmt_While::CStmt_While(std::shared_ptr<CExpr> condition, std::shared_ptr<CStmt> body, std::shared_ptr<CToken> line)
{
	this->condition = condition;
	this->body = body;
	this->line = line;
}
void CStmt_While::accept(CStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
