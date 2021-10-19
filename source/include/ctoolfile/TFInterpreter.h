#ifndef TFINTERPRETER_H
#define TFINTERPRETER_H

#include <memory>
#include <vector>
#include <string>
#include <map>

#include <glm.hpp>

#include "TFEnums.h"
#include "TFExpr.h"
#include "../cscript/CObject.h"
#include "TFForward.h"
#include "TFVisitors.h"
#include "TFToken.h"

#include "../tool/ToolSettings.h"
#include "../tool/Toolbox.h"

class TFInterpreter : public TFExprVisitor_Abstract<std::shared_ptr<CObject>>, public TFStmtVisitor_Abstract<void>,
	public std::enable_shared_from_this<TFInterpreter>
{
private:
	std::shared_ptr<Toolbox> currentToolbox = nullptr;
	std::shared_ptr<Tool> currentTool = nullptr;
	TSetType currentInterestMask = TSetType::null;
	std::map<std::pair<int, int>, std::shared_ptr<CObject>> currentSettingsMap;
	//
	int currentSettingID = -1;
	int currentSubSettingID = -1;
	//
	bool isMetaSearch = false;
	bool foundMeta = false;
	int toolsCount = 0;
	//
	void buildTool(std::shared_ptr<TFStmt> stmt, std::shared_ptr<Toolbox> targetBox);
	//
	TSetType convertTFTokenType_toTSetType(TFTokenType tokenType);
public:
	//
	std::shared_ptr<ChromaToolFile> console = nullptr;
	//
	TFInterpreter();
	void initialize(std::shared_ptr<ChromaToolFile> console);
	//
	void interpret(std::vector<std::shared_ptr<TFStmt>> toolStatements, std::shared_ptr<Toolbox> targetBox);

	std::shared_ptr<ChromaToolFile> getConsole() { return console; }

	std::shared_ptr<CObject> visit(std::shared_ptr<TFExpr_Value> expr);
	std::shared_ptr<CObject> visit(std::shared_ptr<TFExpr_Name> expr);

	void visit(std::shared_ptr<TFStmt_Tool> stmt);
	void visit(std::shared_ptr<TFStmt_SettingsBlock> stmt);
	void visit(std::shared_ptr<TFStmt_Setting> stmt);
};

#endif