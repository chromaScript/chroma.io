#ifndef SPARSER_H
#define SPARSER_H

#include "SEnums.h"
#include "SForward.h"
#include "ChromaStyle.h"

#include <string>
#include <vector>
#include <memory>

class SParser
{
private:
	//
	std::shared_ptr<ChromaStyle> console = nullptr;
	//
	std::vector<std::shared_ptr<SToken>> tokens;
	size_t current = 0;
	//
	std::shared_ptr<SStmt> declaration();
	//
	std::shared_ptr<SStmt> styleStmt();
	std::shared_ptr<SStmt> scriptStmt();
	//
	std::shared_ptr<SExpr> styleProperty();
	//
	std::shared_ptr<SToken> advance();
	std::shared_ptr<SToken> peek();
	std::shared_ptr<SToken> scry(size_t depth);
	std::shared_ptr<SToken> consume(STokenType type, std::string message);
	std::shared_ptr<SToken> previous();
	bool match(std::vector<STokenType> types);
	bool check(std::vector<STokenType> types);
	bool isValidPropertyType(std::shared_ptr<SToken> token);
	bool isValidValueType(std::shared_ptr<SToken> token);
	bool isAtEnd();
	//
	void synchronize();

	void error(std::shared_ptr<SToken> token, std::string message);
public:
	//
	SParser();
	//
	void initialize(std::shared_ptr<ChromaStyle> console);
	//
	std::vector<std::shared_ptr<SStmt>> parseTokens(std::vector<std::shared_ptr<SToken>> tokens);
};

#endif