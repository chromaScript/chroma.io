#ifndef LPARSER_H
#define LPARSER_H

#include "LEnums.h"
#include "LForward.h"
#include "ChromaLayout.h"

#include <string>
#include <vector>
#include <memory>

class LParser
{
private:
	std::vector<std::shared_ptr<LToken>> openTags;
	//
	std::shared_ptr<ChromaLayout> console = nullptr;
	//
	std::vector<std::shared_ptr<LToken>> tokens;
	size_t current = 0;
	//
	std::shared_ptr<LStmt> declaration();
	//
	std::shared_ptr<LStmt> rootStmt();
	std::shared_ptr<LStmt> prototypeStmt();
	//
	std::shared_ptr<LExpr> element();
	std::shared_ptr<LExpr> rootAttrib();
	std::shared_ptr<LExpr> elementAttrib();
	//
	std::shared_ptr<LToken> advance();
	std::shared_ptr<LToken> peek();
	std::shared_ptr<LToken> scry(size_t depth);
	std::shared_ptr<LToken> consume(LTokenType type, std::string message);
	std::shared_ptr<LToken> previous();
	bool match(std::vector<LTokenType> types);
	bool check(std::vector<LTokenType> types);
	bool check(std::shared_ptr<LToken> query, std::vector<LTokenType> types);
	bool isValidElementType(std::shared_ptr<LToken> token);
	bool isValidEleAttribType(std::shared_ptr<LToken> token);
	bool isValidRootAttribType(std::shared_ptr<LToken> token);
	bool isValidValueType(std::shared_ptr<LToken> token);
	bool isAtEnd();
	//
	void synchronize();

	void error(std::shared_ptr<LToken> token, std::string message);
public:
	//
	LParser();
	//
	void initialize(std::shared_ptr<ChromaLayout> console);
	//
	std::vector<std::shared_ptr<LStmt>> parseTokens(std::vector<std::shared_ptr<LToken>> tokens);
};

#endif