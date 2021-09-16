#include "../include/cscript/CToken.h"
#include "../include/cscript/CEnums.h"

// Constructor
CToken::CToken() {}
// Copy Constructor
CToken::CToken(const CToken& ctoken)
{
	this->lexeme = ctoken.lexeme;
	this->type = ctoken.type;
	this->line = ctoken.line;
}
// Overloaded Constructor
CToken::CToken(CTokenType type, int line)
{
	this->type = type;
	this->lexeme = CTokenTypeNames[(size_t)type];
	this->line = line;
}
CToken::CToken(CTokenType type, std::string lexeme)
{
	this->type = type;
	this->lexeme = lexeme;
}
CToken::CToken(CTokenType type, std::string lexeme, int line)
{
	this->type = type;
	this->lexeme = lexeme;
	this->line = line;
}
// Array type constructor
CToken::CToken(CTokenType arr, CTokenType type, std::string lexeme, int line)
{
	this->lexeme = lexeme;
	this->line = line;
	switch (type)
	{
	case CTokenType::ANY:
		this->type = CTokenType::ANY_ARRAY;
		break;
	case CTokenType::NUM:
		this->type = CTokenType::NUM_ARRAY;
		break;
	case CTokenType::VECTOR2:
		this->type = CTokenType::VECTOR2_ARRAY;
		break;
	case CTokenType::VECTOR3:
		this->type = CTokenType::VECTOR3_ARRAY;
		break;
	case CTokenType::VECTOR4:
		this->type = CTokenType::VECTOR4_ARRAY;
		break;
	case CTokenType::STRING:
		this->type = CTokenType::STRING_ARRAY;
		break;
	case CTokenType::BOOL:
		this->type = CTokenType::BOOL_ARRAY;
		break;
	case CTokenType::IDENTIFIER:
		this->type = CTokenType::IDENTIFIER_ARRAY;
		break;
	default:
		this->type = CTokenType::VOID_ARRAY;
		break;
	}
}

// Return string from token
std::string CToken::toString()
{
	return "[" + std::to_string(line) + "] : " + CTokenTypeNames[(size_t)type] + " " + lexeme;
}
// Return type string from token
std::string CToken::typeString()
{
	if (type == CTokenType::IDENTIFIER)
	{
		return lexeme;
	}
	else if (type == CTokenType::IDENTIFIER_ARRAY)
	{
		return "array " + CTokenTypeNames[(size_t)type];
	}
	else
	{
		return CTokenTypeNames[(size_t)type];
	}
}