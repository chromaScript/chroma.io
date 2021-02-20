#include "../include/clayout/LToken.h"
#include "../include/clayout/LEnums.h"

// Constructor
LToken::LToken() {}
// Copy Constructor
LToken::LToken(const LToken& ctoken)
{
	this->lexeme = ctoken.lexeme;
	this->type = ctoken.type;
	this->line = ctoken.line;
}
// Overloaded Constructor
LToken::LToken(LTokenType type, int line)
{
	this->type = type;
	this->lexeme = LTokenTypeNames[(size_t)type];
	this->line = line;
}
LToken::LToken(LTokenType type, std::string lexeme)
{
	this->type = type;
	this->lexeme = lexeme;
}
LToken::LToken(LTokenType type, std::string lexeme, int line)
{
	this->type = type;
	this->lexeme = lexeme;
	this->line = line;
}

// Return string from token
std::string LToken::toString()
{
	return "[" + std::to_string(line) + "] : " + LTokenTypeNames[(size_t)type] + " " + lexeme;
}
// Return type string from token
std::string LToken::typeString()
{
	if (type == LTokenType::IDENTIFIER || 
		type == LTokenType::BOOL || 
		type == LTokenType::NUM || 
		type == LTokenType::STRING)
	{
		return lexeme;
	}
	else
	{
		return LTokenTypeNames[(size_t)type];
	}
}