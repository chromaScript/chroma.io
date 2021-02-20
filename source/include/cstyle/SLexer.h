#ifndef SLEXER_H
#define SLEXER_H

#include "SEnums.h"
#include "SToken.h"
#include "ChromaStyle.h"

#include <string>
#include <vector>
#include <memory>

class SLexer
{
private:
	//
	std::shared_ptr<ChromaStyle> console = nullptr;
	//
	std::vector<std::shared_ptr<SToken>> tokens;
	//
	void addToken(STokenType type, size_t line);
	void addToken(STokenType type, std::string lexeme, size_t line);
	void addToken(STokenType type, char lexeme, size_t line);
	void addToken(STokenType type, std::string lexeme, size_t line, unsigned char special);
	//
	int countNewlines(std::string val);
protected:
public:
	//
	SLexer();
	//
	void initialize(std::shared_ptr<ChromaStyle> console);
	//
	std::vector<std::shared_ptr<SToken>> scanTokens(std::string source);
	//
};

#endif
