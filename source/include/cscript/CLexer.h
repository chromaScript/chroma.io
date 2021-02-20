#ifndef CLEXER_H
#define CLEXER_H

#include "CEnums.h"
#include "CToken.h"
#include "ChromaScript.h"

#include <string>
#include <vector>
#include <memory>

class CLexer
{
private:
	//
	std::shared_ptr<ChromaScript> console = nullptr;
	//
	std::vector<std::shared_ptr<CToken>> tokens;
	//
	void addToken(CTokenType type, size_t line);
	void addToken(CTokenType type, std::string lexeme, size_t line);
	void addToken(CTokenType type, char lexeme, size_t line);
	//
	int countNewlines(std::string val);
protected:
public:
	//
	CLexer();
	//
	void initialize(std::shared_ptr<ChromaScript> console);
	//
	std::vector<std::shared_ptr<CToken>> scanTokens(std::string source);
	//
};

#endif
