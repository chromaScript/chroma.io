#include "../include/cstyle/SToken.h"
#include "../include/cstyle/SEnums.h"

// Constructor
SToken::SToken() {}
// Copy Constructor
SToken::SToken(const SToken& ctoken)
{
	this->lexeme = ctoken.lexeme;
	this->type = ctoken.type;
	this->line = ctoken.line;
	this->special = ctoken.special;
}
// Overloaded Constructor
SToken::SToken(STokenType type, int line)
{
	this->type = type;
	this->lexeme = STokenTypeNames[(size_t)type];
	this->line = line;
}
SToken::SToken(STokenType type, std::string lexeme)
{
	this->type = type;
	this->lexeme = lexeme;
}
SToken::SToken(STokenType type, std::string lexeme, int line)
{
	this->type = type;
	this->lexeme = lexeme;
	this->line = line;
}
SToken::SToken(STokenType type, std::string lexeme, int line, unsigned char special)
{
	this->type = type;
	this->lexeme = lexeme;
	this->line = line;
	this->special = special;
}

// Return string from token
std::string SToken::toString()
{
	return "[" + std::to_string(line) + "] : " + STokenTypeNames[(size_t)type] + " " + lexeme;
}
// Return type string from token
std::string SToken::typeString()
{
	if (type == STokenType::IDENTIFIER || type == STokenType::SCRIPT)
	{
		return lexeme;
	}
	else
	{
		return STokenTypeNames[(size_t)type];
	}
}