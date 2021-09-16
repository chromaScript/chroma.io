#include "../include/cscript/CLiteral.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CToken.h"

CLiteral::CLiteral()
{
	this->type = CLiteralTypes::_CNil;
	this->litName = "nil";
}
CLiteral::CLiteral(CLiteralTypes type, std::string litName)
{
	this->type = type;
	this->litName = litName;
}
CLiteral::CLiteral(std::shared_ptr<CToken> token)
{
	// litName is used in order to store user defined literal (class) names
	this->litName = token.get()->lexeme;
	switch (token.get()->type)
	{
	case CTokenType::IDENTIFIER:
		this->type = CLiteralTypes::_CInstance;
		break;
	case CTokenType::IDENTIFIER_ARRAY:
		this->type = CLiteralTypes::_CInstance_Array;
		break;
	case CTokenType::NUM:
		this->type = CLiteralTypes::_CNumber;
		break;
	case CTokenType::NUM_ARRAY:
		this->type = CLiteralTypes::_CNumber_Array;
		break;
	case CTokenType::BOOL:
		this->type = CLiteralTypes::_CBool;
		break;
	case CTokenType::BOOL_ARRAY:
		this->type = CLiteralTypes::_CBool_Array;
		break;
	case CTokenType::STRING:
		this->type = CLiteralTypes::_CString;
		break;
	case CTokenType::STRING_ARRAY:
		this->type = CLiteralTypes::_CString_Array;
		break;
	case CTokenType::VECTOR2:
		this->type = CLiteralTypes::_CVec2;
		break;
	case CTokenType::VECTOR2_ARRAY:
		this->type = CLiteralTypes::_CVec2_Array;
		break;
	case CTokenType::VECTOR3:
		this->type = CLiteralTypes::_CVec3;
		break;
	case CTokenType::VECTOR3_ARRAY:
		this->type = CLiteralTypes::_CVec3_Array;
		break;
	case CTokenType::VECTOR4:
		this->type = CLiteralTypes::_CVec4;
		break;
	case CTokenType::VECTOR4_ARRAY:
		this->type = CLiteralTypes::_CVec4_Array;
		break;
	case CTokenType::_THIS:
		this->type = CLiteralTypes::_CThis;
		break;
	case CTokenType::FUNCTION:
		this->type = CLiteralTypes::_CFunction;
		break;
	case CTokenType::FUNCTION_ARRAY:
		this->type = CLiteralTypes::_CFunction_Array;
		break;
	case CTokenType::NIL:
	default:
		this->type = CLiteralTypes::_CNil;
		break;
	}
	this->litName = litName;
}