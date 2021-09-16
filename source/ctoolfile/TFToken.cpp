#include "../include/ctoolfile/TFToken.h"
#include "../include/ctoolfile/TFEnums.h"

// Constructor
TFToken::TFToken() {}
// Copy Constructor
TFToken::TFToken(const TFToken& ctoken)
{
	this->lexeme = ctoken.lexeme;
	this->type = ctoken.type;
	this->line = ctoken.line;
}
// Overloaded Constructor
TFToken::TFToken(TFTokenType type, int line)
{
	this->type = type;
	this->lexeme = TFTokenTypeNames[(size_t)type];
	this->line = line;
}
TFToken::TFToken(TFTokenType type, std::string lexeme)
{
	this->type = type;
	this->lexeme = lexeme;
}
TFToken::TFToken(TFTokenType type, std::string lexeme, int line)
{
	this->type = type;
	this->lexeme = lexeme;
	this->line = line;
}

// Return string from token
std::string TFToken::toString()
{
	return "[" + std::to_string(line) + "] : " + TFTokenTypeNames[(size_t)type] + " " + lexeme;
}
// Return type string from token
std::string TFToken::typeString()
{
	return TFTokenTypeNames[(size_t)type];
}