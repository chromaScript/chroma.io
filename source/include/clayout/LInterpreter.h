#ifndef LBUILDER_H
#define LBUILDER_H

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <filesystem>

class CStmt;
class Widget;
class CObject;
class UI;
#include "../cscript/CEnvironment.h"

#include "LForward.h"
#include "LEnums.h"
#include "LVisitors.h"

class LInterpreter : public LExprVisitor_Abstract<std::shared_ptr<Widget>>, public LStmtVisitor_Abstract<void>,
	public std::enable_shared_from_this<LInterpreter>
{
private:
	//
	std::map<std::string, std::shared_ptr<LStmt>> unresolvedSymbols;
	//
	std::shared_ptr<ChromaLayout> console = nullptr;
	std::shared_ptr<UI> ui;
	std::shared_ptr<CEnvironment> currentEnvironment = nullptr;
	std::filesystem::path currentRootDir;
	bool basicOnly = false;
	std::weak_ptr<Widget> currentElement;
	std::weak_ptr<Widget> currentRoot;
	//
	std::map<std::string, std::string> basicAttribs;
	std::vector<std::shared_ptr<LExpr>> unboundAttribs;
	std::map<std::string, std::vector<std::shared_ptr<CStmt>>> callbackAttribs;
	//
	void build(std::shared_ptr<LStmt> stmt);
	void build(std::shared_ptr<LExpr> stmt);
public:
	//
	LInterpreter();
	//
	void initialize(std::shared_ptr<ChromaLayout> console);
	//
	void clearUnresolvedSymbols();
	void checkUnresolvedSymbols();
	//
	void constructWidgets(
		std::vector<std::shared_ptr<LStmt>> statements,
		std::shared_ptr<CEnvironment> rootEnvironment, 
		std::filesystem::path rootDir);
	//
	std::shared_ptr<ChromaLayout> getConsole() { return console; }
	void setEnvironment(std::shared_ptr<CEnvironment> environment) { this->currentEnvironment = environment; }
	std::shared_ptr<CEnvironment> getEnvironment() { return currentEnvironment; }
	//
	std::shared_ptr<Widget> visit(std::shared_ptr<LExpr_Element> expr);
	std::shared_ptr<Widget> visit(std::shared_ptr<LExpr_RootAttrib> expr);
	std::shared_ptr<Widget> visit(std::shared_ptr<LExpr_ElementAttrib> expr);
	//
	void visit(std::shared_ptr<LStmt_Root> stmt);
	void visit(std::shared_ptr<LStmt_Proto> stmt);
};

#endif