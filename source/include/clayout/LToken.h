#ifndef LTOKEN_H
#define LTOKEN_H

#include "LEnums.h"
#include "LForward.h"

#include <string>
#include <memory>

class LToken
{
public:
	LTokenType type = LTokenType::NIL;
	std::string lexeme = "";
	int line = 0;
	// Constructors
	LToken();
	LToken(const LToken& ctoken);
	LToken(LTokenType type, int line);
	LToken(LTokenType type, std::string lexeme);
	LToken(LTokenType type, std::string lexeme, int line);
	LToken(LTokenType arr, LTokenType type, std::string lexeme, int line);
	// Return Line, Identifier, Type From Token
	std::string toString();
	// Return Type String From Token
	std::string typeString();
};

#endif