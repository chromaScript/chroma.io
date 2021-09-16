#ifndef TFSTMT_H
#define TFSTMT_H

#include "TFForward.h"
#include "TFInterpreter.h"
#include "TFToken.h"
#include "TFVisitors.h"

#include <memory>
#include <string>

class TFStmt
{
public:
	virtual void accept(TFStmtVisitor_Abstract<void>& visitor) = 0;
};

class TFStmt_Tool : public TFStmt, public std::enable_shared_from_this<TFStmt_Tool>
{
public:
	std::vector<std::shared_ptr<TFStmt>> statements;
	std::shared_ptr<TFToken> line;
	TFStmt_Tool(std::vector<std::shared_ptr<TFStmt>> statements, std::shared_ptr<TFToken> line);
	virtual void accept(TFStmtVisitor_Abstract<void>& visitor);
};

class TFStmt_SettingsBlock : public TFStmt, public std::enable_shared_from_this<TFStmt_SettingsBlock>
{
public:
	TFTokenType type;
	std::vector<std::shared_ptr<TFStmt>> settings;
	std::shared_ptr<TFToken> line;
	TFStmt_SettingsBlock(TFTokenType type, std::vector<std::shared_ptr<TFStmt>> settings, std::shared_ptr<TFToken> line);
	virtual void accept(TFStmtVisitor_Abstract<void>& visitor);
};

class TFStmt_Setting : public TFStmt, public std::enable_shared_from_this<TFStmt_Setting>
{
public:
	std::shared_ptr<TFExpr> name;
	std::shared_ptr<TFExpr> value;
	std::shared_ptr<TFToken> line;
	TFStmt_Setting(std::shared_ptr<TFExpr> name, std::shared_ptr<TFExpr> value, std::shared_ptr<TFToken> line);
	virtual void accept(TFStmtVisitor_Abstract<void>& visitor);
};

#endif