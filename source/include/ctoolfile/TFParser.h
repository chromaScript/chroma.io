#ifndef TFPARSER_H
#define TFPARSER_H

#include "TFEnums.h"
#include "TFForward.h"
#include "ChromaToolFile.h"

#include <string>
#include <vector>
#include <memory>

class TFParser
{
private:
	//
	std::shared_ptr<ChromaToolFile> console = nullptr;
	//
	std::vector<std::shared_ptr<TFToken>> tokens;
	size_t current = 0;
	//
	std::shared_ptr<TFExpr> name();
	std::shared_ptr<TFExpr> value();
	//
	std::shared_ptr<TFStmt> toolStmt();
	std::shared_ptr<TFStmt> settingsBlockStmt();
	std::shared_ptr<TFStmt> settingStmt();
	//
	std::shared_ptr<TFToken> advance();
	std::shared_ptr<TFToken> peek();
	std::shared_ptr<TFToken> scry(size_t depth);
	std::shared_ptr<TFToken> consume(TFTokenType type, std::string message);
	std::shared_ptr<TFToken> previous();
	bool match(std::vector<TFTokenType> types);
	bool isValidInterestMask(TFTokenType type);
	bool isValidValueType(TFTokenType type);
	bool check(TFTokenType type);
	bool isAtEnd();
	//
	void synchronize();

	void error(std::shared_ptr<TFToken> token, std::string message);

protected:
public:
	//
	TFParser();
	//
	void initialize(std::shared_ptr<ChromaToolFile> console);
	//
	std::vector<std::shared_ptr<TFStmt>> parseTokens(std::vector<std::shared_ptr<TFToken>> tokens);
};

#endif
