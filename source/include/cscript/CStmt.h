#ifndef CSTMT_H
#define CSTMT_H

#include "CForward.h"
#include "CInterpreter.h"
#include "CToken.h"
#include "CVisitors.h"

#include <memory>
#include <string>

class CStmt
{
public:
	virtual void accept(CStmtVisitor_Abstract<void>& visitor) = 0;
};

// Child Classes
class CStmt_Block : public CStmt, public std::enable_shared_from_this<CStmt_Block>
{
public:
	std::string _namespace = "";
	std::vector<std::shared_ptr<CStmt>> statements;
	std::shared_ptr<CToken> line;
	CStmt_Block(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CToken> line);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_Break : public CStmt, public std::enable_shared_from_this<CStmt_Break>
{
public:
	std::shared_ptr<CToken> keyword;
	CStmt_Break(std::shared_ptr<CToken> keyword);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_Class : public CStmt, public std::enable_shared_from_this<CStmt_Class>
{
public:
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::shared_ptr<CToken> name;
	std::shared_ptr<CToken> superclass;
	bool isNameDeclarationOnly;
	std::shared_ptr<CStmt> constructor;
	std::vector<std::shared_ptr<CStmt>> methods;
	std::vector<std::shared_ptr<CStmt>> properties;
	CStmt_Class(
		std::vector<std::shared_ptr<CToken>> scopeStack,
		std::shared_ptr<CToken> name, 
		std::shared_ptr<CToken> superclass,
		bool isNameDeclarationOnly,
		std::shared_ptr<CStmt> constructor,
		std::vector<std::shared_ptr<CStmt>> methods,
		std::vector<std::shared_ptr<CStmt>> properties);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_Continue : public CStmt, public std::enable_shared_from_this<CStmt_Continue>
{
public:
	std::shared_ptr<CToken> keyword;
	CStmt_Continue(std::shared_ptr<CToken> keyword);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_Expression : public CStmt, public std::enable_shared_from_this<CStmt_Expression>
{
public:
	std::shared_ptr<CToken> line;
	std::shared_ptr<CExpr> expression;
	CStmt_Expression(std::shared_ptr<CExpr> expression, std::shared_ptr<CToken> line);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_For : public CStmt, public std::enable_shared_from_this<CStmt_For>
{
public:
	std::string _namespace = "";
	std::shared_ptr<CStmt> initializer;
	std::shared_ptr<CExpr> condition;
	std::shared_ptr<CExpr> increment;
	std::shared_ptr<CStmt> body;
	std::shared_ptr<CToken> line;
	CStmt_For(
		std::shared_ptr<CStmt> initializer,
		std::shared_ptr<CExpr> condition,
		std::shared_ptr<CExpr> increment,
		std::shared_ptr<CStmt> body,
		std::shared_ptr<CToken> line);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_Foreach : public CStmt, public std::enable_shared_from_this<CStmt_Foreach>
{
public:
	std::string _namespace = "";
	std::shared_ptr<CToken> typeName;
	std::shared_ptr<CToken> iteratorName;
	std::shared_ptr<CExpr> listExpression;
	std::shared_ptr<CStmt> body;
	std::shared_ptr<CToken> line;
	CStmt_Foreach(
		std::shared_ptr<CToken> typeName,
		std::shared_ptr<CToken> iteratorName,
		std::shared_ptr<CExpr> listExpression,
		std::shared_ptr<CStmt> body,
		std::shared_ptr<CToken> line);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_Function : public CStmt, public std::enable_shared_from_this<CStmt_Function>
{
public:
	std::shared_ptr<CToken> returnType;
	std::vector<std::shared_ptr<CToken>> scopeStack;
	std::shared_ptr<CToken> name;
	bool isNameDeclarationOnly;
	std::vector<std::shared_ptr<CToken>> paramsTypes;
	std::vector<std::string> paramsNames;
	int arity = 0;
	std::shared_ptr<CStmt> body;
	CStmt_Function(
		std::shared_ptr<CToken> returnType,
		std::shared_ptr<CToken> name,
		bool isNameDeclarationOnly,
		std::vector<std::shared_ptr<CToken>> scopeStack,
		std::shared_ptr<CStmt> body);
	CStmt_Function(
		std::shared_ptr<CToken> returnType,
		std::shared_ptr<CToken> name, 
		bool isNameDeclarationOnly,
		std::vector<std::shared_ptr<CToken>> scopeStack,
		std::vector<std::shared_ptr<CToken>> paramsTypes,
		std::vector<std::string> paramsNames,
		std::shared_ptr<CStmt> body);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_Namespace : public CStmt, public std::enable_shared_from_this<CStmt_Namespace>
{
public:
	std::shared_ptr<CToken> name;
	std::shared_ptr<CStmt> body;
	CStmt_Namespace(
		std::shared_ptr<CToken> name,
		std::shared_ptr<CStmt> body);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_If : public CStmt, public std::enable_shared_from_this<CStmt_If>
{
public:
	std::string _namespace = "";
	std::vector<std::shared_ptr<CExpr>> conditionsList;
	std::vector<std::shared_ptr<CStmt>> executionList;
	std::shared_ptr<CStmt> elseBranch;
	std::shared_ptr<CToken> line;
	CStmt_If(
		std::vector<std::shared_ptr<CExpr>> conditionsList,
		std::vector<std::shared_ptr<CStmt>> executionList,
		std::shared_ptr<CStmt> elseBranch,
		std::shared_ptr<CToken> line);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_Print : public CStmt, public std::enable_shared_from_this<CStmt_Print>
{
public:
	std::shared_ptr<CExpr> expression;
	std::shared_ptr<CToken> line;
	CStmt_Print(std::shared_ptr<CExpr> expression, std::shared_ptr<CToken> line);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_Return : public CStmt, public std::enable_shared_from_this<CStmt_Return>
{
public:
	std::shared_ptr<CToken> keyword;
	std::shared_ptr<CExpr> value;
	CStmt_Return(std::shared_ptr<CToken> keyword, std::shared_ptr<CExpr> value);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_TryCatch : public CStmt, public std::enable_shared_from_this<CStmt_TryCatch>
{
public:
	std::string _namespace = "";
	std::shared_ptr<CStmt> tryBlock;
	std::shared_ptr<CStmt> catchBlock;
	std::shared_ptr<CToken> line;
	CStmt_TryCatch(std::shared_ptr<CStmt> tryBlock, std::shared_ptr<CStmt> catchBlock, std::shared_ptr<CToken> line);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_Variable : public CStmt, public std::enable_shared_from_this<CStmt_Variable>
{
public:
	std::shared_ptr<CToken> typeName;
	std::shared_ptr<CToken> name;
	std::shared_ptr<CExpr> initializer;
	CStmt_Variable(std::shared_ptr<CToken> typeName, std::shared_ptr<CToken> name, std::shared_ptr<CExpr> initializer);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

class CStmt_While : public CStmt, public std::enable_shared_from_this<CStmt_While>
{
public:
	std::string _namespace = "";
	std::shared_ptr<CExpr> condition;
	std::shared_ptr<CStmt> body;
	std::shared_ptr<CToken> line;
	CStmt_While(std::shared_ptr<CExpr> condition, std::shared_ptr<CStmt> body, std::shared_ptr<CToken> line);
	virtual void accept(CStmtVisitor_Abstract<void>& visitor);
};

#endif