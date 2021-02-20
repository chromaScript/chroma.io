#ifndef SRESOLVER_H
#define SRESOLVER_H

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <filesystem>

#include "../cscript/CEnvironment.h"

#include "SForward.h"
#include "SEnums.h"
#include "SVisitors.h"
class CInterpreter;

class SResolver : public SExprVisitor_Abstract<void>, public SStmtVisitor_Abstract<void>,
	public std::enable_shared_from_this<SResolver>
{
private:
	//
	std::map<std::string, std::shared_ptr<SStmt>> unresolvedSymbols;
	//
	std::shared_ptr<ChromaStyle> console = nullptr;
	std::shared_ptr<CEnvironment> currentEnvironment = nullptr;
	std::filesystem::path currentRootDir;
	std::string currentNamespace = "";
	//
	void resolve(std::shared_ptr<SStmt> stmt);
	void resolve(std::shared_ptr<SExpr> stmt);
	//
	std::string scopeToString(std::vector<std::shared_ptr<SToken>> scopeStack);
	std::string getCurrentAddress(std::shared_ptr<CEnvironment> environment);
public:
	//
	SResolver();
	//
	void initialize(std::shared_ptr<ChromaStyle> console);
	//
	void clearUnresolvedSymbols();
	void checkUnresolvedSymbols();
	//
	void resolveStatements(
		std::vector<std::shared_ptr<SStmt>> statements, 
		std::shared_ptr<CEnvironment> rootEnvironment,
		std::filesystem::path rootDir);
	//
	std::shared_ptr<ChromaStyle> getConsole() { return console; }
	void setEnvironment(std::shared_ptr<CEnvironment> environment) { this->currentEnvironment = environment; }
	std::shared_ptr<CEnvironment> getEnvironment() { return currentEnvironment; }
	//
	void visit(std::shared_ptr<SExpr_Property> expr);
	//
	void visit(std::shared_ptr<SStmt_Script> stmt);
	void visit(std::shared_ptr<SStmt_Style> stmt);
};

#endif