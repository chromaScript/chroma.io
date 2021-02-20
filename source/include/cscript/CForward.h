#ifndef CFORWARD_H
#define CFORWARD_H

#include <random>
//
class ChromaScript;
class CEnvironment;
class CInterpreter;
class CObject;
class CToken;
class CLiteral;
class CReturn;
class WidgetStyle;

//
class CExpr;
class CExpr_Approximate;
class CExpr_Array;
class CExpr_Assign;
class CExpr_Binary;
class CExpr_Call;
class CExpr_Get;
class CExpr_Grouping;
class CExpr_InstanceOf;
class CExpr_Literal;
class CExpr_Logical;
class CExpr_Scope;
class CExpr_TypeCast;
class CExpr_Unary;
class CExpr_Variable;
class CExpr_Vector;

//
class CStmt;
class CStmt_Block;
class CStmt_Break;
class CStmt_Class;
class CStmt_Continue;
class CStmt_Expression;
class CStmt_For;
class CStmt_Foreach;
class CStmt_Function;
class CStmt_Namespace;
class CStmt_If;
class CStmt_Print;
class CStmt_Return;
class CStmt_TryCatch;
class CStmt_Variable;
class CStmt_While;

//
const size_t anonLength = 16;
//
inline std::string randomNamespace(size_t length)
{
	const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<size_t> distribution(size_t(0), size_t(CHARACTERS.size() - 1));

	std::string output;

	for (std::size_t i = 0; i < length; ++i)
	{
		output += CHARACTERS[distribution(generator)];
	}

	return output;
}

#endif