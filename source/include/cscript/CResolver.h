#ifndef CRESOLVER_H
#define CRESOLVER_H

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <stack>

#include "CForward.h"
#include "CEnums.h"
#include "CVisitors.h"
class CInterpreter;

class CResolver : public CExprVisitor_Abstract<void>, public CStmtVisitor_Abstract<void>, 
	public std::enable_shared_from_this<CResolver>
{
private:
	//
	std::map<std::string, std::shared_ptr<CStmt>> unresolvedSymbols;
	//
	std::shared_ptr<ChromaScript> console = nullptr;
	std::shared_ptr<CEnvironment> currentEnvironment = nullptr;
	std::string currentNamespace = "";
	//
	void resolve(std::shared_ptr<CStmt> stmt);
	void resolve(std::shared_ptr<CExpr> stmt);
	//
	std::string scopeToString(std::vector<std::shared_ptr<CToken>> scopeStack);
	std::string paramsToString(std::vector<std::string> names, 
		std::vector<std::shared_ptr<CToken>> types);
	std::string getCurrentAddress(std::shared_ptr<CEnvironment> environment);
public:
	//
	CResolver();
	//
	void initialize(std::shared_ptr<ChromaScript> console);
	//
	void clearUnresolvedSymbols();
	void checkUnresolvedSymbols();
	//
	void resolveStatements(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CEnvironment> rootEnvironment);
	//
	std::shared_ptr<ChromaScript> getConsole() { return console; }
	void setEnvironment(std::shared_ptr<CEnvironment> environment) { this->currentEnvironment = environment; }
	std::shared_ptr<CEnvironment> getEnvironment() { return currentEnvironment; }
	//
	void visit(std::shared_ptr<CExpr_Approximate> expr);
	void visit(std::shared_ptr<CExpr_Array> stmt);
	void visit(std::shared_ptr<CExpr_Assign> expr);
	void visit(std::shared_ptr<CExpr_Binary> expr);
	void visit(std::shared_ptr<CExpr_Call> expr);
	void visit(std::shared_ptr<CExpr_Get> expr);
	void visit(std::shared_ptr<CExpr_Grouping> expr);
	void visit(std::shared_ptr<CExpr_InstanceOf> expr);
	void visit(std::shared_ptr<CExpr_Literal> expr);
	void visit(std::shared_ptr<CExpr_Logical> expr);
	void visit(std::shared_ptr<CExpr_Scope> expr);
	void visit(std::shared_ptr<CExpr_TypeCast> expr);
	void visit(std::shared_ptr<CExpr_Unary> expr);
	void visit(std::shared_ptr<CExpr_Variable> expr);
	void visit(std::shared_ptr<CExpr_Vector> expr);
	//
	void visit(std::shared_ptr<CStmt_Block> stmt);
	void visit(std::shared_ptr<CStmt_Break> stmt);
	void visit(std::shared_ptr<CStmt_Class> stmt);
	void visit(std::shared_ptr<CStmt_Continue> stmt);
	void visit(std::shared_ptr<CStmt_Expression> stmt);
	void visit(std::shared_ptr<CStmt_For> stmt);
	void visit(std::shared_ptr<CStmt_Foreach> stmt);
	void visit(std::shared_ptr<CStmt_Function> stmt);
	void visit(std::shared_ptr<CStmt_Namespace> stmt);
	void visit(std::shared_ptr<CStmt_If> stmt);
	void visit(std::shared_ptr<CStmt_Print> stmt);
	void visit(std::shared_ptr<CStmt_Return> stmt);
	void visit(std::shared_ptr<CStmt_TryCatch> stmt);
	void visit(std::shared_ptr<CStmt_Variable> stmt);
	void visit(std::shared_ptr<CStmt_While> stmt);
};

#endif