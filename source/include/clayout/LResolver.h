#ifndef LRESOLVER_H
#define LRESOLVER_H

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <stack>

class Widget;
#include "../cscript/CEnvironment.h"

#include "LForward.h"
#include "LEnums.h"
#include "LVisitors.h"

class LResolver : public LExprVisitor_Abstract<std::shared_ptr<LToken>>, public LStmtVisitor_Abstract<void>,
	public std::enable_shared_from_this<LResolver>
{
private:
	//
	std::map<std::string, std::shared_ptr<LStmt>> unresolvedSymbols;
	//
	std::shared_ptr<ChromaLayout> console = nullptr;
	std::shared_ptr<CEnvironment> currentEnvironment = nullptr;
	std::string currentRootId = "";
	std::shared_ptr<LStmt> currentRoot;
	std::shared_ptr<LExpr_Element> currentElement;
	std::vector<std::string> prototypeIdList;
	//
	void resolve(std::shared_ptr<LStmt> stmt);
	void resolve(std::shared_ptr<LExpr> stmt);
public:
	//
	LResolver();
	//
	void initialize(std::shared_ptr<ChromaLayout> console);
	//
	void clearUnresolvedSymbols();
	void checkUnresolvedSymbols();
	//
	void resolveStatements(
		std::vector<std::shared_ptr<LStmt>> statements,
		std::shared_ptr<CEnvironment> rootEnvironment);
	//
	std::shared_ptr<ChromaLayout> getConsole() { return console; }
	void setEnvironment(std::shared_ptr<CEnvironment> environment) { this->currentEnvironment = environment; }
	std::shared_ptr<CEnvironment> getEnvironment() { return currentEnvironment; }
	//
	std::shared_ptr<LToken> visit(std::shared_ptr<LExpr_Element> expr);
	std::shared_ptr<LToken> visit(std::shared_ptr<LExpr_RootAttrib> expr);
	std::shared_ptr<LToken> visit(std::shared_ptr<LExpr_ElementAttrib> expr);
	//
	void visit(std::shared_ptr<LStmt_Root> stmt);
	void visit(std::shared_ptr<LStmt_Proto> stmt);
};

#endif