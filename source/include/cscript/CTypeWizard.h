#ifndef CTYPEWIZARD_H
#define CTYPEWIZARD_H

#include <memory>
#include <vector>
#include <string>
#include <map>

#include "CEnums.h"
#include "CExpr.h"
#include "CObject.h"
#include "CCallable.h"
#include "CCallable_Lib.h"
#include "CForward.h"
#include "CVisitors.h"
#include "CToken.h"

class CTypeWizard : public CExprVisitor_Abstract<std::shared_ptr<CToken>>, public CStmtVisitor_Abstract<void>,
	public std::enable_shared_from_this<CTypeWizard>
{
private:
	//
	std::shared_ptr<ChromaScript> console = nullptr;
	std::shared_ptr<CEnvironment> currentEnvironment = nullptr;
	std::shared_ptr<CEnvironment> lastEnvironment = nullptr;
	std::string currentGetExprKey = "";
	std::string variableGuardKey = "";
	std::string indexGuardKey = "";
	std::vector<std::string> currentNamespaces;
	//
	std::vector<std::shared_ptr<CToken>> expectedReturnStack;
	std::shared_ptr<CReturn> currentReturn = nullptr;
	//
	std::vector<std::string> getSuperPropertyNames(std::shared_ptr<CObject> superClass);
	std::vector<std::string> getSuperPropertyNames(std::shared_ptr<CClass> superClass);
	//
	bool checkNamespaces(std::string delim);
	std::shared_ptr<CEnvironment> getNamespaceEnvironment(std::string delim);
	void storeGetKey(std::shared_ptr<CExpr_Call> expr, std::shared_ptr<CEnvironment> environment);
	void checkGetKey(std::shared_ptr<CExpr_Call> expr, std::shared_ptr<CEnvironment> previous);
	//
	bool isValidCast(std::shared_ptr<CToken> type, std::shared_ptr<CToken> query);
	std::shared_ptr<CToken> isArrayType(std::shared_ptr<CToken> type);
	bool matchTokens(std::shared_ptr<CToken> left, std::shared_ptr<CToken> right);
	bool matchType(std::shared_ptr<CToken> left, CTokenType right);
	bool matchTypes(std::shared_ptr<CToken> query, std::vector<CTokenType> types);
	bool matchContainer(std::shared_ptr<CToken> iterator, std::shared_ptr<CToken> container);
	std::shared_ptr<CToken> convertToContainer(std::shared_ptr<CToken> unitType);
	//
	std::shared_ptr<CToken> makeCToken(std::shared_ptr<CObject> object, int line);
	std::shared_ptr<CToken> convertToUnit(std::shared_ptr<CObject> object, int line);
public:
	
	//
	CTypeWizard();
	//
	void initialize(std::shared_ptr<ChromaScript> console);
	//
	void checkTypes(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CEnvironment> environment);
	//

	std::shared_ptr<ChromaScript> getConsole() { return console; }
	void setEnvironment(std::shared_ptr<CEnvironment> environment) { this->currentEnvironment = environment; }
	std::shared_ptr<CEnvironment> getEnvironment() { return currentEnvironment; }
	//

	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Approximate> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Array> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Assign> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Binary> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Call> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Get> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Grouping> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_InstanceOf> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Literal> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Logical> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Scope> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_TypeCast> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Unary> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Variable> expr);
	std::shared_ptr<CToken> visit(std::shared_ptr<CExpr_Vector> expr);

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