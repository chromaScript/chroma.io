#ifndef CTOKEN_H
#define CTOKEN_H

#include "CEnums.h"
#include "CForward.h"

#include <string>
#include <memory>

class CToken
{
public:
	CTokenType type = CTokenType::NIL;
	std::string lexeme = "";
	int line = 0;
	// Constructors
	CToken();
	CToken(const CToken& ctoken);
	CToken(CTokenType type, int line);
	CToken(CTokenType type, std::string lexeme);
	CToken(CTokenType type, std::string lexeme, int line);
	CToken(CTokenType arr, CTokenType type, std::string lexeme, int line);
	// Return Line, Identifier, Type From Token
	std::string toString();
	// Return Type String From Token
	std::string typeString();
};

#endif