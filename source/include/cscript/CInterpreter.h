#ifndef CINTERPRETER_H
#define CINTERPRETER_H

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
#include "CEnvironment.h"
#include "CVisitors.h"
#include "CToken.h"

class CInterpreter : public CExprVisitor_Abstract<std::shared_ptr<CObject>>, public CStmtVisitor_Abstract<void>,
	public std::enable_shared_from_this<CInterpreter>
{
private:
	//
	bool foundBreak = false;
	bool foundContinue = false;
	bool foundReturn = false;
	bool shouldExit = false;
	//
	void execute(std::shared_ptr<CStmt> stmt);
	//
	bool isTrue(std::shared_ptr<CObject> object);
	//
	bool checkNamespaces(std::string delim);
	//
	void storeGetKey(std::shared_ptr<CExpr_Call> expr, std::shared_ptr<CEnvironment> environment);
	void checkGetKey(std::shared_ptr<CExpr_Call> expr, std::shared_ptr<CEnvironment> previous);
	//
	std::shared_ptr<CObject> makeArrayObj(std::vector<std::shared_ptr<CExpr>> expressions);
	//
	std::shared_ptr<CObject> assign(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	//
	std::shared_ptr<CObject> add(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> subtract(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> multiply(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> divide(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> modulo(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> power(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> falsify(std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> negate(std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> increment(std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> decrement(std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> logicalOr(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> logicalAnd(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> isEqual(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> isMore(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> isMoreEqual(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> isLess(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	std::shared_ptr<CObject> isLessEqual(std::shared_ptr<CObject> left, std::shared_ptr<CObject> right);
	//
	CLiteralTypes tokenType_ToLiteralType(CTokenType type);
	std::shared_ptr<CObject> matchType(std::shared_ptr<CToken> type, CLiteral query);
public:
	//
	std::shared_ptr<ChromaScript> console = nullptr;

	std::shared_ptr<CEnvironment> currentEnvironment = nullptr;
	std::shared_ptr<CEnvironment> currentThisInstance = nullptr;

	std::string currentGetExprKey = "";
	std::string variableGuardKey = "";
	std::string currentAssignee = "";
	std::vector<std::string> currentNamespaces;
	
	std::vector<std::shared_ptr<CEnvironment>> currentFunctions;
	//
	CInterpreter();
	void initialize(std::shared_ptr<ChromaScript> console);
	void resetInterpreter();
	//
	void interpret(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CEnvironment> entryEnv);
	std::shared_ptr<CObject> interpretLine(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CEnvironment> entryEnv);

	std::shared_ptr<ChromaScript> getConsole() { return console; }
	void setEnvironment(std::shared_ptr<CEnvironment> environment) { this->currentEnvironment = environment; }
	std::shared_ptr<CEnvironment> getEnvironment();
	std::shared_ptr<CEnvironment> getEnvironment(std::string name);
	//
	void addNamespace(std::string name) { currentNamespaces.push_back(name); }
	void popNamespace() { currentNamespaces.pop_back(); }

	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Approximate> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Array> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Assign> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Binary> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Call> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Get> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Grouping> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_InstanceOf> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Literal> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Logical> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Scope> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_TypeCast> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Unary> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Variable> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<CExpr_Vector> expr);

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