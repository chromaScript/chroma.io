#ifndef TFLEXER_H
#define TFLEXER_H

#include "TFEnums.h"
#include "TFToken.h"
#include "ChromaToolFile.h"

#include <string>
#include <vector>
#include <memory>

class TFLexer
{
private:
	//
	std::shared_ptr<ChromaToolFile> console = nullptr;
	//
	std::vector<std::shared_ptr<TFToken>> tokens;
	//
	void addToken(TFTokenType type, size_t line);
	void addToken(TFTokenType type, std::string lexeme, size_t line);
	void addToken(TFTokenType type, char lexeme, size_t line);
	//
	int countNewlines(std::string val);
protected:
public:
	//
	TFLexer();
	//
	void initialize(std::shared_ptr<ChromaToolFile> console);
	//
	std::vector<std::shared_ptr<TFToken>> scanTokens(std::string source);
	//
};

#endif
