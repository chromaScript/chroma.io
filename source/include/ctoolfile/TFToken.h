#ifndef TFTOKEN_H
#define TFTOKEN_H

#include "TFEnums.h"
#include "TFForward.h"

#include <string>
#include <memory>

class TFToken
{
public:
	TFTokenType type = TFTokenType::NIL;
	std::string lexeme = "";
	int line = 0;
	// Constructors
	TFToken();
	TFToken(const TFToken& ctoken);
	TFToken(TFTokenType type, int line);
	TFToken(TFTokenType type, std::string lexeme);
	TFToken(TFTokenType type, std::string lexeme, int line);
	// Return Line, Identifier, Type From Token
	std::string toString();
	// Return Type String From Token
	std::string typeString();
};

#endif