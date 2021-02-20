#ifndef CPARSER_H
#define CPARSER_H

#include "CEnums.h"
#include "CForward.h"
#include "ChromaScript.h"

#include <string>
#include <vector>
#include <memory>

class CParser
{
private:
	//
	std::shared_ptr<ChromaScript> console = nullptr;
	//
	std::vector<std::shared_ptr<CToken>> tokens;
	size_t current = 0;
	//
	std::shared_ptr<CExpr> expression();
	std::shared_ptr<CExpr> assignment();
	std::shared_ptr<CExpr> logicalOr();
	std::shared_ptr<CExpr> logicalAnd();
	std::shared_ptr<CExpr> equality();
	std::shared_ptr<CExpr> comparison();
	std::shared_ptr<CExpr> approximate(std::shared_ptr<CExpr> left);
	std::shared_ptr<CExpr> term();
	std::shared_ptr<CExpr> factor();
	std::shared_ptr<CExpr> exponent();
	std::shared_ptr<CExpr> scope();
	std::shared_ptr<CExpr> typeCast();
	std::shared_ptr<CExpr> instanceOf();
	std::shared_ptr<CExpr> unary();
	std::shared_ptr<CExpr> call();
	std::shared_ptr<CExpr> finishCall(std::shared_ptr<CExpr> callee);
	std::shared_ptr<CExpr> primary();
	std::shared_ptr<CExpr> vector();
	//
	std::shared_ptr<CStmt> declarationStmt();
	std::shared_ptr<CStmt> classDeclarationStmt();
	std::shared_ptr<CStmt> funcDeclarationStmt(std::shared_ptr<CToken> typeSpecifier, std::vector<std::shared_ptr<CToken>> scopeStack, std::string kind);
	std::shared_ptr<CStmt> arrayDeclarationStmt(std::shared_ptr<CToken> typeSpecifier);
	std::shared_ptr<CStmt> varDeclarationStmt(std::shared_ptr<CToken> typeSpecifier);
	std::shared_ptr<CStmt> namespaceDeclarationStmt();
	std::shared_ptr<CStmt> statement();
	std::shared_ptr<CStmt> printStmt();
	std::shared_ptr<CStmt> returnStmt();
	std::shared_ptr<CStmt> breakStmt();
	std::shared_ptr<CStmt> continueStmt();
	std::shared_ptr<CStmt> tryCatchStmt();
	std::shared_ptr<CStmt> ifStmt();
	std::shared_ptr<CStmt> whileStmt();
	std::shared_ptr<CStmt> forStmt();
	std::shared_ptr<CStmt> foreachStmt();
	std::shared_ptr<CStmt> block();
	std::shared_ptr<CStmt> expressionStmt(bool ignoreSemicolon);
	//
	std::shared_ptr<CToken> advance();
	std::shared_ptr<CToken> peek();
	std::shared_ptr<CToken> scry(size_t depth);
	std::shared_ptr<CToken> consume(CTokenType type, std::string message);
	std::shared_ptr<CToken> createTypeSpecifier(std::shared_ptr<CToken> start);
	std::shared_ptr<CExpr> createArraySpecifier(std::shared_ptr<CToken> start);
	std::shared_ptr<CToken> previous();
	bool match(std::vector<CTokenType> types);
	bool check(CTokenType type);
	bool isValidVarType(std::shared_ptr<CToken> token);
	bool isArrayType(std::shared_ptr<CToken> token);
	bool isAtEnd();
	//
	void synchronize();
	void closeVector();
	
	void error(std::shared_ptr<CToken> token, std::string message);

protected:
public:
	//
	CParser();
	//
	void initialize(std::shared_ptr<ChromaScript> console);
	//
	std::vector<std::shared_ptr<CStmt>> parseTokens(std::vector<std::shared_ptr<CToken>> tokens);
	std::vector<std::shared_ptr<CStmt>> parseTokensExpr(std::vector<std::shared_ptr<CToken>> tokens);
};

#endif
