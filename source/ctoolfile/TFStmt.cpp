#include "../include/ctoolfile/TFStmt.h"
#include "../include/ctoolfile/TFExpr.h"
#include "../include/ctoolfile/TFToken.h"
#include "../include/ctoolfile/TFVisitors.h"

#include <memory>

// TFStmt_Tool
TFStmt_Tool::TFStmt_Tool(std::vector<std::shared_ptr<TFStmt>> statements, std::shared_ptr<TFToken> line)
{
	this->statements = statements;
	this->line = line;
}
void TFStmt_Tool::accept(TFStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// TFStmt_SettingsBlock
TFStmt_SettingsBlock::TFStmt_SettingsBlock(TFTokenType type, std::vector<std::shared_ptr<TFStmt>> settings, std::shared_ptr<TFToken> line)
{
	this->type = type;
	this->settings = settings;
	this->line = line;
}
void TFStmt_SettingsBlock::accept(TFStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}

// TFStmt_Setting
TFStmt_Setting::TFStmt_Setting(std::shared_ptr<TFExpr> name, std::shared_ptr<TFExpr> value, std::shared_ptr<TFToken> line)
{
	this->name = name;
	this->value = value;
	this->line = line;
}
void TFStmt_Setting::accept(TFStmtVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}