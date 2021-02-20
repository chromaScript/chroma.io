#ifndef SINTERPRETER_H
#define SINTERPRETER_H

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <filesystem>

class ChromaScript;

#include "SEnums.h"
#include "SProc.h"
#include "../cscript/CObject.h"
#include "SForward.h"
#include "../cscript/CEnvironment.h"
#include "SVisitors.h"
#include "SToken.h"

class SInterpreter : public SExprVisitor_Abstract<void>, public SStmtVisitor_Abstract<void>,
	public std::enable_shared_from_this<SInterpreter>
{
private:
	std::shared_ptr<WidgetStyle> currentStyle = nullptr;
	//
public:
	//
	std::shared_ptr<ChromaStyle> console = nullptr;

	std::shared_ptr<CEnvironment> currentEnvironment = nullptr;
	std::filesystem::path currentRootDir;
	//
	SInterpreter();
	void initialize(std::shared_ptr<ChromaStyle> console);
	//
	void interpret(std::vector<std::shared_ptr<SStmt>> statements, std::shared_ptr<CEnvironment> entryEnv, std::filesystem::path rootDir);
	//
	void visit(std::shared_ptr<SExpr_Property> expr);
	//
	void visit(std::shared_ptr<SStmt_Script> stmt);
	void visit(std::shared_ptr<SStmt_Style> stmt);
};

#endif