#ifndef LLEXER_H
#define LLEXER_H

#include "LEnums.h"
#include "LToken.h"
#include "ChromaLayout.h"

#include <string>
#include <vector>
#include <memory>

class LLexer
{
private:
	//
	std::shared_ptr<ChromaLayout> console = nullptr;
	//
	std::vector<std::shared_ptr<LToken>> tokens;
	//
	void addToken(LTokenType type, size_t line);
	void addToken(LTokenType type, std::string lexeme, size_t line);
	void addToken(LTokenType type, char lexeme, size_t line);
	//
	int countNewlines(std::string val);
protected:
public:
	//
	LLexer();
	//
	void initialize(std::shared_ptr<ChromaLayout> console);
	//
	std::vector<std::shared_ptr<LToken>> scanTokens(std::string source);
	//
};

#endif
