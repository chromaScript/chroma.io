#include "../include/ctoolfile/TFExpr.h"
#include "../include/ctoolfile/TFToken.h"
#include "../include/cscript/CObject.h"

#include <memory>

// TFExpr_Value
TFExpr_Value::TFExpr_Value(std::shared_ptr<CObject> value, std::shared_ptr<TFToken> line)
{
	this->value = value;
	this->line = line;
}
void TFExpr_Value::accept(TFExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> TFExpr_Value::acceptInterpreter(TFExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}

// TFExpr_Name
TFExpr_Name::TFExpr_Name(int settingsID, int settingSubID, std::string name, std::shared_ptr<TFToken> line)
{
	this->settingsID = settingsID;
	this->settingSubID = settingSubID;
	this->settingsName = name;
	this->line = line;
}
void TFExpr_Name::accept(TFExprVisitor_Abstract<void>& visitor)
{
	visitor.visit(shared_from_this());
}
std::shared_ptr<CObject> TFExpr_Name::acceptInterpreter(TFExprVisitor_Abstract<std::shared_ptr<CObject>>& visitor)
{
	return visitor.visit(shared_from_this());
}