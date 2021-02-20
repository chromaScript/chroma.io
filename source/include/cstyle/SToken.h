#ifndef STOKEN_H
#define STOKEN_H

#include "SEnums.h"
#include "SForward.h"

#include <string>
#include <memory>

class SToken
{
public:
	STokenType type = STokenType::NIL;
	std::string lexeme = "";
	int line = 0;
	unsigned char special = 0;
	// Constructors
	SToken();
	SToken(const SToken& ctoken);
	SToken(STokenType type, int line);
	SToken(STokenType type, std::string lexeme);
	SToken(STokenType type, std::string lexeme, int line);
	SToken(STokenType type, std::string lexeme, int line, unsigned char special);
	SToken(STokenType arr, STokenType type, std::string lexeme, int line);
	// Return Line, Identifier, Type From Token
	std::string toString();
	// Return Type String From Token
	std::string typeString();
};

#endif