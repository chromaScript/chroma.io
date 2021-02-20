#ifndef COPTIMIZERRED_H
#define COPTIMIZERRED_H

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

class COptimizerRed : public CExprVisitor_Abstract<std::shared_ptr<CObject>>, public CStmtVisitor_Abstract<void>,
	public std::enable_shared_from_this<COptimizerRed>
{
private:
	//
	std::shared_ptr<CEnvironment> currentEnvironment = nullptr;
	//
	void analyze(std::shared_ptr<CStmt> stmt);
	//
	bool isTrue(std::shared_ptr<CObject> object);
public:
	//
	std::shared_ptr<ChromaScript> console = nullptr;
	//
	COptimizerRed();
	//
	void initialize(std::shared_ptr<ChromaScript> console);
	//
	void optimize(std::vector<std::shared_ptr<CStmt>> statements, std::shared_ptr<CEnvironment> entryEnv);

	//
	std::shared_ptr<ChromaScript> getConsole() { return console; }
	void setEnvironment(std::shared_ptr<CEnvironment> environment) { this->currentEnvironment = environment; }
	std::shared_ptr<CEnvironment> getEnvironment() { return currentEnvironment; }
	//

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