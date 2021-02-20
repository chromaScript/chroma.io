#include "../include/cscript/COptimizerRed.h"
#include "../include/cscript/CEnvironment.h"
#include "../include/cscript/ChromaScript.h"
#include "../include/cscript/CForward.h"
#include "../include/cscript/CExpr.h"
#include "../include/cscript/CStmt.h"
#include "../include/cscript/CEnums.h"
#include "../include/cscript/CToken.h"
#include "../include/cscript/CObject.h"
#include "../include/cscript/CCallable.h"
#include "../include/cscript/CCallable_Lib.h"

#include <string>
#include <vector>
#include <exception>

//
COptimizerRed::COptimizerRed()
{

}

void COptimizerRed::initialize(std::shared_ptr<ChromaScript> console)
{
	this->console = console;
}
//
void COptimizerRed::optimize(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CEnvironment> entryEnv)
{
	setEnvironment(entryEnv);
	for (std::shared_ptr<CStmt> stmt : statements)
	{
		analyze(stmt);
	}
	setEnvironment(console.get()->global);
}

//
void COptimizerRed::analyze(std::shared_ptr<CStmt> stmt)
{
	stmt.get()->accept(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Visit Statements
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CStmt_Block
void COptimizerRed::visit(std::shared_ptr<CStmt_Block> stmt)
{

}
// CStmt_Break
void COptimizerRed::visit(std::shared_ptr<CStmt_Break> stmt)
{

}
// CStmt_Class
void COptimizerRed::visit(std::shared_ptr<CStmt_Class> stmt)
{

}
// CStmt_Continue
void COptimizerRed::visit(std::shared_ptr<CStmt_Continue> stmt)
{

}
// CStmt_Expression
void COptimizerRed::visit(std::shared_ptr<CStmt_Expression> stmt)
{

}
// CStmt_For
void COptimizerRed::visit(std::shared_ptr<CStmt_For> stmt)
{

}
// CStmt_Foreach
void COptimizerRed::visit(std::shared_ptr<CStmt_Foreach> stmt)
{

}
// CStmt_Function
void COptimizerRed::visit(std::shared_ptr<CStmt_Function> stmt)
{

}
// CStmt_Namespace
void COptimizerRed::visit(std::shared_ptr<CStmt_Namespace> stmt)
{

}
// CStmt_If
void COptimizerRed::visit(std::shared_ptr<CStmt_If> stmt)
{

}
// CStmt_Print
void COptimizerRed::visit(std::shared_ptr<CStmt_Print> stmt)
{

}
// CStmt_Return
void COptimizerRed::visit(std::shared_ptr<CStmt_Return> stmt)
{

}
// CStmt_TryCatch
void COptimizerRed::visit(std::shared_ptr<CStmt_TryCatch> stmt)
{

}
// CStmt_Variable
void COptimizerRed::visit(std::shared_ptr<CStmt_Variable> stmt)
{

}
// CStmt_While
void COptimizerRed::visit(std::shared_ptr<CStmt_While> stmt)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Visit Expressions
//
////////////////////////////////////////////////////////////////////////////////////////////////

// CExpr_Approximate
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Approximate> expr)
{
	return nullptr;
}
// CExpr_Array
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Array> stmt)
{
	return nullptr;
}
// CExpr_Assign
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Assign> expr)
{
	return nullptr;
}
// CExpr_Binary
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Binary> expr)
{
	return nullptr;
}
// CExpr_Call
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Call> expr)
{
	return nullptr;
}
// CExpr_Get
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Get> expr)
{
	return nullptr;
}
// CExpr_Grouping
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Grouping> expr)
{
	return nullptr;
}
// CExpr_InstanceOf
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_InstanceOf> expr)
{
	return nullptr;
}
// CExpr_Literal
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Literal> expr)
{
	return nullptr;
}
// CExpr_Logical
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Logical> expr)
{
	return nullptr;
}
// CExpr_Scope
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Scope> expr)
{
	return nullptr;
}
// CExpr_TypeCast
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_TypeCast> expr)
{
	return nullptr;
}
// CExpr_Unary
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Unary> expr)
{
	return nullptr;
}
// CExpr_Variable
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Variable> expr)
{
	return nullptr;
}
// CExpr_Vector
std::shared_ptr<CObject> COptimizerRed::visit(std::shared_ptr<CExpr_Vector> expr)
{
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// COptimizerRed Utility Functions
//
////////////////////////////////////////////////////////////////////////////////////////////////

//
bool COptimizerRed::isTrue(std::shared_ptr<CObject> object)
{
	return false;
}